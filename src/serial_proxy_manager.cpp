/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -p serial_proxy_manager.h:serial_proxy_manager.cpp -i types.h ../xml/serial_proxy_manager.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "serial_proxy_manager.h"

/*
 * Implementation of interface class OrgBluezSerialProxyManagerInterface
 */

OrgBluezSerialProxyManagerInterface::OrgBluezSerialProxyManagerInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

OrgBluezSerialProxyManagerInterface::~OrgBluezSerialProxyManagerInterface()
{
}

