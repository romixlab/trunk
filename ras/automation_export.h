#ifndef AUTOMATION_EXPORT_H
#define AUTOMATION_EXPORT_H

#include <QtCore/qglobal.h>

#if defined(AUTOMATION_MAKE_LIB) // building lib
#define AUTOMATION_EXPORT Q_DECL_EXPORT
#else // using lib
#define AUTOMATION_EXPORT Q_DECL_IMPORT
#endif

#endif // AUTOMATION_EXPORT_H
