#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T14:42:39
#
#-------------------------------------------------
INSTALL_ROOT = /usr/local

greaterThan(QT_MAJOR_VERSION, 4) {
    QTVER_SUFFIX = -qt$${QT_MAJOR_VERSION}
} else {
    QTVER_SUFFIX =
}

CONFIG += debug

INCLUDEDIR = $${INSTALL_ROOT}/include

SHAREDIR = $${INSTALL_ROOT}/share

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qumbiaprojectwizard

TEMPLATE = app

TEMPLATES_INSTALLDIR = $${SHAREDIR}/qumbiaprojectwizard

DEFINES += TEMPLATES_PATH=\"\\\"$${TEMPLATES_INSTALLDIR}\\\"\"

DEFINES += INCLUDE_PATH=\"\\\"$${INCLUDEDIR}\\\"\"

DEFINES -= QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp\
        qumbiaprojectwizard.cpp \
    src/qtango/qtangoimport.cpp \
    src/qtango/main2cu.cpp \
    src/qtango/converter.cpp \
    src/qtango/findreplace.cpp \
    src/qtango/definitions.cpp \
    src/qtango/pro_files_helper.cpp \
    src/qtango/cumbiacodeinjectcmd.cpp \
    src/qtango/codeextractors.cpp \
    src/qtango/codeinjector.cpp \
    src/qtango/maincppwidget2ptr.cpp \
    src/qtango/maincpppreparecmd.cpp \
    src/qtango/conversiondialog.cpp \
    src/qtango/proconvertcmd.cpp \
    projectbackup.cpp \
    src/qtango/prosectionextractor.cpp

HEADERS  += qumbiaprojectwizard.h \
    src/qtango/qtangoimport.h \
    src/qtango/main2cu.h \
    src/qtango/converter.h \
    src/qtango/findreplace.h \
    src/qtango/conversionhealth.h \
    src/qtango/fileprocessor_a.h \
    src/qtango/definitions.h \
    src/qtango/pro_files_helper.h \
    src/qtango/cumbiacodeinjectcmd.h \
    src/qtango/codeextractors.h \
    src/qtango/codeinjector.h \
    src/qtango/maincppwidget2ptr.h \
    src/qtango/maincpppreparecmd.h \
    src/qtango/conversiondialog.h \
    src/qtango/proconvertcmd.h \
    projectbackup.h \
    src/qtango/prosectionextractor.h

FORMS    += qumbiaprojectwizard.ui

target.path = $${INSTALL_ROOT}/bin
target.files = $${TARGET}

templates.path = $${TEMPLATES_INSTALLDIR}
templates.files =  qumbia_project_template/*

INSTALLS = target templates

DISTFILES += \
    qumbia_project_template/qtango.keywords
