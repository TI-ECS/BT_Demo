#include "ObexTest.h"
#include "obex_server_session.h"
#include "types.h"

#include <QDir>
#include <QFileDialog>

static QString ERROR_MESSAGE = "File transfer ended with error";
static QString SUCCESS_MESSAGE = "File transfer ended with success";

using namespace org::openobex;

ObexTest::ObexTest(QWidget *parent)
    :QWidget(parent)
{
    setupUi(this);
    isSending = isTransfering = false;
    server1 = server2 = NULL;
    serverSession = NULL;
    sendSession = NULL;
}

ObexTest::~ObexTest()
{
    if (server1) server1->Close();
    if (server2) server2->Close();
    if (serverSession) serverSession->Cancel();
    if (sendSession) sendSession->Cancel();

    delete manager;
    delete server1;
    delete server2;
    delete serverSession;
    delete sendSession;
}

void ObexTest::acceptTransfer(bool value)
{
    bt_file_done->setEnabled(false);
    bt_file_cancel->setEnabled(true);

    if (value)
        serverSession->Accept();
    else {
        serverSession->Reject();
        serverSession->Disconnect();
    }
}

void ObexTest::cancelClicked()
{
    if (serverSession) {
        serverSession->Cancel();
        sendSession->Close();
    } else {
        sendSession->Cancel();
        sendSession->Close();
        sendSession->Disconnect();
    }

    isSending = false;
    isTransfering = false;

    emit testFinished();
}

void ObexTest::doneClicked()
{
    emit testFinished();
}

void ObexTest::sessionConnected(const QDBusObjectPath &in0)
{
    Q_UNUSED(in0);
    if (!isSending) {
        sendSession->Close();
        sendSession->Disconnect();
        return;
    }

    QString fileToSend;
    QFileDialog dialog(this, "Choose a file to send",
                       "./", "All Files (*.*)");
    dialog.setWindowState(Qt::WindowFullScreen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    if (dialog.exec())
        fileToSend = dialog.selectedFiles().at(0);
    else {
        sendSession->Close();
        sendSession->Disconnect();
        cancelClicked();
        return;
    }

    if (!fileToSend.size()) {
        cancelClicked();
        return;
    }

    QFile file(fileToSend);
    progress_file->setMaximum(file.size());
    progress_file->setValue(0);

    QDBusPendingCallWatcher *watcher;
    watcher = new QDBusPendingCallWatcher(sendSession->SendFile(
                                              fileToSend));

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(sendFileCb(QDBusPendingCallWatcher*)));
}

void ObexTest::sessionCreated(const QDBusObjectPath &in0)
{
    if (isTransfering)
        return;

    if (serverSession)
        delete serverSession;
    serverSession = new ServerSession(OPENOBEX_SERVICE_NAME,
                                      in0.path(),
                                      QDBusConnection::sessionBus());
    if (serverSession) {
        connect(serverSession,
                SIGNAL(TransferStarted(const QString&, const QString &,
                                       qulonglong)),
                this, SLOT(transferStarted(const QString&, const QString &,
                                           qulonglong)));
       connect(serverSession,
               SIGNAL(TransferProgress(qulonglong)),
               this, SLOT(transferProgress(qulonglong)));
       connect(serverSession,
               SIGNAL(TransferCompleted()),
               this, SLOT(transferCompleted()));
        connect(serverSession,
               SIGNAL(ErrorOccurred(const QString&, const QString&)),
               this, SLOT(errorOccurred(const QString&, const QString&)));
   }
}

void ObexTest::serverCreated(QDBusPendingCallWatcher *watcher)
{
    watcher->deleteLater();
    QDBusPendingReply<QDBusObjectPath> reply = *watcher;
    if (!reply.isValid()) {
        if (reply.isError()) {
            qCritical() << reply.error().name();
            return;
        }
    }

    Server *aux;
    static int i = 0;
    if (!i) {
        server1 = new Server(OPENOBEX_SERVICE_NAME, reply.value().path(),
                            QDBusConnection::sessionBus());
        server1->Start(QDir::homePath(), true, false);
        aux = server1;
        i++;
    } else {
        server2 = new Server(OPENOBEX_SERVICE_NAME, reply.value().path(),
                            QDBusConnection::sessionBus());
        server2->Start(QDir::homePath(), true, false);
        aux = server2;
    }

    connect(aux, SIGNAL(SessionCreated(const QDBusObjectPath&)), this,
            SLOT(sessionCreated(const QDBusObjectPath&)));
}

void ObexTest::transferStarted(const QString &in0, const QString &in1, qulonglong in2)
{
    Q_UNUSED(in1);

    if (!isSending)
        emit incomingTransfer(in0, in2);

    if (isTransfering)
        return;

    isTransfering = true;
    progress_file->setMaximum(in2);
}

void ObexTest::transferCompleted()
{
    bt_file_done->setEnabled(true);
    bt_file_cancel->setEnabled(false);
    progress_file->setValue(progress_file->maximum());

    label_file->setText(SUCCESS_MESSAGE);

    if (isSending) {
        sendSession->Close();
        sendSession->Disconnect();
    } else
        serverSession->Disconnect();

    isTransfering = false;
    isSending = false;
}

void ObexTest::transferProgress(qulonglong bytes)
{
    label_file->setText(QString("Transfering: %1").arg(bytes));
    progress_file->setValue(bytes);
}

void ObexTest::sessionConnectError(const QDBusObjectPath &in0, const QString &in1, const QString &in2)
{
    qCritical() << in0.path() << in1 << in2;

    if (isSending) {
        sendSession->Close();
        sendSession->Disconnect();
    } else
        serverSession->Disconnect();

    progress_file->setValue(0);
    bt_file_done->setEnabled(true);
    bt_file_cancel->setEnabled(false);

    label_file->setText(ERROR_MESSAGE);

    isSending = false;
    isTransfering = false;
}

void ObexTest::errorOccurred(const QString &in0, const QString &in1)
{
    qCritical() << "error: " << in0 << in1;

    progress_file->setValue(0);
    bt_file_done->setEnabled(true);
    bt_file_cancel->setEnabled(false);

    if (isSending) {
        sendSession->Close();
        sendSession->Disconnect();
    } else
        serverSession->Disconnect();

    label_file->setText(ERROR_MESSAGE);

    isSending = false;
    isTransfering = false;
}

bool ObexTest::sendFile(const QString &device, const QString &service)
{
    progress_file->setValue(0);
    bt_file_done->setEnabled(false);
    bt_file_cancel->setEnabled(true);
    label_file->setText(QString("Waiting request confirmation ..."));

    QDBusPendingCallWatcher *watcher;
    watcher = new QDBusPendingCallWatcher(manager->CreateBluetoothSession(
                                              device, "00:00:00:00:00:00",
                                              service), this);
    if (!watcher)
        return false;

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(sessionSendCreated(QDBusPendingCallWatcher*)));

    isSending = true;
    return true;
}

void ObexTest::sessionSendCreated(QDBusPendingCallWatcher *watcher)
{
    watcher->deleteLater();
    QDBusPendingReply<QDBusObjectPath> reply = *watcher;
    if (!reply.isValid()) {
        if (reply.isError()) {
            qCritical() << reply.error().name();
            return;
        }
    }

    if (sendSession)
        delete sendSession;

    sendSession = new Session(OPENOBEX_SERVICE_NAME,
                              reply.value().path(),
                              QDBusConnection::sessionBus());
    if (sendSession) {
        connect(sendSession,
                SIGNAL(TransferStarted(const QString&, const QString &,
                                       qulonglong)),
                this, SLOT(transferStarted(const QString&, const QString &,
                                           qulonglong)));
        connect(sendSession,
                SIGNAL(TransferProgress(qulonglong)),
                this, SLOT(transferProgress(qulonglong)));
        connect(sendSession,
                SIGNAL(TransferCompleted()),
                this, SLOT(transferCompleted()));
        connect(sendSession,
                SIGNAL(ErrorOccurred(const QString&, const QString&)),
                this, SLOT(errorOccurred(const QString&, const QString&)));
    }
}

void ObexTest::sendFileCb(QDBusPendingCallWatcher *watcher)
{
    watcher->deleteLater();
    QDBusPendingReply<> reply = *watcher;
    if (!reply.isValid()) {
        if (reply.isError()) {
            qCritical() << reply.error().name();
            isSending = false;
        }
    }
}

void ObexTest::start()
{
    manager = new Manager(OPENOBEX_SERVICE_NAME, "/org/openobex",
                          QDBusConnection::sessionBus());
    if (manager) {

        connect(manager, SIGNAL(SessionConnected(const QDBusObjectPath &)),
                this, SLOT(sessionConnected(const QDBusObjectPath &)));
        connect(manager, SIGNAL(SessionConnectError(const QDBusObjectPath &,
                                                    const QString &, const QString &)),
                this, SLOT(sessionConnectError(const QDBusObjectPath &,
                                               const QString &, const QString &)));

        QDBusPendingCallWatcher *watcherOPP, *watcherFTP;
        watcherOPP = new QDBusPendingCallWatcher(manager->CreateBluetoothServer(
                                                     "00:00:00:00:00:00", "opp",
                                                     false), this);
        connect(watcherOPP, SIGNAL(finished(QDBusPendingCallWatcher*)),
                this, SLOT(serverCreated(QDBusPendingCallWatcher*)));

        watcherFTP = new QDBusPendingCallWatcher(manager->CreateBluetoothServer(
                                                     "00:00:00:00:00:00", "ftp",
                                                     false), this);
        connect(watcherFTP, SIGNAL(finished(QDBusPendingCallWatcher*)),
                this, SLOT(serverCreated(QDBusPendingCallWatcher*)));
    }
}
