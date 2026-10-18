#ifndef DRAWBOT_SUITE_TYPES_H
#define DRAWBOT_SUITE_TYPES_H

typedef struct _DRAWBOT_DrawRef *DRAWBOT_DrawRef;
typedef struct _DRAWBOT_SupplierRef *DRAWBOT_SupplierRef;
typedef struct _DRAWBOT_SurfaceRef *DRAWBOT_SurfaceRef;
typedef struct _DRAWBOT_PathRef *DRAWBOT_PathRef;
typedef struct _DRAWBOT_BrushRef *DRAWBOT_BrushRef;
typedef struct _DRAWBOT_PenRef *DRAWBOT_PenRef;
typedef struct _DRAWBOT_FontRef *DRAWBOT_FontRef;
typedef struct _DRAWBOT_ImageRef *DRAWBOT_ImageRef;

typedef struct DRAWBOT_ColorRGBA {
    float red;
    float green;
    float blue;
    float alpha;
} DRAWBOT_ColorRGBA;

typedef struct DRAWBOT_PointF32 {
    float x;
    float y;
} DRAWBOT_PointF32;

typedef struct DRAWBOT_RectF32 {
    float left;
    float top;
    float width;
    float height;
} DRAWBOT_RectF32;

#endif /* DRAWBOT_SUITE_TYPES_H */
