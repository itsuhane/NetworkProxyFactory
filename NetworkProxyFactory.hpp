
/**
             Copyright itsuhane@gmail.com, 2014.
  Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
**/

#ifndef NETWORKPROXYFACTORY_HPP
#define NETWORKPROXYFACTORY_HPP

#include <QNetworkProxyFactory>
#include <QScriptEngine>

class NetworkProxyFactory : public QNetworkProxyFactory
{
public:
    enum Mode { DirectMode, SystemMode, UniqueMode, SeparatedMode, PacMode };

    NetworkProxyFactory();

    Mode currentMode() const;

    bool setPacMode(const QString& script);

    QList<QNetworkProxy> queryProxy(const QNetworkProxyQuery &query);

signals:

public slots:

private:
    void resetPacEngine();

    Mode m_currentMode;
    QScriptEngine m_pacEngine;
    QNetworkProxy m_directProxy;
};

#endif // NETWORKPROXYFACTORY_HPP
