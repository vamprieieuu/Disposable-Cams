#ifndef _H_SMART_UTILS
#define _H_SMART_UTILS

#include "AE_Effect.h"
#include "AE_EffectCB.h"

inline PF_Boolean IsEmptyRect(const PF_LRect *rect) {
    return (rect->left >= rect->right || rect->top >= rect->bottom);
}

inline void UnionLRect(const PF_LRect *src, PF_LRect *dst) {
    if (IsEmptyRect(dst)) {
        *dst = *src;
    } else if (!IsEmptyRect(src)) {
        if (src->left < dst->left) dst->left = src->left;
        if (src->top < dst->top) dst->top = src->top;
        if (src->right > dst->right) dst->right = src->right;
        if (src->bottom > dst->bottom) dst->bottom = src->bottom;
    }
}

#endif /* _H_SMART_UTILS */
