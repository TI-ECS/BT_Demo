#ifndef _AGENT_H_
#define _AGENT_H_

#include <QtCore/QObject>
#include <QtDBus/QtDBus>

const char PAIRING_AGENT_PATH[] = "/pairing/agent";

class MainWindow;

class Agent : public QObject, protected QDBusContext
{
    Q_OBJECT

public:
    Agent(MainWindow *window, QString path);
    virtual ~Agent();

public Q_SLOTS: // METHODS
    void Authorize(const QDBusObjectPath &in0, const QString &in1);
    void Cancel();
    void ConfirmModeChange(const QString &in0);
    void DisplayPasskey(const QDBusObjectPath &in0, uint in1, uchar in2);
    void Release();
    void RequestConfirmation(const QDBusObjectPath &in0, uint in1);
    uint RequestPasskey(const QDBusObjectPath &in0);
    QString RequestPinCode(const QDBusObjectPath &in0);
    void deviceCreated(QDBusPendingCallWatcher *call);

private:
    void reject(const char *msg);
    MainWindow *win;
};

#endif /* _AGENT_H_ */
