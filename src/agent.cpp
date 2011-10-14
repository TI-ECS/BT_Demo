#include "agent.h"
#include "agent_adaptor.h"
#include "Keyboard.h"
#include "MainWindow.h"

#include <QtDBus>
#include <QInputDialog>
#include <QMessageBox>

Agent::Agent(MainWindow *window, QString path)
{
    new AgentAdaptor(this);
    QDBusConnection::systemBus().registerObject(path, this);
    win = window;
}

Agent::~Agent()
{
}

void Agent::reject(const char *msg)
{
    sendErrorReply("org.bluez.Error.Rejected", msg);
}

void Agent::Authorize(const QDBusObjectPath &in0, const QString &in1)
{
    Q_UNUSED(in0);
    Q_UNUSED(in1);

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(win, "Authorize connection ?",
            "Authorize connection ?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
        return;
    reject("Connection rejected by user");
}

void Agent::Cancel()
{
    QString message("Pairing remote device failed.");
    win->pairFailed(message);
}

void Agent::ConfirmModeChange(const QString &in0)
{
    QString message("Authorize mode change to " + in0 + " ?");
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(win, "Mode change",
            message, QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
        return;
    reject("Mode change by user");
}

void Agent::DisplayPasskey(const QDBusObjectPath &in0, uint in1, uchar in2)
{
    Q_UNUSED(in0);
    Q_UNUSED(in2);

    QString message;
    message.sprintf("Passkey is %i", in1);
    QMessageBox::information(win, "Passkey", message);
}

void Agent::Release()
{
    // TODO release it before program closes
}

void Agent::RequestConfirmation(const QDBusObjectPath &in0, uint in1)
{
    Q_UNUSED(in0);

    QString message;
    message.sprintf("Confirm passkey %i ?", in1);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(win, "Request confirmation",
            message, QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
        return;
    reject("Passkey doesn't match");
}

uint Agent::RequestPasskey(const QDBusObjectPath &in0)
{
    Q_UNUSED(in0);

    Keyboard keyboard;
    if (!keyboard.exec())
        return 0;

    bool valid;
    uint ret = keyboard.getText().toUInt(&valid, 10);
    if (!valid)
        return 0;

    return ret;
}

QString Agent::RequestPinCode(const QDBusObjectPath &in0)
{
    Q_UNUSED(in0);

    Keyboard keyboard;
    if (!keyboard.exec())
        return QString();

    return keyboard.getText();
}

void Agent::deviceCreated(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<QDBusObjectPath> reply = *call;

    if (reply.isValid()) {
        win->pairSuccessful();
        return;
    }

    QDBusError error = reply.error();
    QString message("Pairing failed: " + error.name() + " " + error.message());
    win->pairFailed(message);

    call->deleteLater();
}
