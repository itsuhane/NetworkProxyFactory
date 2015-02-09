
/**
             Copyright itsuhane@gmail.com, 2014.
  Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
**/

#ifndef DOWNLOADTEST_HPP
#define DOWNLOADTEST_HPP

#include <QObject>
#include <QNetworkReply>

class DownloadTest : public QObject
{
    Q_OBJECT
public:
    explicit DownloadTest(QObject *parent = 0);

signals:
    void done();

public slots:
    void run();
    void onFinished(QNetworkReply* reply);
};

#endif // DOWNLOADTEST_HPP
