#ifndef MHPQTBUTTON_GLOBAL_H
#define MHPQTBUTTON_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef MHPQTBUTTON_LIB
# define MHPQTBUTTON_EXPORT Q_DECL_EXPORT
#else
# define MHPQTBUTTON_EXPORT Q_DECL_IMPORT
#endif

#endif // MHPQTBUTTON_GLOBAL_H
