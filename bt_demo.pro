TEMPLATE = app

DEPENDPATH += src/
INCLUDEPATH += src/
UI_DIR += src/
MOC_DIR = mocs/
OBJECTS_DIR = objects/
RESOURCES = bt_demo.qrc
QT += dbus network

CONFIG += debug_and_release
CONFIG(debug, debug|release) {
    TARGET = BT_Demo_debug
    QMAKE_CXXFLAGS += -O0
    DEFINES += DEBUG
} else {
    TARGET = BT_Demo
}

INSTALLDIR= $$(INSTALLDIR)
isEmpty(INSTALLDIR) {
    INSTALLDIR = $$(PWD)/tmp/
}
message ("The application will be installed in $$INSTALLDIR")

target.path = $$INSTALLDIR/usr/bin

scripts.path = $$INSTALLDIR/usr/bin
scripts.files = scripts/*

INSTALLS += scripts target

# Input
HEADERS += device.h \
    A2DPTest.h\
    DeviceItem.h \
    DevicesListDelegate.h \
    DevicesListModel.h \
    HIDTest.h \
    Keyboard.h \
    SPPTest.h \
    MainWindow.h \
    adapter.h \
    audiosink.h \
    manager.h \
    types.h \
    agent_adaptor.h \
    agent.h \
    input.h \
    serial.h \
    serial_proxy.h \
    serial_proxy_manager.h \
    LoadingWidget.h


FORMS += ui/MainWindow.ui \
    ui/A2DPTest.ui \
    ui/HIDTest.ui \
    ui/Keyboard.ui \
    ui/SPPTest.ui

SOURCES += main.cpp\
    A2DPTest.cpp\
    device.cpp \
    DeviceItem.cpp \
    DevicesListDelegate.cpp \
    DevicesListModel.cpp \
    HIDTest.cpp \
    Keyboard.cpp \
    SPPTest.cpp \
    MainWindow.cpp \
    adapter.cpp \
    audiosink.cpp \
    manager.cpp \
    agent_adaptor.cpp \
    agent.cpp \
    input.cpp \
    serial.cpp \
    serial_proxy.cpp \
    serial_proxy_manager.cpp \
    LoadingWidget.cpp

