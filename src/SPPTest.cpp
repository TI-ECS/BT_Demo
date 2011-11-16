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
