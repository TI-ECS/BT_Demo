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

#include "A2DPTest.h"
#include "types.h"

#include <QFile>
#include <unistd.h>

using namespace org::bluez;

static QString A2DP_CONNECTED = "org.bluez.Error.AlreadyConnected";

A2DPTest::A2DPTest(QWidget *parent)
    :QWidget(parent)
{
    setupUi(this);

    device = NULL;
    aplay.closeReadChannel(QProcess::StandardOutput); // | QProcess::StandardError);
    aplay.closeWriteChannel();

    audioFilesModel = new QFileSystemModel;
    audioFilesModel->setNameFilters(QStringList() << "*.wav");
    audioFilesModel->setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    audioFilesModel->setRootPath(QDir::homePath());
    treeView->setModel(audioFilesModel);

    // Just the filname header.
    QHeaderView *headers = treeView->header();
    for (int i = 1; i < headers->length(); i++)
        headers->hideSection(i);
}

A2DPTest::~A2DPTest()
{
    delete audioFilesModel;
}

void A2DPTest::initTest(DeviceItem *device)
{
    this->device = device;

    QFile asoundrc("/etc/asound.conf");
    if (!asoundrc.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Cannot open /etc/asound.conf to write";
        emit deviceReady(false);
        emit testFinished();
        return;
    }

    QTextStream out(&asoundrc);
    out << QString("pcm.!bluetooth {\n    type bluetooth\n    device %1\n    }").
        arg(device->address());
    asoundrc.close();

    audioSink = new AudioSink(BLUEZ_SERVICE_NAME,
                              device->device()->path(),
                              QDBusConnection::systemBus());

    QDBusPendingCallWatcher *watcher;
    watcher = new QDBusPendingCallWatcher(audioSink->Connect(), this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(connectResult(QDBusPendingCallWatcher*)));
}

void A2DPTest::play()
{
    QModelIndex index = treeView->currentIndex();
    QString file =
        static_cast<QFileSystemModel*>(treeView->model())->filePath(index);

    QStringList args;
    args << "-Dplug:bluetooth" << file;
    aplay.start("/usr/bin/aplay", args);
    if (!aplay.waitForStarted(10000))
        close();

    // Trying to play without jumps
    QProcess::execute("renice", QStringList() << "-18" << QString("%1").arg(aplay.pid()));
}

void A2DPTest::stop()
{
    aplay.close();
}

void A2DPTest::done()
{
    aplay.kill();
    audioSink->Disconnect();
    delete audioSink;
    audioSink = NULL;

    emit testFinished();
}

void A2DPTest::connectResult(QDBusPendingCallWatcher *watcher)
{
    watcher->deleteLater();
    QDBusPendingReply<> reply = *watcher;
    if (!reply.isValid()) {
        if (reply.isError() && reply.error().name() == A2DP_CONNECTED) {
            emit deviceReady(true);
            return;
        }

        delete audioSink;
        audioSink = NULL;
        emit deviceReady(false);
    } else
        emit deviceReady(true);
}

void A2DPTest::playFinished(int, QProcess::ExitStatus)
{
    bt_play->setEnabled(true);
}
