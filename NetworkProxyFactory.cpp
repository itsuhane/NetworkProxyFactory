
/**
             Copyright itsuhane@gmail.com, 2014.
  Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
**/

#include "NetworkProxyFactory.hpp"
#include <QFile>
#include <QHostInfo>
#include <QHostAddress>
#include <QNetworkInterface>

QScriptValue dnsResolve(QScriptContext *context, QScriptEngine *engine) {
   Q_UNUSED(engine)
   QString toResolve = context->argument(0).toString();
#ifdef _DEBUG
   qDebug() << "dnsResolve - toResolve: " << toResolve;
#endif
   QHostInfo info = QHostInfo::fromName(toResolve);
   if ( info.error() == QHostInfo::NoError ) {
      return info.addresses().first().toString(); //< Pick the first one
   } else {
      return QString("Host not found");
   }
}

QScriptValue isPlainHostName(QScriptContext *context, QScriptEngine *engine) {
   Q_UNUSED(engine)
   QString hostname = context->argument(0).toString();
#ifdef _DEBUG
   qDebug() << "isPlainHostName - hostname: " << hostname;
#endif
   return hostname.contains('.') ? false : true;
}

QScriptValue isInNet(QScriptContext *context, QScriptEngine *engine) {
   Q_UNUSED(engine)
   QString host = context->argument(0).toString();
   QHostAddress hostAddress;
   QPair<QHostAddress, int> subnet;
   QString pattern = context->argument(1).toString();
   QString mask = context->argument(2).toString();
#ifdef _DEBUG
   qDebug() << "isInNet - host: " << host;
   qDebug() << "isInNet - pattern: " << pattern;
   qDebug() << "isInNet - mask: " << mask;
#endif
   QHostInfo info = QHostInfo::fromName(host);
   if ( info.error() == QHostInfo::NoError ) {
      hostAddress = info.addresses().first();
      subnet = QHostAddress::parseSubnet(QString("%1/%2").arg(pattern,mask));
#ifdef _DEBUG
      qDebug() << "Host "<< host << " is in Subnet " << QString("%1/%2").arg(pattern,mask) << "? " << hostAddress.isInSubnet(subnet);
#endif
      return hostAddress.isInSubnet(subnet);
   } else {
      return false;
   }
}

QScriptValue myIpAddress(QScriptContext *context, QScriptEngine *engine) {
   Q_UNUSED(context)
   Q_UNUSED(engine)

   foreach ( QHostAddress address, QNetworkInterface::allAddresses() ) {
      if( !address.isNull()
         && address != QHostAddress::Null
         && address != QHostAddress::LocalHost
         && address != QHostAddress::LocalHostIPv6
         && address != QHostAddress::Broadcast
         && address != QHostAddress::Any
         && address != QHostAddress::AnyIPv6
         ) {
#ifdef _DEBUG
         qDebug() << "myIpAddress - " << address.toString();
#endif
         return address.toString(); //< Pick the First one
      }
   }
   return "127.0.0.1";
}

NetworkProxyFactory::NetworkProxyFactory()
{
    m_currentMode = DirectMode;
    m_pacEngine.pushContext();
    m_directProxy.setType(QNetworkProxy::NoProxy);
}

bool NetworkProxyFactory::setPacMode(const QString &script)
{
    resetPacEngine();
    if ( m_pacEngine.canEvaluate(script) ) {
        m_pacEngine.evaluate(script);
        m_currentMode = PacMode;
        return true;
    } else {
        m_currentMode = DirectMode;
        return false;
    }
}

QList<QNetworkProxy> NetworkProxyFactory::queryProxy(const QNetworkProxyQuery &query)
{
    QList<QNetworkProxy> resultProxies;

    if(m_currentMode==PacMode) {
        QString str("FindProxyForURL(\"%1\",\"%2\")");
        QScriptValue ctor = m_pacEngine.evaluate(str.arg(query.url().toString()).arg(query.url().host()));
        QString proxyString = "DIRECT";

        if(!ctor.isError()) {
            proxyString = ctor.toString();
        }
#ifdef _DEBUG
        else {
            qDebug()<<"[PAC][DIRECT] Error.";
        }
#endif

        if(proxyString.startsWith("PROXY")) {
            QNetworkProxy proxy;
            proxy.setType(QNetworkProxy::HttpProxy);
            proxyString.remove(0, 6);
            QUrl url("http://"+proxyString);
            proxy.setHostName(url.host());
            proxy.setPort(url.port(8080));
            resultProxies.append(proxy);
#ifdef _DEBUG
            qDebug()<<"[PAC][HTTP]"<<proxy.hostName()<<", port"<<proxy.port();
#endif
        }
        else if(proxyString.startsWith("SOCKS")) {
            QNetworkProxy proxy;
            proxy.setType(QNetworkProxy::Socks5Proxy);
            proxyString.remove(0, 6);
            QUrl url("socks5://"+proxyString);
            proxy.setHostName(url.host());
            proxy.setPort(url.port(8080));
            resultProxies.append(proxy);
#ifdef _DEBUG
            qDebug()<<"[PAC][SOCKS]"<<proxy.hostName()<<", port"<<proxy.port();
#endif
        }
    }

    if(m_currentMode!=DirectMode) {
        resultProxies.append(systemProxyForQuery(query));
    }

    resultProxies.append(m_directProxy);

    return resultProxies;
}

void NetworkProxyFactory::resetPacEngine()
{
    m_pacEngine.popContext();
    m_pacEngine.pushContext();
    QScriptValue dnsResolveFunction = m_pacEngine.newFunction(dnsResolve);
    m_pacEngine.currentContext()->activationObject().setProperty("dnsResolve", dnsResolveFunction);
    QScriptValue isPlainHostNameFunction = m_pacEngine.newFunction(isPlainHostName);
    m_pacEngine.currentContext()->activationObject().setProperty("isPlainHostName", isPlainHostNameFunction);
    QScriptValue isInNetFunction = m_pacEngine.newFunction(isInNet);
    m_pacEngine.currentContext()->activationObject().setProperty("isInNet", isInNetFunction);
    QScriptValue myIpAddressFunction = m_pacEngine.newFunction(myIpAddress);
    m_pacEngine.currentContext()->activationObject().setProperty("myIpAddress", myIpAddressFunction);
}

