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
    bool blend_grain_source;
    float manual_r;
    float manual_g;
    float manual_b;
    int random_seed;
    float time_sec;
    float blend_original;
    float exposure;
    float contrast_adjust;
    float highlights;
    float shadows;
    float whites;
    float blacks;
    float temperature;
    float tint;
    float vibrance;
    float saturation_adjust;
    float clarity;
    float dehaze;
    float vignette;
    float sharpen;
    float hsl_hue[8], hsl_sat[8], hsl_lum[8];
    float curve_rgb, curve_r, curve_g, curve_b;
    float shadow_hue, shadow_sat, mid_hue, mid_sat, highlight_hue, highlight_sat, grade_balance;
    float sharpen_radius, noise_reduction, color_noise_reduction;
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
        // Gentle negative-film shoulder. The former r/(r+rolloff)*(1+rolloff)
        // curve pushed midtones close to white (especially CPM35); this rational
        // shoulder compresses highlights without creating a global exposure lift.
        r = r / (1.0f + profile.highlight_rolloff * r);
        g = g / (1.0f + profile.highlight_rolloff * g);
        b = b / (1.0f + profile.highlight_rolloff * b);
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

    const float cameraContrast = 1.0f + (profile.contrast - 1.0f) * 1.22f;
    r = Clamp01((r - 0.5f) * cameraContrast + 0.5f);
    g = Clamp01((g - 0.5f) * cameraContrast + 0.5f);
    b = Clamp01((b - 0.5f) * cameraContrast + 0.5f);

    float luma = 0.299f * r + 0.587f * g + 0.114f * b;
    const float cameraSaturation = 1.0f + (profile.saturation - 1.0f) * 1.28f;
    r = Clamp01(luma + (r - luma) * cameraSaturation);
    g = Clamp01(luma + (g - luma) * cameraSaturation);
    b = Clamp01(luma + (b - luma) * cameraSaturation);

    // 5b. Camera-engine response stage.
    // These are independent response families, not UI presets: each family
    // changes the luminance shoulder/toe and channel coupling before JPEG/NR.
    // The profile coefficients above remain the reference-specific calibration.
    {
        const float y = 0.299f * r + 0.587f * g + 0.114f * b;
        float target_y = y;
        float mix = 0.0f;
        switch (profile.id) {
            case 1: // CPM35: restrained warm disposable negative
                target_y = y * (0.96f + 0.06f * (1.0f - y));
                mix = 0.36f; r *= 1.045f; g *= 1.005f; b *= 0.925f; break;
            case 2: // Classic U: soft, amber compact-film rendering
                target_y = y > 0.68f ? 0.68f + 0.32f * (1.0f - std::exp(-(y - 0.68f) / 0.32f)) : y;
                mix = 0.30f; r *= 1.035f; b *= 0.945f; break;
            case 3: // IR: strong false-colour response
                target_y = 0.36f * r + 0.48f * g + 0.16f * b;
                mix = 0.58f; { const float old_r = r; r = 0.62f * r + 0.38f * (1.0f - b); b = 0.52f * b + 0.48f * old_r; g *= 1.04f; } break;
            case 4: // FQS: punchy direct-flash response with warm highlights
                target_y = (y - 0.5f) * 1.24f + 0.5f;
                mix = 0.30f; r *= 1.055f; b *= 0.91f; break;
            case 5: // CCD R: saturated early CCD with warm clipping
                target_y = y > 0.78f ? 0.78f + (y - 0.78f) * 0.34f : y;
                mix = 0.42f; r *= 1.055f; g *= 1.015f; b *= 0.91f; break;
            case 6: // FXN: cooler, denser shadow response
                target_y = y * (0.78f + 0.22f * y);
                mix = 0.40f; r *= 0.94f; b *= 1.075f; break;
            case 7: // DHalf: warm half-frame film with compressed midtones
                target_y = y / (0.92f + 0.16f * y);
                mix = 0.38f; r *= 1.04f; b *= 0.94f; break;
            default: {
                // Twelve pocket-CCD references get distinct response knees.
                const int k = profile.id - 8;
                const float knee = 0.62f + 0.025f * static_cast<float>(k % 7);
                const float shoulder = 0.24f + 0.035f * static_cast<float>(k % 5);
                if (y > knee) {
                    const float d = y - knee;
                    target_y = knee + d / (1.0f + d / shoulder);
                } else {
                    const float toe = 0.015f * static_cast<float>(k % 4);
                    target_y = y + toe * (1.0f - y) * (1.0f - y);
                }
                mix = 0.18f + 0.035f * static_cast<float>(k % 6);
                // Distinct but bounded white-balance signatures for each pocket camera.
                switch (k % 6) {
                    case 0: r *= 1.045f; b *= 0.925f; break; // warm
                    case 1: r *= 0.935f; b *= 1.075f; break; // cool/cyan
                    case 2: g *= 1.055f; r *= 0.965f; break; // green compact CCD
                    case 3: r *= 1.065f; g *= 0.985f; b *= 0.925f; break; // red halation
                    case 4: r *= 0.965f; g *= 1.015f; b *= 1.055f; break; // muted cool
                    default: r *= 1.035f; g *= 1.025f; b *= 0.955f; break; // daylight
                }
                break;
            }
        }
        const float delta_y = target_y - y;
        r = Clamp01(r + delta_y * mix);
        g = Clamp01(g + delta_y * mix);
        b = Clamp01(b + delta_y * mix);
    }

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

    // 7. Native Camera Effect. Popup values are 1-based; map each UI choice
    // to a distinct, real processing operation instead of enum values that
    // previously landed on unrelated/near-neutral effects.
    CameraEffectType eff_type = EFFECT_STANDARD;
    switch (payload.effect_choice) {
        case 1: // Soft Highlight Roll-Off
            r = Clamp01(r / (1.0f + 0.22f * r)); g = Clamp01(g / (1.0f + 0.22f * g)); b = Clamp01(b / (1.0f + 0.22f * b)); break;
        case 2: // Hard Direct Flash
            r = Clamp01((r - 0.5f) * 1.28f + 0.54f); g = Clamp01((g - 0.5f) * 1.28f + 0.52f); b = Clamp01((b - 0.5f) * 1.28f + 0.49f); break;
        case 3: // Warm Film Bias
            r = Clamp01(r * 1.10f + 0.012f); g = Clamp01(g * 1.015f); b = Clamp01(b * 0.88f); break;
        case 4: // Cool CCD Bias
            r = Clamp01(r * 0.91f); g = Clamp01(g * 1.015f); b = Clamp01(b * 1.12f + 0.008f); break;
        case 5: { // Low-Saturation Matte
            float yy = 0.299f*r + 0.587f*g + 0.114f*b;
            r = Clamp01(yy + (r-yy)*0.58f + 0.045f); g = Clamp01(yy + (g-yy)*0.58f + 0.045f); b = Clamp01(yy + (b-yy)*0.58f + 0.045f); break;
        }
        case 6: { // High-Saturation Compact
            float yy = 0.299f*r + 0.587f*g + 0.114f*b;
            r = Clamp01(yy + (r-yy)*1.42f); g = Clamp01(yy + (g-yy)*1.42f); b = Clamp01(yy + (b-yy)*1.42f); break;
        }
        case 7: eff_type = EFFECT_MONOCHROME; break;
        default: break;
    }
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
        ? (profile.luma_noise * (payload.grain_amount * 5.0f) * mode_grain_boost * shadow_weight)
        : 0.0f;

    // Chroma Grain is strictly active only when Colored Grain switch is ON and Chroma Grain > 0.
    // When Chroma Grain == 0 (or switch is OFF), chroma_amp is strictly 0.0f (zero chromatic grain contribution).
    const bool use_camera_chroma_grain = payload.colored_grain && (payload.chroma_grain > 0.0f);
    const float chroma_amp = use_camera_chroma_grain
        ? (profile.chroma_noise * (payload.chroma_grain * 3.5f) * mode_grain_boost * shadow_weight)
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

                float raw_cr = g_cr * cr_weight;
                float raw_cg = g_cg * cg_weight;
                float raw_cb = g_cb * cb_weight;
                if (payload.blend_grain_source) {
                    // Source-colour-aware chroma: bias grain channel energy toward
                    // colours actually present in the source frame, without tinting
                    // the image globally.
                    const float srcLum = 0.299f*in_r + 0.587f*in_g + 0.114f*in_b;
                    const float sr = std::max(0.0f, in_r-srcLum) * 1.8f;
                    const float sg = std::max(0.0f, in_g-srcLum) * 1.8f;
                    const float sb = std::max(0.0f, in_b-srcLum) * 1.8f;
                    raw_cr *= 0.65f + sr;
                    raw_cg *= 0.65f + sg;
                    raw_cb *= 0.65f + sb;
                }

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

    // 8b. Lightroom-style global develop controls. Values are normalized from UI
    // ranges and deliberately run after the camera response so users can grade it.
    {
        const float exposureGain = std::pow(2.0f, payload.exposure);
        r *= exposureGain; g *= exposureGain; b *= exposureGain;
        const float lum = 0.2126f*r + 0.7152f*g + 0.0722f*b;
        const float hi = std::max(0.0f, (lum - 0.45f) / 0.55f);
        const float sh = std::max(0.0f, (0.55f - lum) / 0.55f);
        const float tonal = payload.highlights * hi * hi + payload.shadows * sh * sh;
        r += tonal; g += tonal; b += tonal;
        r += payload.whites * std::max(0.0f, (lum - 0.72f) / 0.28f);
        g += payload.whites * std::max(0.0f, (lum - 0.72f) / 0.28f);
        b += payload.whites * std::max(0.0f, (lum - 0.72f) / 0.28f);
        r += payload.blacks * std::max(0.0f, (0.28f - lum) / 0.28f);
        g += payload.blacks * std::max(0.0f, (0.28f - lum) / 0.28f);
        b += payload.blacks * std::max(0.0f, (0.28f - lum) / 0.28f);
        const float contrastFactor = std::max(0.05f, 1.0f + payload.contrast_adjust);
        r = (r - 0.5f) * contrastFactor + 0.5f;
        g = (g - 0.5f) * contrastFactor + 0.5f;
        b = (b - 0.5f) * contrastFactor + 0.5f;
        // Temperature and tint are white-balance offsets, not whole-image overlays.
        r += payload.temperature * 0.12f + payload.tint * 0.035f;
        g -= payload.tint * 0.045f;
        b -= payload.temperature * 0.12f - payload.tint * 0.035f;
        const float yLum = 0.2126f*r + 0.7152f*g + 0.0722f*b;
        const float sat = std::max(0.0f, 1.0f + payload.saturation_adjust + payload.vibrance * (1.0f - std::min(1.0f, std::max(0.0f, std::max(r, std::max(g,b)) - std::min(r, std::min(g,b))))));
        r = yLum + (r-yLum)*sat; g = yLum + (g-yLum)*sat; b = yLum + (b-yLum)*sat;
        // Clarity is a midtone-local contrast approximation; dehaze adds a restrained
        // black-point/contrast correction without requiring neighbouring pixels.
        const float midMask = std::max(0.0f, 1.0f - std::fabs(yLum - 0.5f) * 1.8f);
        const float clarityFactor = 1.0f + payload.clarity * 0.45f * midMask;
        r = (r - 0.5f) * clarityFactor + 0.5f;
        g = (g - 0.5f) * clarityFactor + 0.5f;
        b = (b - 0.5f) * clarityFactor + 0.5f;
        const float haze = payload.dehaze * 0.08f;
        r = (r - 0.5f) * (1.0f + haze) + 0.5f - haze * 0.12f;
        g = (g - 0.5f) * (1.0f + haze) + 0.5f - haze * 0.12f;
        b = (b - 0.5f) * (1.0f + haze) + 0.5f - haze * 0.12f;
        if (payload.vignette > 0.0f) {
            const float radial = std::max(0.0f, 1.0f - (dx*dx + dy*dy) * 2.8f * payload.vignette);
            r *= radial; g *= radial; b *= radial;
        }
        // Detail controls. Without a neighborhood buffer this native pixel path uses
        // a conservative per-pixel chroma-noise suppressor and tonal microcontrast.
        if (payload.color_noise_reduction > 0.0f) {
            const float yy = 0.2126f*r + 0.7152f*g + 0.0722f*b;
            const float k = Clamp01(payload.color_noise_reduction);
            r = r*(1.0f-k) + yy*k; g = g*(1.0f-k) + yy*k; b = b*(1.0f-k) + yy*k;
        }
        if (payload.noise_reduction > 0.0f) {
            // A per-pixel edge-safe approximation: reduce chroma speckle first,
            // then gently compress very small channel differences without washing
            // the whole image to monochrome.
            const float yy = 0.2126f*r + 0.7152f*g + 0.0722f*b;
            const float k = Clamp01(payload.noise_reduction) * 0.22f;
            const float dr = r-yy, dg = g-yy, db = b-yy;
            r = yy + dr*(1.0f-k); g = yy + dg*(1.0f-k); b = yy + db*(1.0f-k);
        }
        if (payload.sharpen > 0.0f) {
            const float yy = 0.2126f*r + 0.7152f*g + 0.0722f*b;
            const float radiusScale = 0.65f + Clamp01((payload.sharpen_radius-1.0f)/4.0f)*0.9f;
            const float k = payload.sharpen * 0.42f * radiusScale;
            r = yy + (r-yy)*(1.0f+k); g = yy + (g-yy)*(1.0f+k); b = yy + (b-yy)*(1.0f+k);
        }
        // HSL mixer: hue-family masks are smooth so adjacent colors transition naturally.
        {
            const float mx = std::max(r, std::max(g, b)), mn = std::min(r, std::min(g, b));
            const float delta = mx - mn;
            float hue = 0.0f;
            if (delta > 1.0e-5f) {
                if (mx == r) hue = 60.0f * std::fmod((g - b) / delta + 6.0f, 6.0f);
                else if (mx == g) hue = 60.0f * ((b-r)/delta + 2.0f);
                else hue = 60.0f * ((r-g)/delta + 4.0f);
            }
            const float centers[8] = {0,30,60,120,180,220,270,320};
            float dh=0, ds=0, dl=0, total=0;
            for (int i=0;i<8;++i) {
                float d=std::fabs(hue-centers[i]); if(d>180) d=360-d;
                float w=Clamp01(1.0f-d/42.0f); w=w*w*(3.0f-2.0f*w);
                dh+=payload.hsl_hue[i]*w; ds+=payload.hsl_sat[i]*w; dl+=payload.hsl_lum[i]*w; total+=w;
            }
            if(total>1.0e-5f) {
                const float angle=(dh/total)*0.01745329252f;
                const float Y=0.299f*r+0.587f*g+0.114f*b;
                const float I=0.596f*r-0.274f*g-0.322f*b, Q=0.211f*r-0.523f*g+0.312f*b;
                const float ii=I*std::cos(angle)-Q*std::sin(angle), qq=I*std::sin(angle)+Q*std::cos(angle);
                r=Y+0.956f*ii+0.621f*qq; g=Y-0.272f*ii-0.647f*qq; b=Y-1.106f*ii+1.703f*qq;
                const float yy=0.2126f*r+0.7152f*g+0.0722f*b;
                const float sm=std::max(0.0f,1.0f+ds/100.0f);
                r=yy+(r-yy)*sm+dl/100.0f; g=yy+(g-yy)*sm+dl/100.0f; b=yy+(b-yy)*sm+dl/100.0f;
            }
        }
        // Parametric tone-curve approximation. The master control bends the
        // tonal range (rather than adding a flat lift); RGB controls bend each
        // channel independently. A custom AE-style spline editor would require
        // a dedicated custom UI/arbitrary-data parameter.
        {
            auto applyCurve = [](float x, float master, float channel) {
                x = Clamp01(x);
                const float m = std::max(-1.0f, std::min(1.0f, master));
                const float c = std::max(-1.0f, std::min(1.0f, channel));
                const float bend = x * (1.0f - x);
                x += m * bend * 2.25f;
                x += c * bend * 2.0f;
                return Clamp01(x);
            };
            r = applyCurve(r, payload.curve_rgb, payload.curve_r);
            g = applyCurve(g, payload.curve_rgb, payload.curve_g);
            b = applyCurve(b, payload.curve_rgb, payload.curve_b);
        }
        // Split toning / three-way color grading, weighted by luminance region.
        {
            const float lum=0.2126f*r+0.7152f*g+0.0722f*b;
            const float sw=Clamp01((0.55f-lum)/0.55f), hw=Clamp01((lum-0.45f)/0.55f);
            const float mw=Clamp01(1.0f-std::fabs(lum-0.5f)*2.0f);
            // Balance shifts the split-tone weighting between shadows and highlights.
            const float bal = payload.grade_balance;
            const float shadowWeight = Clamp01(sw + bal * 0.35f * mw);
            const float highlightWeight = Clamp01(hw - bal * 0.35f * mw);
            const float gh=(payload.shadow_hue*shadowWeight+payload.mid_hue*mw+payload.highlight_hue*highlightWeight)*0.01745329252f;
            const float gs=(payload.shadow_sat*shadowWeight+payload.mid_sat*mw+payload.highlight_sat*highlightWeight)*0.006f;
            const float Y=0.2126f*r+0.7152f*g+0.0722f*b, U=(b-Y)*0.565f, V=(r-Y)*0.713f;
            const float u2=U*std::cos(gh)-V*std::sin(gh), v2=U*std::sin(gh)+V*std::cos(gh);
            r=Y+v2/0.713f; b=Y+u2/0.565f; g=(Y-0.2126f*r-0.0722f*b)/0.7152f;
            r+=gs*(r-Y); g+=gs*(g-Y); b+=gs*(b-Y);
        }
        r = Clamp01(r); g = Clamp01(g); b = Clamp01(b);
    }

    // 9. Blend with Original
    if (std::fabs(payload.blend_original) > 0.0001f) {
        // Positive values blend back toward the untreated source. Negative values
        // extrapolate away from the source to intensify the camera rendering.
        const float mix = std::max(-1.0f, std::min(1.0f, payload.blend_original));
        if (mix >= 0.0f) {
            r = r * (1.0f - mix) + in_r * mix;
            g = g * (1.0f - mix) + in_g * mix;
            b = b * (1.0f - mix) + in_b * mix;
        } else {
            const float boost = -mix;
            r += (r - in_r) * boost;
            g += (g - in_g) * boost;
            b += (b - in_b) * boost;
        }
        r = Clamp01(r); g = Clamp01(g); b = Clamp01(b);
    }

    out_r = r;
    out_g = g;
    out_b = b;
}

} // namespace DigitalCamColor

#endif // COLOR_ENGINE_H
