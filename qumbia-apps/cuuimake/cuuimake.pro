# The application will be installed under INSTALL_ROOT (i.e. prefix)
#
# To set the prefix at build time, call 
# qmake   "INSTALL_ROOT=/my/custom/path"
#
isEmpty(INSTALL_ROOT) {
    INSTALL_ROOT = /usr/local
}

greaterThan(QT_MAJOR_VERSION, 4) {
    QTVER_SUFFIX = -qt$${QT_MAJOR_VERSION}
} else {
    QTVER_SUFFIX =
}


SHAREDIR = $${INSTALL_ROOT}/share
DOCDIR = $${SHAREDIR}/doc/cuuimake

QT -= gui

QT += xml

CONFIG += c++11 console

CONFIG += debug

CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += QT_NO_DEBUG_OUTPUT

# where config files are searched
DEFINES += CONFDIR=\"\\\"$${SHAREDIR}/cuuimake\"\\\"

CUUIMAKE_VERSION_HEX = 0x0001
CUUIMAKE_VERSION = 0.1

DEFINES += CUUIMAKE_VERSION_STR=\"\\\"$${CUUIMAKE_VERSION}\"\\\"

DEFINES += CUUIMAKE_DOCDIR=\"\\\"$${DOCDIR}\"\\\"

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    src/defs.h \
    src/cuuimake.h \
    src/conf.h \
    src/parser.h \
    src/processor.h \
    src/uicproc.h \
    src/options.h \
    src/xmakeprocess.h

SOURCES += src/main.cpp \
    src/defs.cpp \
    src/cuuimake.cpp \
    src/conf.cpp \
    src/parser.cpp \
    src/processor.cpp \
    src/uicproc.cpp \
    src/options.cpp \
    src/xmakeprocess.cpp

DISTFILES += \
    cuuimake-cumbia-qtcontrols.xml

TARGET = bin/cuuimake

target.path = $${INSTALL_ROOT}/bin

conf.path = $${INSTALL_ROOT}/share/cuuimake
conf.files = cuuimake-cumbia-qtcontrols.xml

doc.commands = \
doxygen \
Doxyfile;

doc.files = doc/*
doc.path = $${DOCDIR}

QMAKE_EXTRA_TARGETS += doc

INSTALLS += target conf doc

