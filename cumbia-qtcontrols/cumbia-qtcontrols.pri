#-------------------------------------------------
#
# Project created by QtCreator 2017-04-12T16:47:19
#
#-------------------------------------------------

QT       += widgets opengl


!android-g++ {
    QT += printsupport
}

greaterThan(QT_MAJOR_VERSION, 4) {
    QTVER_SUFFIX = -qt$${QT_MAJOR_VERSION}
} else {
    QTVER_SUFFIX =
}


# + ----------------------------------------------------------------- +
#
# Customization section:
#
# Customize the following paths according to your installation:
#
#
# Here qumbia-qtcontrols will be installed
# INSTALL_ROOT can be specified from the command line running qmake "INSTALL_ROOT=/my/install/path"
#

isEmpty(INSTALL_ROOT) {
    INSTALL_ROOT = /usr/local
}

#
#
# Here qumbia-qtcontrols include files will be installed
    CUMBIA_QTCONTROLS_INCLUDES=$${INSTALL_ROOT}/include/cumbia-qtcontrols
#
#
# Here qumbia-qtcontrols share files will be installed
#
    CUMBIA_QTCONTROLS_SHARE=$${INSTALL_ROOT}/share/cumbia-qtcontrols
#
#
# Here qumbia-qtcontrols libraries will be installed
    CUMBIA_QTCONTROLS_LIBDIR=$${INSTALL_ROOT}/lib
#
#
# Here qumbia-qtcontrols documentation will be installed
    CUMBIA_QTCONTROLS_DOCDIR=$${INSTALL_ROOT}/share/doc/cumbia-qtcontrols
#
# The name of the library
    cumbia_qtcontrols_LIB=cumbia-qtcontrols$${QTVER_SUFFIX}
#
#
#
# ======================== DEPENDENCIES =================================================
#
# Qwt libraries (>= 6.1.2) are installed here:
#   QWT_HOME =
#
exists(/usr/local/qwt-6.1.4) {
    QWT_HOME = /usr/local/qwt-6.1.4
}
exists(/usr/local/qwt-6.1.3) {
    QWT_HOME = /usr/local/qwt-6.1.3
}
exists(/usr/local/qwt-6.1.2) {
    QWT_HOME = /usr/local/qwt-6.1.2
}

QWT_LIB = qwt

QWT_INCLUDES=$${QWT_HOME}/include

QWT_HOME_USR = /usr
QWT_INCLUDES_USR = $${QWT_HOME_USR}/include/qwt

#
# if needed, please
#
# export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
#
# (or wherever cumbia lib is installed) before running qmake
#

unix:!android-g++ {
    CONFIG += link_pkgconfig
    PKGCONFIG += cumbia cumbia-qtcontrols$${QTVER_SUFFIX}

    packagesExist(qwt){
        PKGCONFIG += qwt
        QWT_PKGCONFIG = qwt
        message("cumbia-qtcontrols.pri: using pkg-config to configure qwt includes and libraries")
    }
    else:packagesExist(Qt5Qwt6){
        PKGCONFIG += Qt5Qwt6
        QWT_PKGCONFIG = Qt5Qwt6
        message("cumbia-qtcontrols.pri: using pkg-config to configure qwt includes and libraries (Qt5Qwt6)")
    } else {
        warning("cumbia-qtcontrols.pri: no pkg-config file found")
        warning("cumbia-qtcontrols.pri: export PKG_CONFIG_PATH=/usr/path/to/qwt/lib/pkgconfig if you want to enable pkg-config for qwt")
        warning("cumbia-qtcontrols.pri: if you build and install qwt from sources, be sure to uncomment/enable ")
        warning("cumbia-qtcontrols.pri: QWT_CONFIG     += QwtPkgConfig in qwtconfig.pri qwt project configuration file")
    }
}




#
# + ----------------------------------------------------------------- +
#

DEFINES += CUMBIA_DEBUG_OUTPUT=1

VERSION_HEX = 0x000001
VERSION = 0.0.1

# cumbia plugin directory.
# it should be the same as that defined by
# QUMBIA_PLUGINS_LIBDIR in qumbia-plugins.pri within the qumbia-plugins
# module
#
CU_QTC_PLUGINDIR = $${INSTALL_ROOT}/lib/qumbia-plugins

DEFINES += CUMBIA_QTCONTROLS_VERSION_STR=\"\\\"$${VERSION}\\\"\" \
    CUMBIA_QTCONTROLS_VERSION=$${VERSION_HEX}

QMAKE_CXXFLAGS += -std=c++11 -Wall

freebsd-g++ {
    message( )
    message( *)
    message( * Compiling under FreeBSD)
    message( * :-P)
    message( )
    unix:LIBS -= -ldl
    QMAKE_CXXFLAGS -= -std=c++0x
}

CONFIG += c++11

MOC_DIR = moc
OBJECTS_DIR = obj
FORMS_DIR = ui
LANGUAGE = C++
UI_DIR = src
QMAKE_DEL_FILE = rm \
    -rf
QMAKE_CLEAN = moc \
    obj \
    Makefile \
    *.tag

QMAKE_EXTRA_TARGETS += docs

SHAREDIR = $${INSTALL_ROOT}/share

doc.commands = doxygen \
    Doxyfile;

unix:INCLUDEPATH += \
    $${CUMBIA_QTCONTROLS_INCLUDES} \


unix {
    INCLUDEPATH += $${QWT_INCLUDES} \
    $${QWT_INCLUDES_USR}
}

unix:LIBS +=  \
    -L$${CUMBIA_QTCONTROLS_LIBDIR} -l$${cumbia_qtcontrols_LIB}

# need to adjust qwt path

unix: android-g++ {
    unix:INCLUDEPATH += /usr/local/include/cumbia
    unix:LIBS += -L/libs/armeabi-v7a/ -lcumbia
}


unix: !android-g++ {

    isEmpty(QWT_PKGCONFIG) {
        message("no Qwt pkg-config file found")
        message("adding $${QWT_INCLUDES} and $${QWT_INCLUDES_USR} to include path")
        message("adding  -L$${QWT_HOME_USR}/lib -l$${QWT_LIB}$${QTVER_SUFFIX} to libs")
        message("this should work for ubuntu installations")

        unix:INCLUDEPATH += $${QWT_INCLUDES} $${QWT_INCLUDES_USR} /usr/local/include/cumbia /usr/local/qwt-6.1.3/include
        unix:LIBS += -L$${QWT_HOME_USR}/lib -L/usr/local/lib -L/usr/local/qwt-6.1.3/lib -lcumbia
    }

}
