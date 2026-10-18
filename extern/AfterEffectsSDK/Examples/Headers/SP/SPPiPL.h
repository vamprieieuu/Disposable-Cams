#ifndef __SPPiPL__
#define __SPPiPL__

#include "SPTypes.h"

#pragma pack(push, 4)

typedef uint32 PIType;

#define kSPPiPLVersion 0

typedef struct PIProperty {
    PIType vendorID;
    PIType propertyKey;
    int32 propertyID;
    int32 propertyLength;
    char propertyData[1];
} PIProperty;

typedef struct PIPropertyList {
    int16 version;
    int32 count;
    PIProperty properties[1];
} PIPropertyList;

#pragma pack(pop)

#endif /* __SPPiPL__ */
