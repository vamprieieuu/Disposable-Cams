#ifndef __SPTypes__
#define __SPTypes__

#include "SPConfig.h"
#include "PSIntTypes.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef SPAPI
#define SPAPI
#endif

#ifdef WIN_ENV
typedef int32 SPBoolean;
#else
typedef unsigned char SPBoolean;
#endif

typedef int32 SPErr;

#include "SPErrorCodes.h"

#endif /* __SPTypes__ */
