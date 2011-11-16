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

#ifndef _OBEXTTEST_H_
#define _OBEXTTEST_H_

#include "obex_manager.h"
#include "obex_server.h"
#include "obex_server_session.h"
#include "obex_session.h"
#include "ui_ObexTest.h"


class ObexTest : public QWidget, public Ui::ObexTest
{
Q_OBJECT

public:
    ObexTest(QWidget *parent = 0);
    ~ObexTest();

public slots:
    void acceptTransfer(bool value);
    bool sendFile(const QString &device, const QString &service);
    void start();

private slots:
    void cancelClicked();
    void doneClicked();
    void errorOccurred(const QString &in0, const QString &in1);
    void sessionConnected(const QDBusObjectPath &in0);
    void sessionConnectError(const QDBusObjectPath &in0, const QString &in1, const QString &in2);
    void serverCreated(QDBusPendingCallWatcher *watcher);
    void sessionCreated(const QDBusObjectPath &in0);
    void sessionSendCreated(QDBusPendingCallWatcher *watcher);
    void sendFileCb(QDBusPendingCallWatcher *watcher);
    void transferCompleted();
    void transferProgress(qulonglong bytes);
    void transferStarted(const QString &in0, const QString &in1, qulonglong in2);

signals:
    void testFinished();
    void incomingTransfer(const QString &fileName, qulonglong size);

private:
    bool isTransfering, isSending;
    org::openobex::Manager *manager;
    org::openobex::ServerSession *serverSession;
    // There is two servers (opp and ftp)
    org::openobex::Server *server1, *server2;
    org::openobex::Session *sendSession;
};

#endif /* _OBEXTTEST_H_ */
