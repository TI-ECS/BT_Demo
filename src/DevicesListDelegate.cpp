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

#include "DevicesListDelegate.h"

#include "DeviceItem.h"

#include <QPainter>
#include <QPoint>
#include <QRect>

#define ICON_OFFSET 70

DevicesListDelegate::DevicesListDelegate(QObject *parent)
    :QItemDelegate(parent)
{
    m_space = 100;
}

DevicesListDelegate::~DevicesListDelegate()
{
}

QWidget* DevicesListDelegate::createEditor(QWidget *,
                                           const QStyleOptionViewItem &,
                                           const QModelIndex &) const
{
    return NULL; // read only
}

void DevicesListDelegate::setEditorData(QWidget *, const QModelIndex &) const
{
    // read only
}

void DevicesListDelegate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex & index ) const
{
    QRect aux = option.rect;
    QStringList labels = index.model()->data(index,
                                             Qt::DisplayRole).toStringList();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawRect(option.rect);
    painter->fillRect(option.rect, QColor(40,40,40));

    aux.setWidth(ICON_OFFSET);
    QImage ic = index.model()->data(index, Qt::DecorationRole).value<QImage>();
    painter->drawImage(QPoint(aux.x() + 3, aux.y() + 4), ic);

    aux.setY(aux.y() + 4);
    aux.setHeight(aux.height() - 4);
    aux.setX(ICON_OFFSET);
    aux.setWidth(option.rect.width() - 2 * ICON_OFFSET);
    painter->setFont(QFont("Arial", 8));
    painter->setPen(Qt::darkGray);
    painter->drawText(aux, Qt::AlignHCenter | Qt::AlignTop, labels.at(2));
    painter->drawText(aux, Qt::AlignHCenter | Qt::AlignBottom, labels.at(3));
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 20, QFont::Bold));
    painter->drawText(aux, Qt::AlignCenter, labels.at(1));

    aux.setX(option.rect.width() - ICON_OFFSET);
    aux.setWidth(ICON_OFFSET);
    QImage img = index.model()->data(index, Qt::UserRole + 1).value<QImage>();
    painter->drawImage(QPoint(aux.x() + 3, aux.y() + 4),
                       img);

    aux.setX(option.rect.width() - ICON_OFFSET - 10);
    aux.setY(aux.y() + aux.height()/ 2 - 6);
    aux.setWidth(ICON_OFFSET);
    QImage lock = index.model()->data(index, Qt::UserRole + 2).value<QImage>();
    painter->drawImage(QPoint(aux.x(), aux.y()), lock);

    painter->restore();
}

void DevicesListDelegate::setModelData(QWidget *, QAbstractItemModel *,
                                       const QModelIndex &) const
{
    // read only
}

QSize DevicesListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                    const QModelIndex &) const
{
    return QSize(option.rect.size().width(), 75);
}

int DevicesListDelegate::spacing() const
{
    return m_space;
}

void DevicesListDelegate::setSpacing(int space)
{
    m_space = space;
}
