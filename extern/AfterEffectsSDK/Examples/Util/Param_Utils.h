#ifndef _H_PARAM_UTILS
#define _H_PARAM_UTILS

#include <AE_Effect.h>
#include <cstring>

#ifndef PF_ParamFlag_NONE
#define PF_ParamFlag_NONE ((PF_ParamFlags)0)
#endif

#ifndef PF_PUI_NONE
#define PF_PUI_NONE ((PF_ParamUIFlags)0)
#endif

#ifndef PF_ValueDisplayFlag_NONE
#define PF_ValueDisplayFlag_NONE ((PF_ValueDisplayFlags)0)
#endif

#ifndef PF_Precision_INTEGER
#define PF_Precision_INTEGER            0
#define PF_Precision_TENTHS             1
#define PF_Precision_HUNDREDTHS         2
#define PF_Precision_THOUSANDTHS        3
#define PF_Precision_TEN_THOUSANDTHS    4
#endif

#define PF_ADD_POPUP(NAME, CHOICES, DFLT, STRING, ID) \
    do { \
        PF_Err priv_err = PF_Err_NONE; \
        def.param_type = PF_Param_POPUP; \
        PF_STRCPY(def.name, (NAME)); \
        def.u.pd.num_choices = (CHOICES); \
        def.u.pd.dephault = (DFLT); \
        def.u.pd.value = def.u.pd.dephault; \
        def.u.pd.u.namesptr = (STRING); \
        def.uu.id = (ID); \
        if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
    } while (0)

#define PF_ADD_CHECKBOX(NAME_A, NAME_B, DFLT, FLAGS, ID) \
    do { \
        PF_Err priv_err = PF_Err_NONE; \
        def.param_type = PF_Param_CHECKBOX; \
        PF_STRCPY(def.name, (NAME_A)); \
        def.u.bd.u.nameptr = (NAME_B); \
        def.u.bd.value = (DFLT); \
        def.u.bd.dephault = (PF_Boolean)(DFLT); \
        def.flags |= (FLAGS); \
        def.uu.id = (ID); \
        if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
    } while (0)

#define PF_ADD_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, DFLT, ID) \
    do { \
        PF_Err priv_err = PF_Err_NONE; \
        def.param_type = PF_Param_SLIDER; \
        PF_STRCPY(def.name, (NAME)); \
        def.u.sd.value_str[0] = '\0'; \
        def.u.sd.value_desc[0] = '\0'; \
        def.u.sd.valid_min = (VALID_MIN); \
        def.u.sd.slider_min = (SLIDER_MIN); \
        def.u.sd.valid_max = (VALID_MAX); \
        def.u.sd.slider_max = (SLIDER_MAX); \
        def.u.sd.value = def.u.sd.dephault = (DFLT); \
        def.uu.id = (ID); \
        if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
    } while (0)

#define PF_ADD_FLOAT_SLIDERX(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, DFLT, PREC, DISP, FLAGS, ID) \
    do { \
        PF_Err priv_err = PF_Err_NONE; \
        def.param_type = PF_Param_FLOAT_SLIDER; \
        PF_STRCPY(def.name, (NAME)); \
        def.u.fs_d.valid_min = (PF_FpShort)(VALID_MIN); \
        def.u.fs_d.slider_min = (PF_FpShort)(SLIDER_MIN); \
        def.u.fs_d.valid_max = (PF_FpShort)(VALID_MAX); \
        def.u.fs_d.slider_max = (PF_FpShort)(SLIDER_MAX); \
        def.u.fs_d.value = (PF_FpLong)(DFLT); \
        def.u.fs_d.dephault = (PF_FpShort)(DFLT); \
        def.u.fs_d.precision = (PREC); \
        def.u.fs_d.display_flags = (DISP); \
        def.flags |= (FLAGS); \
        def.uu.id = (ID); \
        if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
    } while (0)

#define PF_ADD_COLOR(NAME, RED, GREEN, BLUE, ID) \
    do { \
        PF_Err priv_err = PF_Err_NONE; \
        def.param_type = PF_Param_COLOR; \
        PF_STRCPY(def.name, (NAME)); \
        def.u.cd.dephault.red = (RED); \
        def.u.cd.dephault.green = (GREEN); \
        def.u.cd.dephault.blue = (BLUE); \
        def.u.cd.dephault.alpha = 255; \
        def.u.cd.value = def.u.cd.dephault; \
        def.uu.id = (ID); \
        if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
    } while (0)

#define PF_ADD_TOPIC(NAME, ID) \
    do { \
        PF_Err priv_err = PF_Err_NONE; \
        def.param_type = PF_Param_GROUP_START; \
        PF_STRCPY(def.name, (NAME)); \
        def.uu.id = (ID); \
        if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
    } while (0)

#define PF_END_TOPIC(ID) \
    do { \
        PF_Err priv_err = PF_Err_NONE; \
        def.param_type = PF_Param_GROUP_END; \
        def.uu.id = (ID); \
        if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
    } while (0)

#endif /* _H_PARAM_UTILS */
