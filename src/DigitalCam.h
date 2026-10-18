#ifndef DIGITAL_CAM_H
#define DIGITAL_CAM_H

#include "AEConfig.h"
#include "entry.h"
#include "AEFX_SuiteHelper.h"
#include "PrSDKAESupport.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_EffectCBSuites.h"
#include "AE_EffectSuites.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "Smart_Utils.h"

#ifdef AE_OS_WIN
#include <windows.h>
#endif

#define MAJOR_VERSION       1
#define MINOR_VERSION       0
#define BUG_VERSION         0
#define STAGE_VERSION       PF_Stage_DEVELOP
#define BUILD_VERSION       1

enum {
    PARAM_INPUT = 0,
    PARAM_CAMERA_MODEL,
    PARAM_NATIVE_EFFECT,
    PARAM_CAMERA_MODE,
    PARAM_FLASH_MODE,
    PARAM_LENS_OPTICS,
    PARAM_GRAIN_TOPIC_START,
    PARAM_GRAIN_AMOUNT,
    PARAM_GRAIN_SIZE,
    PARAM_GRAIN_CLUMPING,
    PARAM_COLORED_GRAIN,
    PARAM_CHROMA_GRAIN,
    PARAM_MANUAL_COLOR,
    PARAM_GRAIN_CONTRAST,
    PARAM_GRAIN_DENSITY,
    PARAM_TEMPORAL_SPEED,
    PARAM_RANDOM_SEED,
    PARAM_GRAIN_TOPIC_END,
    PARAM_BLEND_ORIGINAL,
    DIGITALCAM_NUM_PARAMS
};

extern "C" {
    DllExport PF_Err EffectMain(
        PF_Cmd          cmd,
        PF_InData       *in_data,
        PF_OutData      *out_data,
        PF_ParamDef     *params[],
        PF_LayerDef     *output,
        void            *extra);
}

#endif // DIGITAL_CAM_H
