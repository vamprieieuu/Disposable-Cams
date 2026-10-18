#ifndef __SPPlugs__
#define __SPPlugs__

#include "SPTypes.h"
#include "SPFiles.h"
#include "SPMData.h"
#include "SPProps.h"

#ifdef __cplusplus
extern "C" {
#endif

#define kSPPluginsSuite "SP Plug-ins Suite"
#define kSPPluginsSuiteVersion 4

typedef struct SPPlugin *SPPluginRef;
typedef struct SPPluginList *SPPluginListRef;
typedef struct SPPluginListIterator *SPPluginListIteratorRef;

#ifdef __cplusplus
}
#endif

#endif /* __SPPlugs__ */
