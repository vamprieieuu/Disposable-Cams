#ifndef COLOR_ENGINE_H
#define COLOR_ENGINE_H

#include "CameraProfiles.h"
#include "NoiseEngine.h"
#include <algorithm>
#include <cmath>

struct RenderPayload {
    bool bypass;
    bool flash_enabled;
    int profile_index;
    int effect_choice;
    int camera_mode;
    float optics_amount;
    float grain_amount;
    float grain_size;
    float grain_clumping;
    float chroma_grain;
    float grain_contrast;
    float grain_density;
    float temporal_speed;
    bool colored_grain;
    float manual_r;
    float manual_g;
    float manual_b;
    int random_seed;
    float time_sec;
    float blend_original;
};

namespace DigitalCamColor {

inline float Clamp01(float v) {
    return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
}

inline void ApplyCameraEffect(
    CameraEffectType type,
    float strength,
    float u,
    float v,
    float luma,
    float& r,
    float& g,
    float& b)
{
    switch (type) {
        case EFFECT_NONE:
        case EFFECT_STANDARD:
            break;

        case EFFECT_VIVID:
        case EFFECT_POP_COLOR: {
            float boost = (type == EFFECT_POP_COLOR) ? 1.45f : (1.22f * strength);
            r = Clamp01(luma + (r - luma) * boost);
            g = Clamp01(luma + (g - luma) * boost);
            b = Clamp01(luma + (b - luma) * boost);
            break;
        }

        case EFFECT_NATURAL:
        case EFFECT_NEUTRAL:
        case EFFECT_FAITHFUL: {
            float atten = (type == EFFECT_NEUTRAL) ? 0.85f : 0.92f;
            r = Clamp01(luma + (r - luma) * atten);
            g = Clamp01(luma + (g - luma) * atten);
            b = Clamp01(luma + (b - luma) * atten);
            break;
        }

        case EFFECT_PORTRAIT:
        case EFFECT_BEAUTY_SOFT: {
            r = Clamp01(r * 1.04f + 0.015f);
            g = Clamp01(g * 1.01f + 0.01f);
            b = Clamp01(b * 0.97f);
            break;
        }

        case EFFECT_LANDSCAPE: {
            r = Clamp01(r * 0.98f);
            g = Clamp01(g * 1.08f);
            b = Clamp01(b * 1.10f);
            break;
        }

        case EFFECT_SUNSET:
        case EFFECT_WIZEN_WARM: {
            r = Clamp01(r * 1.18f + 0.02f);
            g = Clamp01(g * 1.02f);
            b = Clamp01(b * 0.82f);
            break;
        }

        case EFFECT_MONOCHROME:
        case EFFECT_BW:
        case EFFECT_FILM_HP5:
        case EFFECT_FILM_TRIX:
        case EFFECT_WIZEN_BW: {
            float bw = luma;
            if (type == EFFECT_FILM_TRIX || type == EFFECT_WIZEN_BW) {
                bw = Clamp01((bw - 0.5f) * 1.22f + 0.5f);
            }
            r = g = b = bw;
            break;
        }

        case EFFECT_SEPIA:
        case EFFECT_SALON: {
            r = Clamp01(luma * 1.14f + 0.04f);
            g = Clamp01(luma * 0.96f + 0.01f);
            b = Clamp01(luma * 0.74f);
            break;
        }

        case EFFECT_NEGATIVE:
        case EFFECT_NEG_ART: {
            r = Clamp01(1.0f - r);
            g = Clamp01(1.0f - g);
            b = Clamp01(1.0f - b);
            break;
        }

        case EFFECT_SOLARIZE: {
            r = (r > 0.5f) ? (1.0f - r) * 2.0f : r * 2.0f;
            g = (g > 0.5f) ? (1.0f - g) * 2.0f : g * 2.0f;
            b = (b > 0.5f) ? (1.0f - b) * 2.0f : b * 2.0f;
            r = Clamp01(r);
            g = Clamp01(g);
            b = Clamp01(b);
            break;
        }

        case EFFECT_PASTEL: {
            r = Clamp01(std::floor(r * 6.0f) / 6.0f * 0.85f + 0.15f);
            g = Clamp01(std::floor(g * 6.0f) / 6.0f * 0.85f + 0.15f);
            b = Clamp01(std::floor(b * 6.0f) / 6.0f * 0.85f + 0.15f);
            break;
        }

        case EFFECT_MOSAIC: {
            float quant = 12.0f;
            r = Clamp01(std::floor(r * quant + 0.5f) / quant);
            g = Clamp01(std::floor(g * quant + 0.5f) / quant);
            b = Clamp01(std::floor(b * quant + 0.5f) / quant);
            break;
        }

        case EFFECT_PIN_HOLE:
        case EFFECT_VIGNETTING:
        case EFFECT_TOY_CAMERA: {
            float dx = u - 0.5f;
            float dy = v - 0.5f;
            float dist2 = dx * dx + dy * dy;
            float vig = Clamp01(1.0f - dist2 * 2.6f);
            r = Clamp01(r * vig * 1.05f);
            g = Clamp01(g * vig * 1.02f);
            b = Clamp01(b * vig * 0.90f);
            break;
        }

        case EFFECT_DREAMY: {
            r = Clamp01(r * 0.88f + 0.12f);
            g = Clamp01(g * 0.88f + 0.11f);
            b = Clamp01(b * 0.88f + 0.13f);
            break;
        }

        case EFFECT_CLEAR:
        case EFFECT_DEEP: {
            float c = (type == EFFECT_DEEP) ? 1.18f : 1.10f;
            r = Clamp01((r - 0.5f) * c + 0.5f);
            g = Clamp01((g - 0.5f) * c + 0.5f);
            b = Clamp01((b - 0.5f) * c + 0.5f);
            break;
        }

        case EFFECT_LIGHT:
        case EFFECT_WIZEN_MATTE: {
            r = Clamp01(r * 0.88f + 0.06f);
            g = Clamp01(g * 0.90f + 0.06f);
            b = Clamp01(b * 0.94f + 0.08f);
            break;
        }

        case EFFECT_FILM_PORTRA: {
            r = Clamp01(r * 1.04f + 0.01f);
            g = Clamp01(g * 1.01f + 0.01f);
            b = Clamp01(b * 0.97f + 0.01f);
            break;
        }

        case EFFECT_FILM_GOLD:
        case EFFECT_POSITIVE_FILM: {
            r = Clamp01(r * 1.10f + 0.01f);
            g = Clamp01(g * 1.03f);
            b = Clamp01(b * 0.91f);
            break;
        }

        case EFFECT_FILM_SUPERIA: {
            r = Clamp01(r * 1.02f);
            g = Clamp01(g * 1.07f + 0.01f);
            b = Clamp01(b * 1.03f);
            break;
        }

        default:
            break;
    }
}

inline void ProcessPixel(
    float in_r,
    float in_g,
    float in_b,
    float u,
    float v,
    int width,
    int height,
    const RenderPayload& payload,
    const CameraProfile& profile,
    float& out_r,
    float& out_g,
    float& out_b)
{
    if (payload.bypass) {
        out_r = in_r;
        out_g = in_g;
        out_b = in_b;
        return;
    }

    float r = in_r;
    float g = in_g;
    float b = in_b;

    float dx = u - 0.5f;
    float dy = v - 0.5f;
    float r2 = dx * dx + dy * dy;

    // 1. Lens Vignette & Chromatic Aberration optical character
    float vig_amt = payload.optics_amount * profile.vignette_strength;
    if (vig_amt > 0.0001f) {
        float vig = std::max(0.15f, 1.0f - r2 * vig_amt * 2.0f);
        r *= vig * (1.0f + profile.chromatic_aberration * r2 * 18.0f);
        g *= vig;
        b *= vig * std::max(0.2f, 1.0f - profile.chromatic_aberration * r2 * 22.0f);
    }

    // 2. Flash illumination model
    if (payload.flash_enabled && profile.has_flash) {
        float flash_fall = std::max(0.0f, 1.0f - r2 * (1.2f + profile.flash_falloff * 2.5f));
        float flash_gain = 1.0f + (profile.flash_intensity - 1.0f) * flash_fall;
        r *= flash_gain * profile.flash_wb_r;
        g *= flash_gain * profile.flash_wb_g;
        b *= flash_gain * profile.flash_wb_b;
    }

    // 3. Exposure & White Balance & Camera Color Matrix
    float exp_gain = std::pow(2.0f, profile.exposure_bias);
    if (payload.camera_mode == 7) { // High Sensitivity (High ISO)
        exp_gain *= 1.15f;
    } else if (payload.camera_mode == 2) { // Night Portrait
        exp_gain *= 0.92f;
    }

    float wr = r * exp_gain * profile.wb_r;
    float wg = g * exp_gain * profile.wb_g;
    float wb = b * exp_gain * profile.wb_b;

    float mr = profile.color_matrix[0] * wr + profile.color_matrix[1] * wg + profile.color_matrix[2] * wb;
    float mg = profile.color_matrix[3] * wr + profile.color_matrix[4] * wg + profile.color_matrix[5] * wb;
    float mb = profile.color_matrix[6] * wr + profile.color_matrix[7] * wg + profile.color_matrix[8] * wb;

    r = Clamp01(mr);
    g = Clamp01(mg);
    b = Clamp01(mb);

    // 4. Sensor response curve (CCD / BIONZ / DIGIC / Film / Mobile CMOS / PureView)
    float blk = profile.black_level;
    float wht = (profile.highlight_clip > blk + 0.05f) ? profile.highlight_clip : (blk + 0.05f);
    r = Clamp01((r - blk) / (wht - blk));
    g = Clamp01((g - blk) / (wht - blk));
    b = Clamp01((b - blk) / (wht - blk));

    if (profile.is_film) {
        // Photochemical film shoulder/toe response
        r = r / (r + profile.highlight_rolloff) * (1.0f + profile.highlight_rolloff);
        g = g / (g + profile.highlight_rolloff) * (1.0f + profile.highlight_rolloff);
        b = b / (b + profile.highlight_rolloff) * (1.0f + profile.highlight_rolloff);
    } else if (profile.is_pureview) {
        // PureView oversampled highlight retention
        r = std::pow(Clamp01(r), 0.96f);
        g = std::pow(Clamp01(g), 0.96f);
        b = std::pow(Clamp01(b), 0.96f);
    } else {
        float roll = profile.highlight_rolloff;
        if (roll > 0.01f) {
            float thresh = 1.0f - roll;
            if (r > thresh) r = thresh + (1.0f - thresh) * std::tanh((r - thresh) / roll);
            if (g > thresh) g = thresh + (1.0f - thresh) * std::tanh((g - thresh) / roll);
            if (b > thresh) b = thresh + (1.0f - thresh) * std::tanh((b - thresh) / roll);
        }
    }

    // 5. Contrast, Gamma, and Saturation
    float inv_gamma = 2.2f / (profile.gamma > 0.1f ? profile.gamma : 2.2f);
    r = std::pow(Clamp01(r), inv_gamma);
    g = std::pow(Clamp01(g), inv_gamma);
    b = std::pow(Clamp01(b), inv_gamma);

    r = Clamp01((r - 0.5f) * profile.contrast + 0.5f);
    g = Clamp01((g - 0.5f) * profile.contrast + 0.5f);
    b = Clamp01((b - 0.5f) * profile.contrast + 0.5f);

    float luma = 0.299f * r + 0.587f * g + 0.114f * b;
    r = Clamp01(luma + (r - luma) * profile.saturation);
    g = Clamp01(luma + (g - luma) * profile.saturation);
    b = Clamp01(luma + (b - luma) * profile.saturation);

    // 6. Camera-specific NR / JPEG quantization approximation
    if (profile.has_watercolor_nr || profile.nr_smear > 0.25f) {
        float q_steps = 48.0f * (1.0f - profile.nr_smear * 0.5f);
        if (q_steps < 16.0f) q_steps = 16.0f;
        float ql = std::floor(luma * q_steps + 0.5f) / q_steps;
        float nr_mix = profile.nr_smear * 0.35f;
        float delta = (ql - luma) * nr_mix;
        r = Clamp01(r + delta);
        g = Clamp01(g + delta);
        b = Clamp01(b + delta);
    }

    // 7. Native Camera Effect
    CameraEffectType eff_type = static_cast<CameraEffectType>(payload.effect_choice);
    float eff_strength = 1.0f;
    if (eff_type == EFFECT_NONE && !profile.effects.empty()) {
        eff_type = profile.effects[0].type;
        eff_strength = profile.effects[0].default_strength;
    }
    luma = 0.299f * r + 0.587f * g + 0.114f * b;
    ApplyCameraEffect(eff_type, eff_strength, u, v, luma, r, g, b);

    // 8. Procedural Temporal Sensor & Film Grain Engine
    float mode_grain_boost = 1.0f;
    if (payload.camera_mode == 7) { // High Sensitivity (High ISO)
        mode_grain_boost *= 1.6f;
    }
    if (eff_type == EFFECT_FILM_GRAIN || eff_type == EFFECT_GRAINY) {
        mode_grain_boost *= 1.45f;
    }

    float shadow_weight = 1.0f - 0.45f * luma;
    if (profile.is_film) {
        shadow_weight = 4.0f * luma * (1.0f - luma) * 0.7f + 0.3f;
    }

    const float luma_amp = (payload.grain_amount > 0.0f)
        ? (profile.luma_noise * (payload.grain_amount * 2.0f) * mode_grain_boost * shadow_weight)
        : 0.0f;

    // Chroma Grain is strictly active only when Colored Grain switch is ON and Chroma Grain > 0.
    // When Chroma Grain == 0 (or switch is OFF), chroma_amp is strictly 0.0f (zero chromatic grain contribution).
    const bool use_camera_chroma_grain = payload.colored_grain && (payload.chroma_grain > 0.0f);
    const float chroma_amp = use_camera_chroma_grain
        ? (profile.chroma_noise * (payload.chroma_grain * 1.6f) * mode_grain_boost * shadow_weight)
        : 0.0f;

    if (luma_amp > 0.0f || chroma_amp > 0.0f) {
        float g_luma = 0.0f, g_cr = 0.0f, g_cg = 0.0f, g_cb = 0.0f;
        DigitalCamNoise::SampleSensorAndGrainNoise(
            u, v, width, height,
            payload.time_sec,
            payload.temporal_speed,
            payload.random_seed + profile.id * 97,
            profile.grain_size * payload.grain_size,
            payload.grain_clumping,
            payload.grain_contrast,
            payload.grain_density,
            profile.sensor_type,
            use_camera_chroma_grain,
            g_luma, g_cr, g_cg, g_cb);

        if (payload.colored_grain) {
            // ON: Use camera-specific chromatic grain + neutral luminance grain.
            const float dl = g_luma * luma_amp;
            float dc_r = 0.0f;
            float dc_g = 0.0f;
            float dc_b = 0.0f;

            if (chroma_amp > 0.0f) {
                float cr_weight = 1.15f * profile.wb_r;
                float cg_weight = 0.65f * profile.wb_g;
                float cb_weight = 1.30f * profile.wb_b;

                switch (profile.sensor_type) {
                    case SENSOR_COMPACT_CCD:
                        cr_weight = 1.15f * profile.wb_r;
                        cg_weight = 0.62f * profile.wb_g;
                        cb_weight = 1.38f * profile.wb_b;
                        break;
                    case SENSOR_SONY_BIONZ:
                        cr_weight = 1.12f * profile.wb_r;
                        cg_weight = 0.72f * profile.wb_g;
                        cb_weight = 1.28f * profile.wb_b;
                        break;
                    case SENSOR_CANON_DIGIC:
                        cr_weight = 1.26f * profile.wb_r;
                        cg_weight = 0.58f * profile.wb_g;
                        cb_weight = 1.16f * profile.wb_b;
                        break;
                    case SENSOR_PHOTOCHEMICAL_FILM:
                        cr_weight = 1.06f * profile.wb_r;
                        cg_weight = 0.94f * profile.wb_g;
                        cb_weight = 1.12f * profile.wb_b;
                        break;
                    case SENSOR_EARLY_MOBILE_CMOS:
                        cr_weight = 1.32f * profile.wb_r;
                        cg_weight = 0.75f * profile.wb_g;
                        cb_weight = 1.48f * profile.wb_b;
                        break;
                    case SENSOR_PUREVIEW_OVERSAMPLED:
                        cr_weight = 0.85f * profile.wb_r;
                        cg_weight = 0.60f * profile.wb_g;
                        cb_weight = 0.92f * profile.wb_b;
                        break;
                    default:
                        break;
                }

                const float raw_cr = g_cr * cr_weight;
                const float raw_cg = g_cg * cg_weight;
                const float raw_cb = g_cb * cb_weight;

                // Orthogonalize against luminance so chromatic grain has zero luma shift and never tints the image
                const float chroma_luma = 0.299f * raw_cr + 0.587f * raw_cg + 0.114f * raw_cb;
                dc_r = (raw_cr - chroma_luma) * chroma_amp;
                dc_g = (raw_cg - chroma_luma) * chroma_amp;
                dc_b = (raw_cb - chroma_luma) * chroma_amp;
            }

            r += dl + dc_r;
            g += dl + dc_g;
            b += dl + dc_b;
        } else {
            // OFF: Disable chromatic grain and apply manual Grain Color strictly to the zero-mean grain modulation
            // so only grain particles are colored without tinting the entire image.
            const float dl = g_luma * luma_amp;
            r += dl * payload.manual_r;
            g += dl * payload.manual_g;
            b += dl * payload.manual_b;
        }
    }

    r = Clamp01(r);
    g = Clamp01(g);
    b = Clamp01(b);

    // 9. Blend with Original
    if (payload.blend_original > 0.0001f) {
        float mix_orig = Clamp01(payload.blend_original);
        r = r * (1.0f - mix_orig) + in_r * mix_orig;
        g = g * (1.0f - mix_orig) + in_g * mix_orig;
        b = b * (1.0f - mix_orig) + in_b * mix_orig;
    }

    out_r = r;
    out_g = g;
    out_b = b;
}

} // namespace DigitalCamColor

#endif // COLOR_ENGINE_H
