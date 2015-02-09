
/**
             Copyright itsuhane@gmail.com, 2014.
  Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
**/

#include "DownloadTest.hpp"
#include <QNetworkAccessManager>
#include <QFile>
#include <QDebug>
#include "NetworkProxyFactory.hpp"

DownloadTest::DownloadTest(QObject *parent) :
    QObject(parent)
{
}

void DownloadTest::run()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    NetworkProxyFactory *factory = new NetworkProxyFactory;

    QFile pacFile("proxy.pac");
    if(!pacFile.open(QFile::ReadOnly)) {
        qWarning()<<"Cannot read PAC file.";
        emit done();
    }

    QString pacScript = pacFile.readAll();
    if(!factory->setPacMode(pacScript)) {
        qWarning()<<"Cannot set PAC mode.";
        emit done();
    }

    manager->setProxyFactory(factory);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://qt-project.org/")));
}

void DownloadTest::onFinished(QNetworkReply *reply)
{
    qDebug()<<reply->readAll();
    emit done();
}
