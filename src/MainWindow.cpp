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

#include "MainWindow.h"
#include "agent.h"
#include "agent_adaptor.h"
#include "device.h"
#include "manager.h"
#include "types.h"

#include <QFile>
#include <QFileDialog>
#include <QStringListModel>
#include <QTextStream>
#include <QTimer>
#include <QtDBus>

#define SCAN_TIMEOUT 60000      // 60s
#define DAEMON_TIMEOUT 10000      // 10s

static QString PAIRING("Ensure that the remote device is in pairing/scanning"
                       " mode.\nPairing remote device...");

using namespace org::bluez;

MainWindow::MainWindow(QWidget *parent)
    :QWidget(parent),
     loading(this, Qt::Dialog)
{
    currentDeviceItem = NULL;
    scanning = false;

    setupUi(this);
    lb_logo_left->setPixmap(QPixmap(":/images/logo_pf.png"));
    lb_logo_right->setPixmap(QPixmap(":/images/logo_texas.png"));
    stackedWidget->setCurrentWidget(page_main);

    devicesModel = new DevicesListModel(listView);
    devicesModel->setDevicesList(devices);
    listView->setModel(devicesModel);
    listView->setAutoScroll(true);
    devicesDelegate = new DevicesListDelegate();
    listView->setItemDelegate(devicesDelegate);

#if defined(DEBUG)
    startFinished(0, QProcess::NormalExit);
#else
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(startFinished(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)), this,
            SLOT(processFails(QProcess::ProcessError)));
    process.start("/usr/bin/BT_Init.sh");
    if (!process.waitForStarted(DAEMON_TIMEOUT))
        close();
#endif
}

MainWindow::~MainWindow()
{
    delete timer;

    foreach(DeviceItem *it, devices)
        delete it;

    foreach(Agent *ag, agents)
        delete ag;

    delete adapter;
    delete devicesModel;
    delete devicesDelegate;
}

void MainWindow::deviceFound(const QString &in0, const QVariantMap &in1)
{
    foreach (DeviceItem *dev, devices)
        if (dev->address() == in0) {
            QString name = in1.value("Name").toString();
            dev->updateName(name.isEmpty() ? in0 : name);
            return;
        }

    DeviceItem *item = new DeviceItem(BLUEZ_SERVICE_NAME, adapterPath.path(),
                                      in0, in1.value("Name").toString(), in1.value("Icon").toString(),
                                      in1.value("Paired").toBool(), in1.value("Trusted").toBool());

    devices.append(item);
    devicesModel->setDevicesList(devices);
}

void MainWindow::deviceDisappeared(const QString &device)
{
    // TODO: It's not safe.
    int i = 0;
    foreach (DeviceItem *item, devices) {
        if (item->paired())
            return;

        QString address = item->address();
        if (currentDeviceItem && (address == currentDeviceItem->address()))
            currentDeviceItem = NULL;

        if (address == device) {
            devices.removeAt(i);
            delete item;
            devicesModel->setDevicesList(devices);
            return;
        }
        i++;
    }
}

void MainWindow::deviceRemoved(const QDBusObjectPath &device)
{
    foreach (DeviceItem *item, devices) {
        if (item->device() &&
            (QDBusObjectPath(item->device()->path()) == device)) {
            item->unpair();
            return;
        }
    }
}

void MainWindow::quit()
{
    loading.close();
    close();
}

void MainWindow::quitBT()
{
#if defined(DEBUG)
    loading.close();
    close();
#else
    loading.start("Shutdown");
    process.start("/usr/bin/BT_Exit.sh");
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(shutDown(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(processFails(QProcess::ProcessError)));
#endif
}

void MainWindow::showDevicePage()
{
    stackedWidget->setCurrentWidget(page_device);
}

void MainWindow::showMainPage()
{
    stackedWidget->setCurrentWidget(page_main);
}

void MainWindow::testHID()
{
    if (!currentDeviceItem) {
        stackedWidget->setCurrentWidget(page_main);
	return;
    }

    loading.start("Connecting");

    hid->initTest(currentDeviceItem);
    currentTestPage = page_hid;
}

void MainWindow::testOPP()
{
    if (!currentDeviceItem) {
        showMainPage();
        return;
    }

    if (obex->sendFile(currentDeviceItem->address(), "opp"))
        stackedWidget->setCurrentWidget(page_progress);
}

void MainWindow::testFTP()
{
    if (!currentDeviceItem) {
        showMainPage();
        return;
    }

    if (obex->sendFile(currentDeviceItem->address(), "ftp"))
        stackedWidget->setCurrentWidget(page_progress);
}

void MainWindow::stopDiscovering()
{
    disconnect(adapter, SIGNAL(DeviceFound(const QString&, const QVariantMap&)),
               this, SLOT(deviceFound(const QString&, const QVariantMap&)));
    adapter->StopDiscovery();
    this->bt_scan->setText("Scan devices");
    scanning = false;
}

void MainWindow::scanDevices()
{
    if (scanning) {
        timer->stop();
        stopDiscovering();
        return;
    }

    connect(adapter, SIGNAL(DeviceFound(const QString&, const QVariantMap&)),
            this, SLOT(deviceFound(const QString&, const QVariantMap&)));
    adapter->StartDiscovery();
    this->bt_scan->setText("Stop scan");
    scanning = true;
    timer->start();
}

void MainWindow::showDevice(QModelIndex index)
{
    DeviceItem *device = index.data(Qt::UserRole).value<DeviceItem*>();
    currentDeviceItem = device;

    QDBusPendingReply<QVariantMap> props_reply =
        device->device()->GetProperties();
    props_reply.waitForFinished();
    QVariantMap props = props_reply.value();
    bool paired = props.value("Paired").toBool();

    if (paired) {
        bt_opp->setEnabled(false);
        bt_ftp->setEnabled(false);
        bt_hid->setEnabled(false);
        bt_a2dp->setEnabled(false);
        bt_spp->setEnabled(false);

        foreach (const QVariant &str, props.value("UUIDs").toList()) {
            if (str.toString() == "00001105-0000-1000-8000-00805f9b34fb")
                bt_opp->setEnabled(true);
            else if (str.toString() == "00001106-0000-1000-8000-00805f9b34fb")
                bt_ftp->setEnabled(true);
            else if (str.toString() == "00001124-0000-1000-8000-00805f9b34fb")
                bt_hid->setEnabled(true);
            else if (str.toString() == "0000110b-0000-1000-8000-00805f9b34fb")
                bt_a2dp->setEnabled(true);
            else if (str.toString() == "00001101-0000-1000-8000-00805f9b34fb")
                bt_spp->setEnabled(true);
        }

        bool trusted = props.value("Trusted").toBool();
        if (trusted)
            bt_trust->setText("Untrust Device");
        else
            bt_trust->setText("Trust Device");

        stackedWidget->setCurrentWidget(page_device);
        return;
    }

    QString address = device->address();
    pairDevice(address);
}

void MainWindow::pairDevice(QString address)
{
    QString pairing_agent_path(PAIRING_AGENT_PATH + QString("_") + address);
    pairing_agent_path.replace(':', '_');

    Agent *agent = new Agent(this, pairing_agent_path);
    agents.append(agent);

    bt_pair_done->setEnabled(false);
    bt_pair_cancel->setEnabled(true);
    label_pair->setText(PAIRING);
    stackedWidget->setCurrentWidget(page_pair);

    pairWatcher = new QDBusPendingCallWatcher
        (adapter->CreatePairedDevice
         (address, QDBusObjectPath(pairing_agent_path), "DisplayYesNo"), this);
    connect(pairWatcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            agent, SLOT(deviceCreated(QDBusPendingCallWatcher*)));
}

void MainWindow::getAdapterProperties(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<QVariantMap> reply = *call;
    if (!reply.isValid()) {
        call->deleteLater();
        return;
    }

    QVariantMap properties = reply.value();
    const QDBusArgument& devicesObjects = properties.value("Devices").
        value<QDBusArgument>();
    foreach (const QDBusObjectPath &path, qdbus_cast<QList<QDBusObjectPath> >
             (devicesObjects)) {
        QString address =
            path.path().split("/").last().remove("dev_").replace("_", ":");
        DeviceItem *item = new DeviceItem(BLUEZ_SERVICE_NAME,
                                          adapterPath.path(), address);
        devices.append(item);
    }

    devicesModel->setDevicesList(devices);
    call->deleteLater();
}

void MainWindow::cancelPair()
{
    delete pairWatcher;
    if (currentDeviceItem)
        adapter->CancelDeviceCreation(currentDeviceItem->address());
    stackedWidget->setCurrentWidget(page_main);
}

void MainWindow::pairFailed(QString msg)
{
    label_pair->setText(msg);
    bt_pair_cancel->setEnabled(false);
    bt_pair_done->setEnabled(true);
}

void MainWindow::pairSuccessful()
{
    label_pair->setText("Pairing Successful");
    bt_pair_cancel->setEnabled(false);
    bt_pair_done->setEnabled(true);
}

void MainWindow::unpairDevice()
{
    if (currentDeviceItem)
        adapter->RemoveDevice(QDBusObjectPath
                              (currentDeviceItem->device()->path()));
    stackedWidget->setCurrentWidget(page_main);
}

void MainWindow::trustDevice()
{
    if (!currentDeviceItem) {
        stackedWidget->setCurrentWidget(page_main);
        return;
    }

    QDBusPendingCallWatcher *watcher;
    watcher = new QDBusPendingCallWatcher
        (currentDeviceItem->device()->SetProperty
         (QString("Trusted"), QDBusVariant(!(currentDeviceItem->trusted()))),
         this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(trustedChanged(QDBusPendingCallWatcher*)));
}

void MainWindow::trustedChanged(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<> reply = *watcher;
    watcher->deleteLater();

    if (!reply.isValid()) {
        return;
    }

    if (!currentDeviceItem) {
        stackedWidget->setCurrentWidget(page_main);
        return;
    }

    if (currentDeviceItem->trusted())
        bt_trust->setText("Untrust Device");
    else
        bt_trust->setText("Trust Device");
}

// SPP Server Stuff
void MainWindow::listenSPP()
{
    spp->initListen();
    stackedWidget->setCurrentWidget(page_spp);
}

// SPP Client Stuff
void MainWindow::testSPP()
{
    if (!currentDeviceItem) {
        stackedWidget->setCurrentWidget(page_main);
        return;
    }

    spp->initTest(currentDeviceItem);
    stackedWidget->setCurrentWidget(page_spp);
}

// A2DP Stuff
void MainWindow::testA2DP()
{
    if (!currentDeviceItem) {
        stackedWidget->setCurrentWidget(page_main);
        return;
    }

    this->setEnabled(false);
    loading.start("Connecting");

    QTimer::singleShot(0, this, SLOT(startA2DPTest()));
}

void MainWindow::readyToTest(bool ready)
{
    this->setEnabled(true);
    loading.stop();

    if (ready)
        stackedWidget->setCurrentWidget(currentTestPage);
    else
        stackedWidget->setCurrentWidget(page_main);
}

void MainWindow::startFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);

    disconnect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
               SLOT(startFinished(int, QProcess::ExitStatus)));

    if (exitCode != 0)
        close();

    org::bluez::Manager manager(BLUEZ_SERVICE_NAME, BLUEZ_MANAGER_PATH,
                                QDBusConnection::systemBus());
    QDBusPendingReply<QDBusObjectPath> defaultPath = manager.DefaultAdapter();
    defaultPath.waitForFinished();
    adapterPath = defaultPath.value();

    adapter = new Adapter(BLUEZ_SERVICE_NAME, adapterPath.path(),
                          QDBusConnection::systemBus());

    adapter->SetProperty(QString("Discoverable"), QDBusVariant(true));

    connect(adapter, SIGNAL(DeviceFound(const QString&, const QVariantMap&)),
            this, SLOT(deviceFound(const QString&, const QVariantMap&)));
    connect(adapter, SIGNAL(DeviceDisappeared(const QString&)),
            this, SLOT(deviceDisappeared(const QString&)));
    connect(adapter, SIGNAL(DeviceRemoved(const QDBusObjectPath&)),
            this, SLOT(deviceRemoved(const QDBusObjectPath&)));

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher
        (adapter->GetProperties(), this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(getAdapterProperties(QDBusPendingCallWatcher*)));

    Agent *agent = new Agent(this, PAIRING_AGENT_PATH);
    agents.append(agent);
    adapter->RegisterAgent(QDBusObjectPath(PAIRING_AGENT_PATH),
                           "DisplayYesNo");

    obex->start();
    timer = new QTimer(this);
    timer->setInterval(SCAN_TIMEOUT);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(stopDiscovering()));

    spp->setupSPP(adapterPath);
    scanDevices();

    bt_listen_spp->setEnabled(true);

    connect(obex, SIGNAL(incomingTransfer(const QString&, qulonglong)),
            this, SLOT(newIncomingTransfer(const QString&, qulonglong)));
    loading.stop();
}

void MainWindow::showFullScreen()
{
    QWidget::showFullScreen();
    if (process.state() == QProcess::Running)
        loading.start("Starting");
}

void MainWindow::shutDown(int exitCode, QProcess::ExitStatus status)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(status);

    loading.stop();
    loading.close();

    close();
}

void MainWindow::processFails(QProcess::ProcessError error)
{
    qCritical() << "Process fails with error: " << error;

    exit(error);
}

void MainWindow::startA2DPTest()
{
    a2dp->initTest(currentDeviceItem);
    currentTestPage = page_a2dp;
}

void MainWindow::newIncomingTransfer(const QString &fileName, qulonglong size)
{
    if (stackedWidget->currentIndex()) {
        obex->acceptTransfer(false);
        return;
    }

    int ret = QMessageBox::question(this, tr("Transfer incoming ..."),
                                    QString("File: %1\nSize: %2Kb").
                                            arg(fileName).arg(size/1000),
                                    QMessageBox::Cancel,
                                    QMessageBox::Save);

    if (ret == QMessageBox::Save) {
        obex->acceptTransfer(true);
        stackedWidget->setCurrentWidget(page_progress);
    } else
        obex->acceptTransfer(false);
}
