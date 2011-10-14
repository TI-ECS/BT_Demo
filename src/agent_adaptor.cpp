/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -a agent_adaptor.h:agent_adaptor.cpp ../xml/agent.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "agent_adaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class AgentAdaptor
 */

AgentAdaptor::AgentAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

AgentAdaptor::~AgentAdaptor()
{
    // destructor
}

void AgentAdaptor::Authorize(const QDBusObjectPath &in0, const QString &in1)
{
    // handle method call org.bluez.Agent.Authorize
    QMetaObject::invokeMethod(parent(), "Authorize", Q_ARG(QDBusObjectPath, in0), Q_ARG(QString, in1));
}

void AgentAdaptor::Cancel()
{
    // handle method call org.bluez.Agent.Cancel
    QMetaObject::invokeMethod(parent(), "Cancel");
}

void AgentAdaptor::ConfirmModeChange(const QString &in0)
{
    // handle method call org.bluez.Agent.ConfirmModeChange
    QMetaObject::invokeMethod(parent(), "ConfirmModeChange", Q_ARG(QString, in0));
}

void AgentAdaptor::DisplayPasskey(const QDBusObjectPath &in0, uint in1, uchar in2)
{
    // handle method call org.bluez.Agent.DisplayPasskey
    QMetaObject::invokeMethod(parent(), "DisplayPasskey", Q_ARG(QDBusObjectPath, in0), Q_ARG(uint, in1), Q_ARG(uchar, in2));
}

void AgentAdaptor::Release()
{
    // handle method call org.bluez.Agent.Release
    QMetaObject::invokeMethod(parent(), "Release");
}

void AgentAdaptor::RequestConfirmation(const QDBusObjectPath &in0, uint in1)
{
    // handle method call org.bluez.Agent.RequestConfirmation
    QMetaObject::invokeMethod(parent(), "RequestConfirmation", Q_ARG(QDBusObjectPath, in0), Q_ARG(uint, in1));
}

uint AgentAdaptor::RequestPasskey(const QDBusObjectPath &in0)
{
    // handle method call org.bluez.Agent.RequestPasskey
    uint out0;
    QMetaObject::invokeMethod(parent(), "RequestPasskey", Q_RETURN_ARG(uint, out0), Q_ARG(QDBusObjectPath, in0));
    return out0;
}

QString AgentAdaptor::RequestPinCode(const QDBusObjectPath &in0)
{
    // handle method call org.bluez.Agent.RequestPinCode
    QString out0;
    QMetaObject::invokeMethod(parent(), "RequestPinCode", Q_RETURN_ARG(QString, out0), Q_ARG(QDBusObjectPath, in0));
    return out0;
}

