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

#ifndef _A2DPTEST_H_
#define _A2DPTEST_H_

#include "DeviceItem.h"
#include "audiosink.h"
#include "ui_A2DPTest.h"

#include <QFileSystemModel>

class A2DPTest : public QWidget, public Ui::A2DPTest
{
Q_OBJECT

public:

    A2DPTest(QWidget *parent = 0);
    ~A2DPTest();

public slots:

    void initTest(DeviceItem *device);

private slots:

    void connectResult(QDBusPendingCallWatcher *watcher);
    void done();
    void play();
    void playFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void stop();

signals:

    void deviceReady(bool);
    void testFinished();

private:

    QFileSystemModel *audioFilesModel;
    QProcess aplay;
    const DeviceItem *device;
    org::bluez::AudioSink *audioSink;
};

#endif /* _A2DPTEST_H_ */
