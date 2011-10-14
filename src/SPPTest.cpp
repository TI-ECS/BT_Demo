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

#include "SPPTest.h"
#include "serial_proxy.h"

using namespace org::bluez;

SPPTest::SPPTest(QWidget *parent)
    :QWidget(parent)
{
    clientConnection = NULL;
    localServer = NULL;
    spp = NULL;
    sppProxyMan = NULL;

    setupUi(this);
}

SPPTest::~SPPTest()
{
    if (sppProxyMan) {
        sppProxyMan->RemoveProxy(proxyPath);
        delete sppProxyMan;
    }

    if (spp)
        delete spp;
}

void SPPTest::setupSPP(const QDBusObjectPath &adapterPath)
{
    spp = NULL;
    clientConnection = NULL;

    QFile::remove("/tmp/bluezserialproxy");
    localServer = new QLocalServer(this);
    localServer->listen("/tmp/bluezserialproxy");
    connect(localServer, SIGNAL(newConnection()),
            this, SLOT(proxyNewConnection()));

    sppProxyMan = new SerialProxyManager(BLUEZ_SERVICE_NAME,
                                         adapterPath.path(),
                                         QDBusConnection::systemBus());
    QDBusPendingCallWatcher *watcher;
    watcher = new QDBusPendingCallWatcher
        (sppProxyMan->CreateProxy("spp", localServer->serverName()), this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(proxyCreatedSPP(QDBusPendingCallWatcher*)));
}

void SPPTest::proxyCreatedSPP(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<QString> reply = *watcher;
    watcher->deleteLater();

    if (!reply.isValid()) {
        delete sppProxyMan;
        sppProxyMan = NULL;
        return;
    }

    proxyPath = reply.value();
    SerialProxy serialProxy(BLUEZ_SERVICE_NAME, proxyPath,
                            QDBusConnection::systemBus());
    serialProxy.Enable();
}

void SPPTest::initTest(DeviceItem *device)
{
    chat_spp->clear();
    msg_spp->setText("");
    msg_spp->setEnabled(true);

    label_spp->setText("Connecting to remote device...");

    spp = new Serial(BLUEZ_SERVICE_NAME,
                     device->device()->path(),
                     QDBusConnection::systemBus());

    QDBusPendingCallWatcher *watcher;
    watcher = new QDBusPendingCallWatcher(spp->Connect("spp"), this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(printSPP(QDBusPendingCallWatcher*)));
}

void SPPTest::initListen()
{
    label_spp->setText
        ("Proxy created.\nMessages received will be displayed on box below.");
    msg_spp->setText("");
    chat_spp->clear();
    msg_spp->setEnabled(false);
}

void SPPTest::msgWrite()
{
    chat_spp->appendPlainText(QString("Me: %1").arg(msg_spp->text()));
    if (!spp) { // Server
        if (clientConnection)
            clientConnection->write(msg_spp->text().toAscii());
        msg_spp->setText("");
        return;
    } // Client
    QTextStream out(&file);
    out << msg_spp->text();
    file.flush();
    msg_spp->setText("");
}

void SPPTest::msgClientRead()
{
    QString msg(file.readAll());
    chat_spp->appendPlainText(QString("Stranger: %1").arg(msg));
}

void SPPTest::msgServerRead()
{
    QString msg(clientConnection->readAll());
    chat_spp->appendPlainText(QString("Stranger: %1").arg(msg));
}

void SPPTest::printSPP(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<QString> reply = *watcher;
    watcher->deleteLater();

    if (!reply.isValid()) {
        label_spp->setText("Connection failed.");
        spp->Disconnect("spp");
        delete spp;
        spp = NULL;
        return;
    }

    devicePath = reply.value();
    file.setFileName(devicePath);
    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Cannot open file: "
                 << qPrintable(file.errorString());
        label_spp->setText("Cannot open file.");
        spp->Disconnect("spp");
        delete spp;
        spp = NULL;
        return;
    }

    connect(&file, SIGNAL(readyRead()), this, SLOT(msgClientRead()));

    label_spp->setText("Connected, use field below to write messages.");
}

void SPPTest::proxyNewConnection()
{
    clientConnection = localServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()), clientConnection,
            SLOT(deleteLater()));
    connect(clientConnection, SIGNAL(readyRead()),
            this, SLOT(msgServerRead()));
}

void SPPTest::done()
{
    if (spp) { // Client
        spp->Disconnect(devicePath);
        delete spp;
        spp = NULL;
    }
    if (file.isOpen())
        file.close();

    emit testFinished();
}
