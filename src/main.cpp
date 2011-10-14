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

#include "Keyboard.h"
#include "MainWindow.h"
#include "types.h"

#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Bluetooth Demo");
    app.setStyleSheet(
        "QScrollBar:vertical {"                                 \
        "    border: 2px solid grey;"                           \
        "    width: 36px;"                                      \
        "    background: rgba(255, 255, 255, 0);"               \
        "    height: 36px;"                                     \
        "    margin: 32px 0 32px 0;"                            \
        "}"                                                     \
        "QScrollBar::handle:vertical {"                         \
        "    background: rgb(181, 181, 181);"                   \
        "    min-height: 20px;"                                 \
        "}"                                                     \
        "QScrollBar::add-line:vertical {"                       \
        "    border: 2px solid grey;"                           \
        "    background: rgb(81, 81, 81);"                      \
        "    height: 30px;"                                     \
        "    subcontrol-position: bottom;"                      \
        "    subcontrol-origin: margin;"                        \
        "}"                                                     \
        "QScrollBar::sub-line:vertical {"                       \
        "    border: 2px solid grey;"                           \
        "    background: rgb(81, 81, 81);"                      \
        "    height: 30px;"                                     \
        "    subcontrol-position: top;"                         \
        "    subcontrol-origin: margin;"                        \
        "}"                                                     \
        "QScrollBar::up-arrow:vertical {"                       \
        "    border: 2px solid grey;"                           \
        "    width: 16px;"                                      \
        "    height: 16px;"                                     \
        "    border-image: url(:/images/go-up.png);"            \
        "}"                                                     \
        "QScrollBar::down-arrow:vertical {"                     \
        "    border: 2px solid grey;"                           \
        "    width: 16px;"                                      \
        "    height: 16px;"                                     \
        "    border-image: url(:/images/go-down.png);"          \
        "}"                                                     \
        "QScrollBar::add-page, QScrollBar::sub-page {"          \
        "    background: none;"                                 \
        "}"                                                     \
        "QScrollBar:horizontal {"                               \
        "    border: 2px solid grey;"                           \
        "    background: rgba(255, 255, 255, 0);"               \
        "    height: 36px;"                                     \
        "    margin: 0px 32px 0 32px;"                          \
        "}"                                                     \
        "QScrollBar::handle:horizontal {"                       \
        "    background: rgb(181, 181, 181);"                   \
        "    min-width: 20px;"                                  \
        "}"                                                     \
        "QScrollBar::add-line:horizontal {"                     \
        "    border: 2px solid grey;"                           \
        "    background: rgb(81, 81, 81);"                      \
        "    width: 30px;"                                      \
        "    subcontrol-position: right;"                       \
        "    subcontrol-origin: margin;"                        \
        "}"                                                     \
        "QScrollBar::sub-line:horizontal {"                     \
        "    border: 2px solid grey;"                           \
        "    background: rgb(81, 81, 81);"                      \
        "    width: 30px;"                                      \
        "    subcontrol-position: left;"                        \
        "    subcontrol-origin: margin;"                        \
        "}"                                                     \
        "QScrollBar::right-arrow:horizontal {"                  \
        "    border: 2px solid grey;"                           \
        "    width: 16px;"                                      \
        "    height: 16px;"                                     \
        "    border-image: url(:/images/go-next.png);"          \
        "}"                                                     \
        "QScrollBar:left-arrow:horizontal  {"                   \
        "    border: 2px solid grey;"                           \
        "    width: 16px;"                                      \
        "    height: 16px;"                                     \
        "    border-image: url(:/images/go-previous.png);"      \
        "}"                                                     \
        "QTreeView {"                                           \
        "    font: 20pt \"Arial\";"                             \
        "}");
    REGISTER_TYPES();

#if !defined(DEBUG)
    setenv("DBUS_SESSION_BUS_ADDRESS", "unix:path=/var/run/dbus/system_bus_socket", 1);
#endif

    MainWindow window;

#if defined(DEBUG)
    window.setMaximumSize(800,480);
    window.show();
#else
    window.showFullScreen();
#endif
    window.setWindowTitle(QApplication::translate("main", "Bluetooth Demo"));
    return app.exec();
}
