#-------------------------------------------------
#
# Project created by QtCreator 2017-04-20T12:00:05
#
#-------------------------------------------------

include(cumbia-qtcontrols.pri)

CONFIG += debug

# remove ourselves (defined in .pri)
PKGCONFIG -= cumbia-qtcontrols$${QTVER_SUFFIX}

TARGET = $${cumbia_qtcontrols_LIB}
TEMPLATE = lib

DEFINES += CUMBIAQTCONTROLS_LIBRARY

DEFINES -= QT_NO_DEBUG_OUTPUT

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += cumbiaqtcontrols.cpp \
    src/qubutton.cpp \
    src/qulabel.cpp \
    src/qupalette.cpp \
    src/cucontrolsutils.cpp \
    src/qulineedit.cpp \
    src/qulogimpl.cpp \
    src/qutrendplot.cpp \
#    src/cucharts/callout.cpp \
#    src/cucharts/quplotcommon.cpp \
#    src/cucharts/quspectrumplot.cpp \
#    src/cucharts/quchartconfdialog.cpp \
    src/quled.cpp \
    src/qutable.cpp \
    src/widgets-base/eplotlight/scrollbar.cpp \
    src/widgets-base/number_delegate.cpp \
    src/widgets-base/eng_notation.cpp \
    src/widgets-base/leftclick_with_modifiers_eater.cpp \
    src/qthreadseventbridge.cpp \
    src/qthreadseventbridgefactory.cpp \
    src/cuevent_qt.cpp \
    src/quplotcommon.cpp \
    src/widgets-base/eplotlight/quplot_base.cpp \
    src/widgets-base/eplotlight/quplotzoomcomponent.cpp \
    src/widgets-base/eplotlight/quplotmarkercomponent.cpp \
    src/widgets-base/eplotlight/quplotcanvaspaintercomponent.cpp \
    src/widgets-base/eplotlight/quplotaxescomponent.cpp \
    src/widgets-base/eplotlight/quplotcontextmenucomponent.cpp \
    src/widgets-base/eplotlight/qutimescaledraw.cpp \
    src/widgets-base/eplotlight/quplotcurve.cpp \
    src/quspectrumplot.cpp \
    src/widgets-base/eplotlight/quplotbasecontextmenustrategy.cpp \
    src/cucontrolsfactorypool.cpp \
    src/widgets-base/culinkcontrol.cpp

HEADERS += cumbiaqtcontrols.h\
        cumbia-qtcontrols_global.h \
    src/qubutton.h \
    src/qulabel.h \
    src/cucontrolsreader_abs.h \
    src/cucontrolswriter_abs.h \
    src/cucontrolsfactories_i.h \
    src/cucontrolsfactorypool.h \
    src/qupalette.h \
    src/cucontrolsutils.h \
    src/qulineedit.h \
    src/qulogimpl.h \
#    src/cucharts/callout.h \
#    src/cucharts/quplotcommon.h \
#    src/cucharts/quspectrumplot.h \
#    src/cucharts/quchartconfdialog.h \
    src/quled.h \
    src/qutable.h \
    src/qutrendplot.h \
    src/widgets-base/eplotlight/scrollbar.h \
    src/widgets-base/string_delegate.h \
    src/widgets-base/number_delegate.h \
    src/widgets-base/int_delegate.h \
    src/widgets-base/float_delegate.h \
    src/widgets-base/eng_notation.h \
    src/widgets-base/leftclick_with_modifiers_eater.h \
    src/qthreadseventbridgefactory.h \
    src/qthreadseventbridge.h \
    src/cuevent_qt.h \
    src/quplotcommon.h \
    src/quwidgetupdatestrategy_i.h \
    src/widgets-base/quwidgetinterface.h \
    src/widgets-base/eplotlight/quplot_base.h \
    src/widgets-base/eplotlight/quplotcomponent.h \
    src/widgets-base/eplotlight/quplotzoomcomponent.h \
    src/widgets-base/eplotlight/quplotmarkercomponent.h \
    src/widgets-base/eplotlight/quplotcanvaspaintercomponent.h \
    src/widgets-base/eplotlight/quplotaxescomponent.h \
    src/widgets-base/eplotlight/quplotcontextmenucomponent.h \
    src/widgets-base/eplotlight/qutimescaledraw.h \
    src/widgets-base/eplotlight/quplotcurve.h \
    src/quspectrumplot.h \
    src/quwidgetcontextmenustrategyi.h \
    src/widgets-base/eplotlight/quplotbasecontextmenustrategy.h \
    src/widgets-base/culinkcontrol.h


RESOURCES += \
    src/widgets-base/eimage/confdialog.qrc

FORMS += \
    src/widgets-base/eimage/confdialog.ui \
    src/widgets-base/eplotlight/configuration.ui 

HEADERS += \
    src/widgets-base/eimage/colortablemap.h \
    src/widgets-base/eimage/confdialog.h \
    src/widgets-base/eimage/eimagegl.h \
    src/widgets-base/eimage/eimagemouseeventinterface.h \
    src/widgets-base/eimage/eimagewidget.h \
    src/widgets-base/eimage/eimagewidgetbase.h \
    src/widgets-base/eimage/imagereadevent.h \
    src/widgets-base/eplotlight/ecurvedata.h \
    src/widgets-base/eplotlight/eplot_configuration_widget.h \
 #   src/widgets-base/eplotlight/eplotlight.h \
 #   src/widgets-base/eplotlight/eplotlight_base.h \
 #   src/widgets-base/eplotlight/eplotlight_marker.h \
    src/widgets-base/eplotlight/plotsaver.h \
    src/widgets-base/eplotlight/scrollzoomer.h \
    src/widgets-base/eplotlight/snapshot.h \
    src/widgets-base/dec_int_from_format.h \
    src/widgets-base/eapplybutton.h \
    src/widgets-base/eapplynumeric.h \
    src/widgets-base/earrow.h \
    src/widgets-base/econstants.h \
    src/widgets-base/eflag.h \
    src/widgets-base/egauge.h \
    src/widgets-base/elabel.h \
    src/widgets-base/eled.h \
    src/widgets-base/elettracolors.h \
    src/widgets-base/elineedit.h \
    src/widgets-base/enumeric.h \
    src/widgets-base/esimplelabel.h

SOURCES += \
    src/widgets-base/eimage/colortablemap.cpp \
    src/widgets-base/eimage/confdialog.cpp \
    src/widgets-base/eimage/eimagegl.cpp \
    src/widgets-base/eimage/eimagemouseeventinterface.cpp \
    src/widgets-base/eimage/eimagewidget.cpp \
    src/widgets-base/eimage/eimagewidgetbase.cpp \
    src/widgets-base/eimage/imagereadevent.cpp \
    src/widgets-base/eplotlight/ecurvedata.cpp \
    src/widgets-base/eplotlight/eplot_configuration_widget.cpp \
#    src/widgets-base/eplotlight/eplotlight.cpp \
#    src/widgets-base/eplotlight/eplotlight_base.cpp \
#    src/widgets-base/eplotlight/eplotlight_marker.cpp \
    src/widgets-base/eplotlight/plotsaver.cpp \
    src/widgets-base/eplotlight/scrollzoomer.cpp \
    src/widgets-base/eplotlight/snapshot.cpp \
    src/widgets-base/dec_int_from_format.cpp \
    src/widgets-base/eapplybutton.cpp \
    src/widgets-base/eapplynumeric.cpp \
    src/widgets-base/earrow.cpp \
    src/widgets-base/eflag.cpp \
    src/widgets-base/egauge.cpp \
    src/widgets-base/elabel.cpp \
    src/widgets-base/eled.cpp \
    src/widgets-base/elettracolors.cpp \
    src/widgets-base/elineedit.cpp \
    src/widgets-base/enumeric.cpp \
    src/widgets-base/esimplelabel.cpp

DISTFILES += \
    cumbia-qtcontrols.pri

unix:INCLUDEPATH -= \
    $${QUMBIA_TANGO_CONTROLS_INCLUDES}

unix:INCLUDEPATH += src \
    src/widgets-base \
    src/widgets-base/eplotlight \
    src/widgets-base/eimage

unix:LIBS -= \
    -l$${cumbia_qtcontrols_LIB}

unix {

    doc.commands = \
    doxygen \
    Doxyfile;
    doc.files = doc/*
    doc.path = $${CUMBIA_QTCONTROLS_DOCDIR}

    inc.files = $${HEADERS}
    inc.path = $${CUMBIA_QTCONTROLS_INCLUDES}

    other_inst.files = $${DISTFILES}
    other_inst.path = $${CUMBIA_QTCONTROLS_INCLUDES}

    target.path = $${CUMBIA_QTCONTROLS_LIBDIR}
    INSTALLS += target doc inc other_inst


# generate pkg config file
    CONFIG += create_pc create_prl no_install_prl

    QMAKE_PKGCONFIG_NAME = cumbia-qtcontrols
    QMAKE_PKGCONFIG_DESCRIPTION = Qt controls widget for graphical interfaces over cumbia
    QMAKE_PKGCONFIG_PREFIX = $${INSTALL_ROOT}
    QMAKE_PKGCONFIG_LIBDIR = $${target.path}
    QMAKE_PKGCONFIG_INCDIR = $${inc.path}
    QMAKE_PKGCONFIG_VERSION = $${VERSION}
    QMAKE_PKGCONFIG_DESTDIR = pkgconfig
}
