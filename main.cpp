
/**
             Copyright itsuhane@gmail.com, 2014.
  Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
**/

#include <QCoreApplication>
#include <QTimer>
#include "DownloadTest.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    DownloadTest test;
    QObject::connect(&test, SIGNAL(done()), &app, SLOT(quit()));
    QTimer::singleShot(0, &test, SLOT(run()));
    return app.exec();
}
