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

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_


#include "A2DPTest.h"
#include "DeviceItem.h"
#include "DevicesListDelegate.h"
#include "DevicesListModel.h"
#include "HIDTest.h"
#include "Keyboard.h"
#include "LoadingWidget.h"
#include "ObexTest.h"
#include "SPPTest.h"
#include "adapter.h"
#include "agent.h"
#include "device.h"
#include "ui_MainWindow.h"

#include <QDirModel>
#include <QProcess>


class MainWindow : public QWidget, public Ui::MainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void pairFailed(QString msg);
    void pairSuccessful();
    void showDevicePage();

public slots:
    virtual void showFullScreen();
    void readyToTest(bool);
    void showMainPage();

private slots:
    void cancelPair();
    void deviceDisappeared(const QString &device);
    void deviceFound(const QString &in0, const QVariantMap &in1);
    void deviceRemoved(const QDBusObjectPath &device);
    void getAdapterProperties(QDBusPendingCallWatcher *watcher);
    void listenSPP();
    void newIncomingTransfer(const QString &fileName, qulonglong size);
    void processFails(QProcess::ProcessError error);
    void quit();
    void quitBT();
    void scanDevices();
    void showDevice(QModelIndex index);
    void shutDown(int exitCode, QProcess::ExitStatus exitStatus);
    void startA2DPTest();
    void startFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void stopDiscovering();
    void testA2DP();
    void testFTP();
    void testHID();
    void testOPP();
    void testSPP();
    void trustDevice();
    void trustedChanged(QDBusPendingCallWatcher *watcher);
    void unpairDevice();

private:
    DeviceItem *currentDeviceItem;
    DevicesListDelegate *devicesDelegate;
    DevicesListModel *devicesModel;
    LoadingWidget loading;
    QDBusObjectPath adapterPath;
    QDBusPendingCall *pairDevicePendingCall;
    QDBusPendingCallWatcher *pairWatcher;
    QList<Agent *> agents;
    QList<DeviceItem *> devices;
    QProcess process;
    QTimer *timer;
    QWidget *currentTestPage;
    bool scanning;
    org::bluez::Adapter *adapter;
    void pairDevice(QString address);
};

#endif /* _MAINWINDOW_H_ */
