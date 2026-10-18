#include "DigitalCam.h"
#include "CameraProfiles.h"
#include "ColorEngine.h"
#include "NoiseEngine.h"

static void RenderPixel8(
    const PF_LayerDef* input_world,
    PF_LayerDef* output_world,
    const RenderPayload& payload,
    const CameraProfile& profile)
{
    if (!input_world || !output_world || !input_world->data || !output_world->data) {
        return;
    }

    const int width = std::min(input_world->width, output_world->width);
    const int height = std::min(input_world->height, output_world->height);
    if (width <= 0 || height <= 0) {
        return;
    }

    const float inv_w = (width > 1) ? (1.0f / static_cast<float>(width - 1)) : 0.0f;
    const float inv_h = (height > 1) ? (1.0f / static_cast<float>(height - 1)) : 0.0f;

    const char* in_base = reinterpret_cast<const char*>(input_world->data);
    char* out_base = reinterpret_cast<char*>(output_world->data);

    for (int y = 0; y < height; ++y) {
        const PF_Pixel8* in_row = reinterpret_cast<const PF_Pixel8*>(in_base + static_cast<ptrdiff_t>(y) * input_world->rowbytes);
        PF_Pixel8* out_row = reinterpret_cast<PF_Pixel8*>(out_base + static_cast<ptrdiff_t>(y) * output_world->rowbytes);
        const float v = static_cast<float>(y) * inv_h;

        for (int x = 0; x < width; ++x) {
            const PF_Pixel8& src = in_row[x];
            PF_Pixel8& dst = out_row[x];
            dst.alpha = src.alpha;

            if (src.alpha == 0) {
                dst.red = src.red;
                dst.green = src.green;
                dst.blue = src.blue;
                continue;
            }

            const float u = static_cast<float>(x) * inv_w;
            const float in_r = static_cast<float>(src.red) * (1.0f / 255.0f);
            const float in_g = static_cast<float>(src.green) * (1.0f / 255.0f);
            const float in_b = static_cast<float>(src.blue) * (1.0f / 255.0f);

            float out_r = in_r, out_g = in_g, out_b = in_b;
            DigitalCamColor::ProcessPixel(in_r, in_g, in_b, u, v, width, height, payload, profile, out_r, out_g, out_b);

            dst.red   = static_cast<A_u_char>(DigitalCamColor::Clamp01(out_r) * 255.0f + 0.5f);
            dst.green = static_cast<A_u_char>(DigitalCamColor::Clamp01(out_g) * 255.0f + 0.5f);
            dst.blue  = static_cast<A_u_char>(DigitalCamColor::Clamp01(out_b) * 255.0f + 0.5f);
        }
    }
}

static void RenderPixel16(
    const PF_LayerDef* input_world,
    PF_LayerDef* output_world,
    const RenderPayload& payload,
    const CameraProfile& profile)
{
    if (!input_world || !output_world || !input_world->data || !output_world->data) {
        return;
    }

    const int width = std::min(input_world->width, output_world->width);
    const int height = std::min(input_world->height, output_world->height);
    if (width <= 0 || height <= 0) {
        return;
    }

    const float inv_w = (width > 1) ? (1.0f / static_cast<float>(width - 1)) : 0.0f;
    const float inv_h = (height > 1) ? (1.0f / static_cast<float>(height - 1)) : 0.0f;

    const char* in_base = reinterpret_cast<const char*>(input_world->data);
    char* out_base = reinterpret_cast<char*>(output_world->data);

    for (int y = 0; y < height; ++y) {
        const PF_Pixel16* in_row = reinterpret_cast<const PF_Pixel16*>(in_base + static_cast<ptrdiff_t>(y) * input_world->rowbytes);
        PF_Pixel16* out_row = reinterpret_cast<PF_Pixel16*>(out_base + static_cast<ptrdiff_t>(y) * output_world->rowbytes);
        const float v = static_cast<float>(y) * inv_h;

        for (int x = 0; x < width; ++x) {
            const PF_Pixel16& src = in_row[x];
            PF_Pixel16& dst = out_row[x];
            dst.alpha = src.alpha;

            if (src.alpha == 0) {
                dst.red = src.red;
                dst.green = src.green;
                dst.blue = src.blue;
                continue;
            }

            const float u = static_cast<float>(x) * inv_w;
            const float in_r = static_cast<float>(src.red) * (1.0f / 32768.0f);
            const float in_g = static_cast<float>(src.green) * (1.0f / 32768.0f);
            const float in_b = static_cast<float>(src.blue) * (1.0f / 32768.0f);

            float out_r = in_r, out_g = in_g, out_b = in_b;
            DigitalCamColor::ProcessPixel(in_r, in_g, in_b, u, v, width, height, payload, profile, out_r, out_g, out_b);

            dst.red   = static_cast<A_u_short>(DigitalCamColor::Clamp01(out_r) * 32768.0f + 0.5f);
            dst.green = static_cast<A_u_short>(DigitalCamColor::Clamp01(out_g) * 32768.0f + 0.5f);
            dst.blue  = static_cast<A_u_short>(DigitalCamColor::Clamp01(out_b) * 32768.0f + 0.5f);
        }
    }
}

static void RenderPixelFloat(
    const PF_LayerDef* input_world,
    PF_LayerDef* output_world,
    const RenderPayload& payload,
    const CameraProfile& profile)
{
    if (!input_world || !output_world || !input_world->data || !output_world->data) {
        return;
    }

    const int width = std::min(input_world->width, output_world->width);
    const int height = std::min(input_world->height, output_world->height);
    if (width <= 0 || height <= 0) {
        return;
    }

    const float inv_w = (width > 1) ? (1.0f / static_cast<float>(width - 1)) : 0.0f;
    const float inv_h = (height > 1) ? (1.0f / static_cast<float>(height - 1)) : 0.0f;

    const char* in_base = reinterpret_cast<const char*>(input_world->data);
    char* out_base = reinterpret_cast<char*>(output_world->data);

    for (int y = 0; y < height; ++y) {
        const PF_PixelFloat* in_row = reinterpret_cast<const PF_PixelFloat*>(in_base + static_cast<ptrdiff_t>(y) * input_world->rowbytes);
        PF_PixelFloat* out_row = reinterpret_cast<PF_PixelFloat*>(out_base + static_cast<ptrdiff_t>(y) * output_world->rowbytes);
        const float v = static_cast<float>(y) * inv_h;

        for (int x = 0; x < width; ++x) {
            const PF_PixelFloat& src = in_row[x];
            PF_PixelFloat& dst = out_row[x];
            dst.alpha = src.alpha;

            if (src.alpha <= 0.0f) {
                dst.red = src.red;
                dst.green = src.green;
                dst.blue = src.blue;
                continue;
            }

            const float u = static_cast<float>(x) * inv_w;
            const float in_r = DigitalCamColor::Clamp01(src.red);
            const float in_g = DigitalCamColor::Clamp01(src.green);
            const float in_b = DigitalCamColor::Clamp01(src.blue);

            float out_r = in_r, out_g = in_g, out_b = in_b;
            DigitalCamColor::ProcessPixel(in_r, in_g, in_b, u, v, width, height, payload, profile, out_r, out_g, out_b);

            dst.red   = out_r;
            dst.green = out_g;
            dst.blue  = out_b;
        }
    }
}

static PF_Err About(
    PF_InData       *in_data,
    PF_OutData      *out_data)
{
    PF_SPRINTF(
        out_data->return_msg,
        "%s v%d.%d\r\r%s",
        "Disposable Cams",
        MAJOR_VERSION,
        MINOR_VERSION,
        "Disposable Cams - Digital and disposable camera emulation engine.");
    return PF_Err_NONE;
}

static PF_Err GlobalSetup(
    PF_OutData      *out_data)
{
    CameraProfileManager::EnsureInitialized();

    out_data->my_version = PF_VERSION(
        MAJOR_VERSION,
        MINOR_VERSION,
        BUG_VERSION,
        STAGE_VERSION,
        BUILD_VERSION);



    out_data->num_params = DIGITALCAM_NUM_PARAMS;

    out_data->out_flags =
        PF_OutFlag_PIX_INDEPENDENT |
        PF_OutFlag_DEEP_COLOR_AWARE |
        PF_OutFlag_NON_PARAM_VARY |
        PF_OutFlag_SEND_UPDATE_PARAMS_UI;

    out_data->out_flags2 =
        PF_OutFlag2_SUPPORTS_SMART_RENDER |
        PF_OutFlag2_FLOAT_COLOR_AWARE |
        PF_OutFlag2_SUPPORTS_THREADED_RENDERING;

    return PF_Err_NONE;
}

static PF_Err ParamsSetup(
    PF_InData       *in_data,
    PF_OutData      *out_data)
{
    PF_Err err = PF_Err_NONE;
    PF_ParamDef def;

    AEFX_CLR_STRUCT(def);
    PF_ADD_POPUP(
        "Camera Engine",
        19,
        1,
        "CPM35 — Disposable Film|"
        "Classic U — Compact Film|"
        "IR — Infrared Response|"
        "FQS — Flash Quick Snapshot|"
        "CCD R — CCD Color Response|"
        "FXN — Flash X-Negative|"
        "DHalf — Half-Frame Film|"
        "Amber Flash 3792|"
        "Cool Snapshot 3793|"
        "Soft Summer 3794|"
        "Deep Shadow 3795|"
        "Warm Compact 3797|"
        "Green Cast 3798|"
        "Hard Flash 3799|"
        "Muted Film 3802|"
        "Cyan Night 3804|"
        "Bright CCD 3808|"
        "Matte Daylight 3809|"
        "Red Halation 3813",
        PARAM_CAMERA_MODEL);

    AEFX_CLR_STRUCT(def);
    PF_ADD_POPUP(
        "Color Rendering",
        8,
        1,
        "Engine Default|Soft Highlight Roll-Off|Hard Direct Flash|Warm Film Bias|Cool CCD Bias|Low-Saturation Matte|High-Saturation Compact|Monochrome Film",
        PARAM_NATIVE_EFFECT);

    AEFX_CLR_STRUCT(def);
    PF_ADD_POPUP(
        "Capture Conditions",
        5,
        1,
        "Neutral / Available Light|Direct Flash|Warm Indoor Light|Low-Light Snapshot|Bright Daylight",
        PARAM_CAMERA_MODE);

    AEFX_CLR_STRUCT(def);
    PF_ADD_POPUP(
        "Flash Behavior",
        4,
        1,
        "Disabled|Automatic Snapshot Flash|Forced Direct Flash|Slow-Sync Flash",
        PARAM_FLASH_MODE);

    AEFX_CLR_STRUCT(def);
    PF_ADD_CHECKBOX(
        "Lens Character",
        "Enable Lens Falloff & Optical Character",
        TRUE,
        0,
        PARAM_LENS_OPTICS);

    AEFX_CLR_STRUCT(def);
    def.flags = PF_ParamFlag_START_COLLAPSED;
    PF_ADD_TOPIC("Film & Sensor Grain", PARAM_GRAIN_TOPIC_START);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Grain Strength",
        0.0f, 100.0f, 0.0f, 100.0f, 25.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_GRAIN_AMOUNT);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Grain Size (8mm - 68mm)",
        8.0f, 68.0f, 8.0f, 68.0f, 24.0f,
        PF_Precision_HUNDREDTHS, 0, 0,
        PARAM_GRAIN_SIZE);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Grain Clumping",
        0.0f, 100.0f, 0.0f, 100.0f, 35.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_GRAIN_CLUMPING);

    AEFX_CLR_STRUCT(def);
    def.flags = PF_ParamFlag_SUPERVISE;
    PF_ADD_CHECKBOX(
        "Chroma Grain / Colored Grain",
        "Use Camera Chromatic Grain",
        TRUE,
        PF_ParamFlag_SUPERVISE,
        PARAM_COLORED_GRAIN);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Chroma Grain",
        0.0f, 100.0f, 0.0f, 100.0f, 50.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_CHROMA_GRAIN);

    AEFX_CLR_STRUCT(def);
    def.ui_flags = PF_PUI_DISABLED;
    PF_ADD_COLOR(
        "Grain Color",
        PF_MAX_CHAN8, PF_MAX_CHAN8, PF_MAX_CHAN8,
        PARAM_MANUAL_COLOR);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Grain Contrast",
        0.0f, 100.0f, 0.0f, 100.0f, 50.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_GRAIN_CONTRAST);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Grain Density",
        0.0f, 100.0f, 0.0f, 100.0f, 75.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_GRAIN_DENSITY);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Grain Motion Speed",
        0.0f, 5.0f, 0.0f, 5.0f, 1.0f,
        PF_Precision_HUNDREDTHS, 0, 0,
        PARAM_TEMPORAL_SPEED);

    AEFX_CLR_STRUCT(def);
    PF_ADD_SLIDER(
        "Random Seed",
        0, 32767, 0, 32767, 1234,
        PARAM_RANDOM_SEED);

    AEFX_CLR_STRUCT(def);
    PF_END_TOPIC(PARAM_GRAIN_TOPIC_END);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Original Mix",
        0.0f, 100.0f, 0.0f, 100.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_BLEND_ORIGINAL);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Exposure", -5.0f, 5.0f, -5.0f, 5.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_EXPOSURE);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Contrast", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_CONTRAST);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Highlights", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_HIGHLIGHTS);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Shadows", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_SHADOWS);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Whites", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_WHITES);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Blacks", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_BLACKS);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Temperature", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_TEMPERATURE);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Tint", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_TINT);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Vibrance", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_VIBRANCE);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Saturation", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_SATURATION);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Clarity", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_CLARITY);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Dehaze", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_DEHAZE);

    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX(
        "Develop Vignette", 0.0f, 100.0f, 0.0f, 100.0f, 0.0f,
        PF_Precision_TENTHS, 0, 0,
        PARAM_VIGNETTE);


    AEFX_CLR_STRUCT(def); def.flags = PF_ParamFlag_START_COLLAPSED; PF_ADD_TOPIC("Color Mixer (HSL)", PARAM_HSL_TOPIC_START);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Red Hue", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_HUE_RED);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Red Saturation", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_SAT_RED);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Red Luminance", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_LUM_RED);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Orange Hue", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_HUE_ORANGE);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Orange Saturation", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_SAT_ORANGE);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Orange Luminance", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_LUM_ORANGE);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Yellow Hue", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_HUE_YELLOW);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Yellow Saturation", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_SAT_YELLOW);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Yellow Luminance", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_LUM_YELLOW);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Green Hue", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_HUE_GREEN);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Green Saturation", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_SAT_GREEN);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Green Luminance", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_LUM_GREEN);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Aqua Hue", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_HUE_AQUA);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Aqua Saturation", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_SAT_AQUA);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Aqua Luminance", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_LUM_AQUA);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Blue Hue", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_HUE_BLUE);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Blue Saturation", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_SAT_BLUE);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Blue Luminance", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_LUM_BLUE);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Purple Hue", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_HUE_PURPLE);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Purple Saturation", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_SAT_PURPLE);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Purple Luminance", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_LUM_PURPLE);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Magenta Hue", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_HUE_MAGENTA);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Magenta Saturation", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_SAT_MAGENTA);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Magenta Luminance", -100.0f, 100.0f, -100.0f, 100.0f, 0.0f, PF_Precision_TENTHS, 0, 0, PARAM_LUM_MAGENTA);
    AEFX_CLR_STRUCT(def); PF_END_TOPIC(PARAM_HSL_TOPIC_END);
    AEFX_CLR_STRUCT(def); def.flags = PF_ParamFlag_START_COLLAPSED; PF_ADD_TOPIC("Tone Curves", PARAM_CURVE_TOPIC_START);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Master Curve", -100, 100, -100, 100, 0, PF_Precision_TENTHS, 0, 0, PARAM_CURVE_RGB);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Red Curve", -100, 100, -100, 100, 0, PF_Precision_TENTHS, 0, 0, PARAM_CURVE_RED);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Green Curve", -100, 100, -100, 100, 0, PF_Precision_TENTHS, 0, 0, PARAM_CURVE_GREEN);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Blue Curve", -100, 100, -100, 100, 0, PF_Precision_TENTHS, 0, 0, PARAM_CURVE_BLUE);
    AEFX_CLR_STRUCT(def); PF_END_TOPIC(PARAM_CURVE_TOPIC_END);
    AEFX_CLR_STRUCT(def); def.flags = PF_ParamFlag_START_COLLAPSED; PF_ADD_TOPIC("Color Grading", PARAM_GRADE_TOPIC_START);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Shadow Hue", -100, 100, -100, 100, 0, PF_Precision_TENTHS, 0, 0, PARAM_SHADOW_HUE);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Shadow Saturation", -100, 100, -100, 100, 0, PF_Precision_TENTHS, 0, 0, PARAM_SHADOW_SAT);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Midtone Hue", -100, 100, -100, 100, 0, PF_Precision_TENTHS, 0, 0, PARAM_MID_HUE);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Midtone Saturation", -100, 100, -100, 100, 0, PF_Precision_TENTHS, 0, 0, PARAM_MID_SAT);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Highlight Hue", -100, 100, -100, 100, 0, PF_Precision_TENTHS, 0, 0, PARAM_HIGHLIGHT_HUE);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Highlight Saturation", -100, 100, -100, 100, 0, PF_Precision_TENTHS, 0, 0, PARAM_HIGHLIGHT_SAT);
    AEFX_CLR_STRUCT(def); PF_END_TOPIC(PARAM_GRADE_TOPIC_END);
    AEFX_CLR_STRUCT(def); def.flags = PF_ParamFlag_START_COLLAPSED; PF_ADD_TOPIC("Detail & Noise Reduction", PARAM_DETAIL_TOPIC_START);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Sharpening", 0, 100, 0, 100, 0, PF_Precision_TENTHS, 0, 0, PARAM_SHARPENING);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Sharpen Radius", 1, 5, 1, 5, 1, PF_Precision_TENTHS, 0, 0, PARAM_SHARPEN_RADIUS);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Luminance Noise Reduction", 0, 100, 0, 100, 0, PF_Precision_TENTHS, 0, 0, PARAM_NOISE_REDUCTION);
    AEFX_CLR_STRUCT(def); PF_ADD_FLOAT_SLIDERX("Color Noise Reduction", 0, 100, 0, 100, 0, PF_Precision_TENTHS, 0, 0, PARAM_COLOR_NOISE_REDUCTION);
    AEFX_CLR_STRUCT(def); PF_END_TOPIC(PARAM_DETAIL_TOPIC_END);

    out_data->num_params = DIGITALCAM_NUM_PARAMS;
    return err;
}

static PF_Err UpdateParamsUI(
    PF_InData       *in_data,
    PF_OutData      *out_data,
    PF_ParamDef     *params[])
{
    PF_Err err = PF_Err_NONE;
    if (!in_data || !out_data || !params ||
        !params[PARAM_COLORED_GRAIN] ||
        !params[PARAM_CHROMA_GRAIN] ||
        !params[PARAM_MANUAL_COLOR]) {
        return PF_Err_NONE;
    }

    const bool colored_grain_on = (params[PARAM_COLORED_GRAIN]->u.bd.value != 0);

    AEFX_SuiteScoper<PF_ParamUtilsSuite3> paramUtilsP(
        in_data,
        kPFParamUtilsSuite,
        kPFParamUtilsSuiteVersion3,
        out_data);

    if (paramUtilsP.get()) {
        PF_ParamDef chroma_def = *params[PARAM_CHROMA_GRAIN];
        if (colored_grain_on) {
            chroma_def.ui_flags &= ~PF_PUI_DISABLED;
        } else {
            chroma_def.ui_flags |= PF_PUI_DISABLED;
        }
        ERR(paramUtilsP->PF_UpdateParamUI(in_data->effect_ref, PARAM_CHROMA_GRAIN, &chroma_def));

        PF_ParamDef manual_color_def = *params[PARAM_MANUAL_COLOR];
        if (colored_grain_on) {
            manual_color_def.ui_flags |= PF_PUI_DISABLED;
        } else {
            manual_color_def.ui_flags &= ~PF_PUI_DISABLED;
        }
        ERR(paramUtilsP->PF_UpdateParamUI(in_data->effect_ref, PARAM_MANUAL_COLOR, &manual_color_def));
    }

    return err;
}

static PF_Err PreRender(
    PF_InData           *in_data,
    PF_OutData          *out_data,
    PF_PreRenderExtra   *extra)
{
    PF_Err err = PF_Err_NONE;
    if (!extra || !extra->cb || !extra->input || !extra->output) {
        return PF_Err_BAD_CALLBACK_PARAM;
    }

    PF_RenderRequest req = extra->input->output_request;
    req.preserve_rgb_of_zero_alpha = TRUE;

    PF_CheckoutResult in_result;
    AEFX_CLR_STRUCT(in_result);

    ERR(extra->cb->checkout_layer(
        in_data->effect_ref,
        PARAM_INPUT,
        PARAM_INPUT,
        &req,
        in_data->current_time,
        in_data->time_step,
        in_data->time_scale,
        &in_result));

    UnionLRect(&in_result.result_rect, &extra->output->result_rect);
    UnionLRect(&in_result.max_result_rect, &extra->output->max_result_rect);

    return err;
}

static PF_Err SmartRender(
    PF_InData           *in_data,
    PF_OutData          *out_data,
    PF_SmartRenderExtra *extra)
{
    PF_Err err = PF_Err_NONE;
    PF_Err err2 = PF_Err_NONE;

    if (!extra || !extra->cb) {
        return PF_Err_BAD_CALLBACK_PARAM;
    }

    PF_EffectWorld *input_worldP = nullptr;
    PF_EffectWorld *output_worldP = nullptr;

    ERR(extra->cb->checkout_layer_pixels(in_data->effect_ref, PARAM_INPUT, &input_worldP));
    ERR(extra->cb->checkout_output(in_data->effect_ref, &output_worldP));

    if (!err && input_worldP && output_worldP && input_worldP->data && output_worldP->data) {
        PF_ParamDef p_model, p_effect, p_mode, p_flash, p_optics;
        PF_ParamDef p_g_amt, p_g_size, p_g_clump, p_c_grain, p_g_chroma, p_m_col, p_g_cont, p_g_dens, p_t_speed, p_seed, p_blend;
        PF_ParamDef p_develop[13];
        const int develop_ids[13] = { PARAM_EXPOSURE, PARAM_CONTRAST, PARAM_HIGHLIGHTS, PARAM_SHADOWS, PARAM_WHITES, PARAM_BLACKS, PARAM_TEMPERATURE, PARAM_TINT, PARAM_VIBRANCE, PARAM_SATURATION, PARAM_CLARITY, PARAM_DEHAZE, PARAM_VIGNETTE };
        for (int i = 0; i < 13; ++i) AEFX_CLR_STRUCT(p_develop[i]);

        AEFX_CLR_STRUCT(p_model);
        AEFX_CLR_STRUCT(p_effect);
        AEFX_CLR_STRUCT(p_mode);
        AEFX_CLR_STRUCT(p_flash);
        AEFX_CLR_STRUCT(p_optics);
        AEFX_CLR_STRUCT(p_g_amt);
        AEFX_CLR_STRUCT(p_g_size);
        AEFX_CLR_STRUCT(p_g_clump);
        AEFX_CLR_STRUCT(p_c_grain);
        AEFX_CLR_STRUCT(p_g_chroma);
        AEFX_CLR_STRUCT(p_m_col);
        AEFX_CLR_STRUCT(p_g_cont);
        AEFX_CLR_STRUCT(p_g_dens);
        AEFX_CLR_STRUCT(p_t_speed);
        AEFX_CLR_STRUCT(p_seed);
        AEFX_CLR_STRUCT(p_blend);

        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_CAMERA_MODEL, in_data->current_time, in_data->time_step, in_data->time_scale, &p_model));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_NATIVE_EFFECT, in_data->current_time, in_data->time_step, in_data->time_scale, &p_effect));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_CAMERA_MODE, in_data->current_time, in_data->time_step, in_data->time_scale, &p_mode));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_FLASH_MODE, in_data->current_time, in_data->time_step, in_data->time_scale, &p_flash));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_LENS_OPTICS, in_data->current_time, in_data->time_step, in_data->time_scale, &p_optics));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_GRAIN_AMOUNT, in_data->current_time, in_data->time_step, in_data->time_scale, &p_g_amt));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_GRAIN_SIZE, in_data->current_time, in_data->time_step, in_data->time_scale, &p_g_size));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_GRAIN_CLUMPING, in_data->current_time, in_data->time_step, in_data->time_scale, &p_g_clump));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_COLORED_GRAIN, in_data->current_time, in_data->time_step, in_data->time_scale, &p_c_grain));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_CHROMA_GRAIN, in_data->current_time, in_data->time_step, in_data->time_scale, &p_g_chroma));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_MANUAL_COLOR, in_data->current_time, in_data->time_step, in_data->time_scale, &p_m_col));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_GRAIN_CONTRAST, in_data->current_time, in_data->time_step, in_data->time_scale, &p_g_cont));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_GRAIN_DENSITY, in_data->current_time, in_data->time_step, in_data->time_scale, &p_g_dens));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_TEMPORAL_SPEED, in_data->current_time, in_data->time_step, in_data->time_scale, &p_t_speed));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_RANDOM_SEED, in_data->current_time, in_data->time_step, in_data->time_scale, &p_seed));
        ERR(PF_CHECKOUT_PARAM(in_data, PARAM_BLEND_ORIGINAL, in_data->current_time, in_data->time_step, in_data->time_scale, &p_blend));
        for (int i = 0; i < 13; ++i) ERR(PF_CHECKOUT_PARAM(in_data, develop_ids[i], in_data->current_time, in_data->time_step, in_data->time_scale, &p_develop[i]));
        const int extra_ids[38] = {PARAM_HUE_RED, PARAM_SAT_RED, PARAM_LUM_RED, PARAM_HUE_ORANGE, PARAM_SAT_ORANGE, PARAM_LUM_ORANGE, PARAM_HUE_YELLOW, PARAM_SAT_YELLOW, PARAM_LUM_YELLOW, PARAM_HUE_GREEN, PARAM_SAT_GREEN, PARAM_LUM_GREEN, PARAM_HUE_AQUA, PARAM_SAT_AQUA, PARAM_LUM_AQUA, PARAM_HUE_BLUE, PARAM_SAT_BLUE, PARAM_LUM_BLUE, PARAM_HUE_PURPLE, PARAM_SAT_PURPLE, PARAM_LUM_PURPLE, PARAM_HUE_MAGENTA, PARAM_SAT_MAGENTA, PARAM_LUM_MAGENTA, PARAM_CURVE_RGB, PARAM_CURVE_RED, PARAM_CURVE_GREEN, PARAM_CURVE_BLUE, PARAM_SHADOW_HUE, PARAM_SHADOW_SAT, PARAM_MID_HUE, PARAM_MID_SAT, PARAM_HIGHLIGHT_HUE, PARAM_HIGHLIGHT_SAT, PARAM_SHARPENING, PARAM_SHARPEN_RADIUS, PARAM_NOISE_REDUCTION, PARAM_COLOR_NOISE_REDUCTION};
        PF_ParamDef p_extra[38]; for (int i=0;i<38;++i) AEFX_CLR_STRUCT(p_extra[i]);
        for (int i=0;i<38;++i) ERR(PF_CHECKOUT_PARAM(in_data, extra_ids[i], in_data->current_time, in_data->time_step, in_data->time_scale, &p_extra[i]));

        if (!err) {
            RenderPayload payload{};
            payload.bypass         = false;
            payload.profile_index  = std::max(0, static_cast<int>(p_model.u.pd.value) - 1);
            payload.effect_choice  = std::max(0, static_cast<int>(p_effect.u.pd.value) - 1);
            payload.camera_mode    = std::max(0, static_cast<int>(p_mode.u.pd.value) - 1);
            payload.flash_enabled  = (p_flash.u.pd.value > 1);
            payload.optics_amount  = (p_optics.u.bd.value != 0) ? 1.0f : 0.0f;
            payload.grain_amount   = std::max(0.0f, static_cast<float>(p_g_amt.u.fs_d.value) * 0.01f);
            payload.grain_size     = static_cast<float>(p_g_size.u.fs_d.value);
            payload.grain_clumping = static_cast<float>(p_g_clump.u.fs_d.value) * 0.01f;
            payload.colored_grain  = (p_c_grain.u.bd.value != 0);
            payload.chroma_grain   = payload.colored_grain
                ? std::max(0.0f, static_cast<float>(p_g_chroma.u.fs_d.value) * 0.01f)
                : 0.0f;
            payload.grain_contrast = static_cast<float>(p_g_cont.u.fs_d.value) * 0.01f;
            payload.grain_density  = static_cast<float>(p_g_dens.u.fs_d.value) * 0.01f;
            payload.temporal_speed = static_cast<float>(p_t_speed.u.fs_d.value);
            payload.manual_r       = static_cast<float>(p_m_col.u.cd.value.red) * (1.0f / 255.0f);
            payload.manual_g       = static_cast<float>(p_m_col.u.cd.value.green) * (1.0f / 255.0f);
            payload.manual_b       = static_cast<float>(p_m_col.u.cd.value.blue) * (1.0f / 255.0f);
            payload.random_seed    = static_cast<int>(p_seed.u.sd.value);
            payload.blend_original = static_cast<float>(p_blend.u.fs_d.value) * 0.01f;
            payload.exposure = static_cast<float>(p_develop[0].u.fs_d.value);
            payload.contrast_adjust = static_cast<float>(p_develop[1].u.fs_d.value) * 0.01f;
            payload.highlights = static_cast<float>(p_develop[2].u.fs_d.value) * 0.0025f;
            payload.shadows = static_cast<float>(p_develop[3].u.fs_d.value) * 0.0025f;
            payload.whites = static_cast<float>(p_develop[4].u.fs_d.value) * 0.0025f;
            payload.blacks = static_cast<float>(p_develop[5].u.fs_d.value) * 0.0025f;
            payload.temperature = static_cast<float>(p_develop[6].u.fs_d.value) * 0.01f;
            payload.tint = static_cast<float>(p_develop[7].u.fs_d.value) * 0.01f;
            payload.vibrance = static_cast<float>(p_develop[8].u.fs_d.value) * 0.01f;
            payload.saturation_adjust = static_cast<float>(p_develop[9].u.fs_d.value) * 0.01f;
            payload.clarity = static_cast<float>(p_develop[10].u.fs_d.value) * 0.01f;
            payload.dehaze = static_cast<float>(p_develop[11].u.fs_d.value) * 0.01f;
            payload.vignette = static_cast<float>(p_develop[12].u.fs_d.value) * 0.01f;
            payload.hsl_hue[0] = static_cast<float>(p_extra[0].u.fs_d.value); payload.hsl_sat[0] = static_cast<float>(p_extra[1].u.fs_d.value); payload.hsl_lum[0] = static_cast<float>(p_extra[2].u.fs_d.value);
            payload.hsl_hue[1] = static_cast<float>(p_extra[3].u.fs_d.value); payload.hsl_sat[1] = static_cast<float>(p_extra[4].u.fs_d.value); payload.hsl_lum[1] = static_cast<float>(p_extra[5].u.fs_d.value);
            payload.hsl_hue[2] = static_cast<float>(p_extra[6].u.fs_d.value); payload.hsl_sat[2] = static_cast<float>(p_extra[7].u.fs_d.value); payload.hsl_lum[2] = static_cast<float>(p_extra[8].u.fs_d.value);
            payload.hsl_hue[3] = static_cast<float>(p_extra[9].u.fs_d.value); payload.hsl_sat[3] = static_cast<float>(p_extra[10].u.fs_d.value); payload.hsl_lum[3] = static_cast<float>(p_extra[11].u.fs_d.value);
            payload.hsl_hue[4] = static_cast<float>(p_extra[12].u.fs_d.value); payload.hsl_sat[4] = static_cast<float>(p_extra[13].u.fs_d.value); payload.hsl_lum[4] = static_cast<float>(p_extra[14].u.fs_d.value);
            payload.hsl_hue[5] = static_cast<float>(p_extra[15].u.fs_d.value); payload.hsl_sat[5] = static_cast<float>(p_extra[16].u.fs_d.value); payload.hsl_lum[5] = static_cast<float>(p_extra[17].u.fs_d.value);
            payload.hsl_hue[6] = static_cast<float>(p_extra[18].u.fs_d.value); payload.hsl_sat[6] = static_cast<float>(p_extra[19].u.fs_d.value); payload.hsl_lum[6] = static_cast<float>(p_extra[20].u.fs_d.value);
            payload.hsl_hue[7] = static_cast<float>(p_extra[21].u.fs_d.value); payload.hsl_sat[7] = static_cast<float>(p_extra[22].u.fs_d.value); payload.hsl_lum[7] = static_cast<float>(p_extra[23].u.fs_d.value);
            payload.curve_rgb = static_cast<float>(p_extra[24].u.fs_d.value) * 0.01f;
            payload.curve_r = static_cast<float>(p_extra[25].u.fs_d.value) * 0.01f;
            payload.curve_g = static_cast<float>(p_extra[26].u.fs_d.value) * 0.01f;
            payload.curve_b = static_cast<float>(p_extra[27].u.fs_d.value) * 0.01f;
            payload.shadow_hue = static_cast<float>(p_extra[28].u.fs_d.value);
            payload.shadow_sat = static_cast<float>(p_extra[29].u.fs_d.value);
            payload.mid_hue = static_cast<float>(p_extra[30].u.fs_d.value);
            payload.mid_sat = static_cast<float>(p_extra[31].u.fs_d.value);
            payload.highlight_hue = static_cast<float>(p_extra[32].u.fs_d.value);
            payload.highlight_sat = static_cast<float>(p_extra[33].u.fs_d.value);
            payload.sharpen = static_cast<float>(p_extra[34].u.fs_d.value) * 0.01f;
            payload.sharpen_radius = static_cast<float>(p_extra[35].u.fs_d.value);
            payload.noise_reduction = static_cast<float>(p_extra[36].u.fs_d.value) * 0.01f;
            payload.color_noise_reduction = static_cast<float>(p_extra[37].u.fs_d.value) * 0.01f;
            payload.time_sec       = (in_data->time_scale > 0)
                ? (static_cast<float>(in_data->current_time) / static_cast<float>(in_data->time_scale))
                : 0.0f;

            const CameraProfile& profile = CameraProfileManager::GetProfile(payload.profile_index);

            PF_PixelFormat format = PF_PixelFormat_INVALID;
            AEFX_SuiteScoper<PF_WorldSuite2> wsP(in_data, kPFWorldSuite, kPFWorldSuiteVersion2, out_data);
            if (wsP.get()) {
                wsP->PF_GetPixelFormat(output_worldP, &format);
            }

            if (format == PF_PixelFormat_ARGB128) {
                RenderPixelFloat(input_worldP, output_worldP, payload, profile);
            } else if (format == PF_PixelFormat_ARGB64) {
                RenderPixel16(input_worldP, output_worldP, payload, profile);
            } else {
                RenderPixel8(input_worldP, output_worldP, payload, profile);
            }
        }

        ERR2(PF_CHECKIN_PARAM(in_data, &p_model));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_effect));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_mode));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_flash));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_optics));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_g_amt));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_g_size));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_g_clump));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_c_grain));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_g_chroma));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_m_col));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_g_cont));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_g_dens));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_t_speed));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_seed));
        ERR2(PF_CHECKIN_PARAM(in_data, &p_blend));
        for (int i = 0; i < 13; ++i) ERR2(PF_CHECKIN_PARAM(in_data, &p_develop[i]));
    }

    ERR2(extra->cb->checkin_layer_pixels(in_data->effect_ref, PARAM_INPUT));
    return err;
}

static PF_Err LegacyRender(
    PF_InData       *in_data,
    PF_OutData      *out_data,
    PF_ParamDef     *params[],
    PF_LayerDef     *output)
{
    if (!params || !output || !params[PARAM_INPUT]) {
        return PF_Err_BAD_CALLBACK_PARAM;
    }

    const PF_LayerDef* input_world = &params[PARAM_INPUT]->u.ld;
    if (!input_world->data || !output->data || output->width <= 0 || output->height <= 0) {
        return PF_Err_NONE;
    }

    RenderPayload payload{};
    payload.bypass         = false;
    payload.profile_index  = std::max(0, static_cast<int>(params[PARAM_CAMERA_MODEL]->u.pd.value) - 1);
    payload.effect_choice  = std::max(0, static_cast<int>(params[PARAM_NATIVE_EFFECT]->u.pd.value) - 1);
    payload.camera_mode    = std::max(0, static_cast<int>(params[PARAM_CAMERA_MODE]->u.pd.value) - 1);
    payload.flash_enabled  = (params[PARAM_FLASH_MODE]->u.pd.value > 1);
    payload.optics_amount  = (params[PARAM_LENS_OPTICS]->u.bd.value != 0) ? 1.0f : 0.0f;
    payload.grain_amount   = std::max(0.0f, static_cast<float>(params[PARAM_GRAIN_AMOUNT]->u.fs_d.value) * 0.01f);
    payload.grain_size     = static_cast<float>(params[PARAM_GRAIN_SIZE]->u.fs_d.value);
    payload.grain_clumping = static_cast<float>(params[PARAM_GRAIN_CLUMPING]->u.fs_d.value) * 0.01f;
    payload.colored_grain  = (params[PARAM_COLORED_GRAIN]->u.bd.value != 0);
    payload.chroma_grain   = payload.colored_grain
        ? std::max(0.0f, static_cast<float>(params[PARAM_CHROMA_GRAIN]->u.fs_d.value) * 0.01f)
        : 0.0f;
    payload.grain_contrast = static_cast<float>(params[PARAM_GRAIN_CONTRAST]->u.fs_d.value) * 0.01f;
    payload.grain_density  = static_cast<float>(params[PARAM_GRAIN_DENSITY]->u.fs_d.value) * 0.01f;
    payload.temporal_speed = static_cast<float>(params[PARAM_TEMPORAL_SPEED]->u.fs_d.value);
    payload.manual_r       = static_cast<float>(params[PARAM_MANUAL_COLOR]->u.cd.value.red) * (1.0f / 255.0f);
    payload.manual_g       = static_cast<float>(params[PARAM_MANUAL_COLOR]->u.cd.value.green) * (1.0f / 255.0f);
    payload.manual_b       = static_cast<float>(params[PARAM_MANUAL_COLOR]->u.cd.value.blue) * (1.0f / 255.0f);
    payload.random_seed    = static_cast<int>(params[PARAM_RANDOM_SEED]->u.sd.value);
    payload.blend_original = static_cast<float>(params[PARAM_BLEND_ORIGINAL]->u.fs_d.value) * 0.01f;
    payload.exposure = static_cast<float>(params[PARAM_EXPOSURE]->u.fs_d.value);
    payload.contrast_adjust = static_cast<float>(params[PARAM_CONTRAST]->u.fs_d.value) * 0.01f;
    payload.highlights = static_cast<float>(params[PARAM_HIGHLIGHTS]->u.fs_d.value) * 0.0025f;
    payload.shadows = static_cast<float>(params[PARAM_SHADOWS]->u.fs_d.value) * 0.0025f;
    payload.whites = static_cast<float>(params[PARAM_WHITES]->u.fs_d.value) * 0.0025f;
    payload.blacks = static_cast<float>(params[PARAM_BLACKS]->u.fs_d.value) * 0.0025f;
    payload.temperature = static_cast<float>(params[PARAM_TEMPERATURE]->u.fs_d.value) * 0.01f;
    payload.tint = static_cast<float>(params[PARAM_TINT]->u.fs_d.value) * 0.01f;
    payload.vibrance = static_cast<float>(params[PARAM_VIBRANCE]->u.fs_d.value) * 0.01f;
    payload.saturation_adjust = static_cast<float>(params[PARAM_SATURATION]->u.fs_d.value) * 0.01f;
    payload.clarity = static_cast<float>(params[PARAM_CLARITY]->u.fs_d.value) * 0.01f;
    payload.dehaze = static_cast<float>(params[PARAM_DEHAZE]->u.fs_d.value) * 0.01f;
    payload.vignette = static_cast<float>(params[PARAM_VIGNETTE]->u.fs_d.value) * 0.01f;
    payload.time_sec       = (in_data->time_scale > 0)
        ? (static_cast<float>(in_data->current_time) / static_cast<float>(in_data->time_scale))
        : 0.0f;

    const CameraProfile& profile = CameraProfileManager::GetProfile(payload.profile_index);

    if (PF_WORLD_IS_DEEP(output)) {
        RenderPixel16(input_world, output, payload, profile);
    } else {
        RenderPixel8(input_world, output, payload, profile);
    }

    return PF_Err_NONE;
}

extern "C" DllExport
PF_Err EffectMain(
    PF_Cmd          cmd,
    PF_InData       *in_data,
    PF_OutData      *out_data,
    PF_ParamDef     *params[],
    PF_LayerDef     *output,
    void            *extra)
{
    PF_Err err = PF_Err_NONE;

    try {
        switch (cmd) {
            case PF_Cmd_ABOUT:
                err = About(in_data, out_data);
                break;

            case PF_Cmd_GLOBAL_SETUP:
                err = GlobalSetup(out_data);
                break;

            case PF_Cmd_PARAMS_SETUP:
                err = ParamsSetup(in_data, out_data);
                break;

            case PF_Cmd_UPDATE_PARAMS_UI:
                err = UpdateParamsUI(in_data, out_data, params);
                break;

            case PF_Cmd_USER_CHANGED_PARAM:
                err = UpdateParamsUI(in_data, out_data, params);
                if (out_data) {
                    out_data->out_flags |= PF_OutFlag_FORCE_RERENDER;
                }
                break;

            case PF_Cmd_RENDER:
                err = LegacyRender(in_data, out_data, params, output);
                break;

            case PF_Cmd_SMART_PRE_RENDER:
                err = PreRender(in_data, out_data, reinterpret_cast<PF_PreRenderExtra*>(extra));
                break;

            case PF_Cmd_SMART_RENDER:
                err = SmartRender(in_data, out_data, reinterpret_cast<PF_SmartRenderExtra*>(extra));
                break;

            default:
                break;
        }
    } catch (...) {
        err = PF_Err_INTERNAL_STRUCT_DAMAGED;
    }

    return err;
}
