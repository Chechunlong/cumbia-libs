#ifndef CUMBIAQTCONTROLS_GLOBAL_H
#define CUMBIAQTCONTROLS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CUMBIAQTCONTROLS_LIBRARY)
#  define CUMBIAQTCONTROLSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CUMBIAQTCONTROLSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CUMBIAQTCONTROLS_GLOBAL_H