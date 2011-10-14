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
