#ifndef NSUSB_GLOBAL_H
#define NSUSB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NSUSB_LIBRARY)
#  define NSUSBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NSUSBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif
