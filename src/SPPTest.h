/*
 *    BlueZ demo
 *    Copyright (C) 2011  ProFUSION embedded systems
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SPPTEST_H_
#define _SPPTEST_H_

#include "DeviceItem.h"
#include "input.h"
#include "serial.h"
#include "serial_proxy_manager.h"
#include "ui_SPPTest.h"

#include <QLocalServer>
#include <QLocalSocket>

class SPPTest : public QWidget, public Ui::SPPTest
{
Q_OBJECT

public:

    SPPTest(QWidget *parent = 0);
    ~SPPTest();

    void setupSPP(const QDBusObjectPath &adapterPath);

public slots:

    void initTest(DeviceItem *device);
    void initListen();

private slots:

    void done();
    void msgClientRead();
    void msgServerRead();
    void msgWrite();
    void printSPP(QDBusPendingCallWatcher *watcher);
    void proxyCreatedSPP(QDBusPendingCallWatcher *watcher);
    void proxyNewConnection();

signals:

    void testFinished();

private:

    QFile file;
    QLocalServer *localServer;
    QLocalSocket *clientConnection;
    QString devicePath;
    QString proxyPath;
    org::bluez::Serial *spp;
    org::bluez::SerialProxyManager *sppProxyMan;
};

#endif /* _SPPTEST_H_ */
