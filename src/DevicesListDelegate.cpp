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
