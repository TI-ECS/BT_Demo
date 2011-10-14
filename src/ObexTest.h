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
