/*
* Bluetooth Demo
*
* Bluetooth demo application.
*
* Copyright (C) 2011 ProFUSION embedded systems
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of ProFUSION embedded systems nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
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
