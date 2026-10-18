#ifndef _H_ENTRY
#define _H_ENTRY

#include "AE_PluginData.h"

#ifdef AE_OS_WIN
    #define DllExport __declspec(dllexport)
#elif defined(__GNUC__)
    #define DllExport __attribute__((visibility("default")))
#else
    #define DllExport
#endif

#define AE_RESERVED_INFO 8

#define PF_REGISTER_EFFECT(IN_PTR, CB_PTR, NAME, MATCH_NAME, CATEGORY, RESERVED_INFO) \
    (*(CB_PTR))((IN_PTR), \
                reinterpret_cast<const A_u_char*>(NAME), \
                reinterpret_cast<const A_u_char*>(MATCH_NAME), \
                reinterpret_cast<const A_u_char*>(CATEGORY), \
                reinterpret_cast<const A_u_char*>("EffectMain"), \
                'eFKT', \
                PF_AE_PLUG_IN_VERSION, \
                PF_AE_PLUG_IN_SUBVERS, \
                (RESERVED_INFO))

#define PF_REGISTER_EFFECT_EXT2(IN_PTR, CB_PTR, NAME, MATCH_NAME, CATEGORY, RESERVED_INFO, ENTRY_NAME, SUPPORT_URL) \
    (*(CB_PTR))((IN_PTR), \
                reinterpret_cast<const A_u_char*>(NAME), \
                reinterpret_cast<const A_u_char*>(MATCH_NAME), \
                reinterpret_cast<const A_u_char*>(CATEGORY), \
                reinterpret_cast<const A_u_char*>(ENTRY_NAME), \
                'eFKT', \
                PF_AE_PLUG_IN_VERSION, \
                PF_AE_PLUG_IN_SUBVERS, \
                (RESERVED_INFO), \
                reinterpret_cast<const A_u_char*>(SUPPORT_URL))

#endif /* _H_ENTRY */
