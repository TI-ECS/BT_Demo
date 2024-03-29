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

#include <QMessageBox>
#include <QTextStream>

#include "SINKTest.h"
#include "types.h"


#define ALSA_CARDS_PROC_FILE "/proc/asound/cards"
using namespace org::bluez;

static QString BLUEZ_ALREADY_CONNECTED = "org.bluez.Error.AlreadyConnected";

SINKTest::SINKTest(QWidget *parent)
    :QWidget(parent),
    m_connected(false),
    m_alsaSink(0)
{
    setupUi(this);

    m_pactl.closeReadChannel(QProcess::StandardOutput);
    m_pactl.closeWriteChannel();

    m_pulse.closeReadChannel(QProcess::StandardOutput);
    m_pulse.closeWriteChannel();

    // Parse command line
    QString blacklistarg = "--blacklist-alsa-cards=";
    foreach (QString arg, QApplication::arguments()) {
        if (!arg.startsWith(blacklistarg))
            continue;

        QStringList l = arg.remove(0, blacklistarg.length()).split(',', QString::SkipEmptyParts);
        foreach(const QString &s, l)
            m_blacklist.insert(s);
    }
}

bool SINKTest::loadAlsaCards()
{
    combo_cards->clear();

    QFile file(ALSA_CARDS_PROC_FILE);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not load list of alsa cards";
        return false;
    }

    QTextStream t(&file);
    for (;;) {
        QString line = t.readLine();
        if (line == NULL)
            break;

        QString idx(line.left(2));
        QString shortname(line.section('-', -1, -1).trimmed());

        // discard useless line, but check for error
        if (t.readLine() == NULL) {
            qDebug() << "Unexpected end of file";
            return false;
        }

        bool blacklisted = m_blacklist.contains(shortname);
        qDebug() << "idx:" << idx.toInt() << " shortname:" << shortname
                 << "blacklisted?" << blacklisted;

        if (!blacklisted)
            combo_cards->addItem(shortname, idx.toInt());
    }

    if (combo_cards->count() > 0)
        combo_cards->setCurrentIndex(0);

    return true;
}

SINKTest::~SINKTest()
{
}

void SINKTest::initLoadLoopbackResult(int exitCode, QProcess::ExitStatus)
{
    disconnect(&m_pactl, SIGNAL(finished(int, QProcess::ExitStatus)), this,
               SLOT(initLoadLoopbackResult(int, QProcess::ExitStatus)));

    if (exitCode != 0) {
        emit deviceReady(false);
        return;
    }

    emit deviceReady(true);
}

void SINKTest::initLoadLoopback()
{
    QStringList args;

    if (m_pactl.state() != QProcess::NotRunning)
        return;

    setVolume(hslider_volume->value());

    args << "load-module" << "module-loopback"
         << QString("source=bluez_source.%1").arg(m_sourceAddr)
         << "sink=audioout";

    connect(&m_pactl, SIGNAL(finished(int, QProcess::ExitStatus)), this,
                SLOT(initLoadLoopbackResult(int, QProcess::ExitStatus)));

    m_pactl.start("/usr/bin/pactl", args);
    if (!m_pactl.waitForStarted(10000)) {
        QMessageBox::critical(this, "pulseaudio", "Could not load loopback module");
        emit deviceReady(false);
    }
}

void SINKTest::initConnectRemoteResult(QDBusPendingCallWatcher *watcher)
{
    watcher->deleteLater();

    QDBusPendingReply<> reply = *watcher;
    if (!reply.isValid() && (!reply.isError()
                             || reply.error().name() !=
                                BLUEZ_ALREADY_CONNECTED)) {
            emit deviceReady(false);
            return;
    }
    // Loopback will be loaded async, when property changed to "connected"
    // or "playing"
}

void SINKTest::initConnectRemote()
{
    QDBusPendingCallWatcher *watcher =
                new QDBusPendingCallWatcher(m_audioSource->Connect(), this);

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(initConnectRemoteResult(QDBusPendingCallWatcher*)));
}

void SINKTest::initStartPulse()
{
    QStringList args;

    args << "/usr/bin/pulseaudio" << "-n" << "--realtime" << "--disallow-exit"
         << "--high-priority" << "--exit-idle-time=-1"
         << "--resample-method=trivial" << "--fail"
         << "-L" << "module-native-protocol-unix"
         << "-L" << "module-bluetooth-discover"
         << "-L" << QString("module-alsa-sink device=\"hw:%1,0\" sink_name=audioout").arg(m_alsaSink);

    m_pulse.start("/usr/bin/dbus-launch", args);
    if (!m_pulse.waitForStarted(10000)) {
        QMessageBox::critical(this, "pulseaudio", "Could not run pulseaudio-server");
        emit deviceReady(false);
    }

    initConnectRemote();
}

void SINKTest::initTest()
{
    emit connectingStarted();
    connect(m_audioSource, SIGNAL(PropertyChanged(const QString&,
                                                  const QDBusVariant&)),
            this, SLOT(propertyChanged(const QString&,
                                       const QDBusVariant&)));
    stackedWidget->setCurrentWidget(page_init_test);
    m_alsaSink = combo_cards->itemData(combo_cards->currentIndex()).toInt();
    initStartPulse();
}

bool SINKTest::initTest(DeviceItem *device)
{
    m_sourceAddr = device->address().replace(':', '_');

    if (!loadAlsaCards()) {
        QMessageBox::critical(this, "alsa", "Could not list sound cards");
        emit deviceReady(false);
        return false;
    }

    if (combo_cards->count() == 0) {
        QMessageBox::critical(this, "alsa", "Could not find any sound card");
        emit deviceReady(false);
        return false;
    }

    m_audioSource = new AudioSource(BLUEZ_SERVICE_NAME,
                                    device->device()->path(),
                                    QDBusConnection::systemBus());

    // Finish previous instance of pulseaudio
    shutdownPulse();

    if (combo_cards->count() == 1)
        initTest();
    else
        stackedWidget->setCurrentWidget(page_choose_card);

    return true;
}

void SINKTest::shutdownPulse()
{
    QProcess k;

    k.closeWriteChannel();
    k.start("/usr/bin/pulseaudio", QStringList() << "--kill");
    if (!k.waitForFinished(10000))
        close();
}

void SINKTest::shutdown()
{
    shutdownPulse();

    delete m_audioSource;
}

void SINKTest::done()
{
    if (m_connected) {
        m_audioSource->Disconnect();
        m_connected = false;
    }

    shutdown();
    emit testFinished();
}

void SINKTest::propertyChanged(const QString &property,
                               const QDBusVariant &value)
{
    if (property != QString("State"))
        return;

    QString v = value.variant().toString();
    label_status->setText(v);

    if (v == QString("disconnected"))
        m_connected = false;
    else if (!m_connected &&
            (v == QString("connected") || v == QString("playing"))) {
        m_connected = true;
        initLoadLoopback();
    }
}

void SINKTest::setVolume(int val)
{
    QProcess k;
    QStringList args;

    args << "set-sink-volume" << "audioout"
         << QString::number((unsigned)((val/100.) * 65535));

    qDebug() << args;

    k.closeWriteChannel();
    k.start("/usr/bin/pactl", args);
    if (!k.waitForFinished(10000))
        close();
}
