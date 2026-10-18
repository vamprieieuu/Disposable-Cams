#ifndef NOISE_ENGINE_H
#define NOISE_ENGINE_H

#include "CameraProfiles.h"
#include <cmath>
#include <cstdint>

namespace DigitalCamNoise {

inline uint32_t Hash32(uint32_t x) {
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    return x;
}

inline float HashFloat(uint32_t x) {
    return static_cast<float>(Hash32(x) & 0x00FFFFFFU) * (1.0f / 16777215.0f);
}

inline float ValueNoise2D(float x, float y, uint32_t seed) {
    int ix = static_cast<int>(std::floor(x));
    int iy = static_cast<int>(std::floor(y));
    float fx = x - static_cast<float>(ix);
    float fy = y - static_cast<float>(iy);

    float ux = fx * fx * (3.0f - 2.0f * fx);
    float uy = fy * fy * (3.0f - 2.0f * fy);

    uint32_t ux0 = static_cast<uint32_t>(ix) * 374761393U;
    uint32_t ux1 = static_cast<uint32_t>(ix + 1) * 374761393U;
    uint32_t uy0 = static_cast<uint32_t>(iy) * 668265263U;
    uint32_t uy1 = static_cast<uint32_t>(iy + 1) * 668265263U;

    float n00 = HashFloat(ux0 + uy0 + seed) * 2.0f - 1.0f;
    float n10 = HashFloat(ux1 + uy0 + seed) * 2.0f - 1.0f;
    float n01 = HashFloat(ux0 + uy1 + seed) * 2.0f - 1.0f;
    float n11 = HashFloat(ux1 + uy1 + seed) * 2.0f - 1.0f;

    float nx0 = n00 + ux * (n10 - n00);
    float nx1 = n01 + ux * (n11 - n01);
    return nx0 + uy * (nx1 - nx0);
}

inline void SampleSensorAndGrainNoise(
    float u,
    float v,
    int width,
    int height,
    float time_sec,
    float temporal_speed,
    int user_seed,
    float grain_size,
    float clumping,
    float contrast,
    float density,
    SensorResponseType sensor_type,
    bool sample_chroma,
    float& out_luma_grain,
    float& out_chroma_r,
    float& out_chroma_g,
    float& out_chroma_b)
{
    float safe_size = (grain_size < 0.25f) ? 0.25f : grain_size;
    float px = u * static_cast<float>(width) / safe_size;
    float py = v * static_cast<float>(height) / safe_size;

    uint32_t frame_step = static_cast<uint32_t>(std::floor(time_sec * 24.0f * (temporal_speed > 0.0f ? temporal_speed : 0.0f) + 0.5f));
    uint32_t base_seed = Hash32(static_cast<uint32_t>(user_seed) * 2654435761U + frame_step * 2246822519U + 0x9E3779B9U);

    float fine = ValueNoise2D(px, py, base_seed);
    float coarse = ValueNoise2D(px * 0.42f, py * 0.42f, base_seed ^ 0xA5A5A5A5U);
    float cluster = ValueNoise2D(px * 0.18f, py * 0.18f, base_seed ^ 0x5A5A5A5AU);

    float clump_mix = (clumping < 0.0f) ? 0.0f : (clumping > 1.0f ? 1.0f : clumping);
    float g = fine * (1.0f - 0.55f * clump_mix) + coarse * (0.40f * clump_mix) + cluster * (0.25f * clump_mix);

    // Uncorrelated density gate so grain remains strictly zero-mean
    float dens = (density < 0.05f) ? 0.05f : (density > 1.0f ? 1.0f : density);
    float gate = HashFloat(
        static_cast<uint32_t>(std::floor(px)) * 73856093U ^
        static_cast<uint32_t>(std::floor(py)) * 19349663U ^
        (base_seed + 0x7F4A7C15U));
    float dens_atten = (gate > dens) ? 0.25f : 1.0f;
    g *= dens_atten;

    // Contrast shaping
    float c_scale = 0.5f + contrast * 1.5f;
    g *= c_scale;

    out_luma_grain = g;

    if (!sample_chroma) {
        out_chroma_r = 0.0f;
        out_chroma_g = 0.0f;
        out_chroma_b = 0.0f;
        return;
    }

    // Sensor-specific spatial frequency for chromatic grain / demosaicing blotches
    float chroma_freq = 0.62f;
    switch (sensor_type) {
        case SENSOR_EARLY_MOBILE_CMOS:
            chroma_freq = 0.42f;
            break;
        case SENSOR_COMPACT_CCD:
            chroma_freq = 0.58f;
            break;
        case SENSOR_SONY_BIONZ:
        case SENSOR_CANON_DIGIC:
            chroma_freq = 0.68f;
            break;
        case SENSOR_PHOTOCHEMICAL_FILM:
            chroma_freq = 0.54f;
            break;
        case SENSOR_PUREVIEW_OVERSAMPLED:
            chroma_freq = 0.85f;
            break;
        default:
            chroma_freq = 0.62f;
            break;
    }

    float cpx = px * chroma_freq;
    float cpy = py * chroma_freq;

    float cr_fine = ValueNoise2D(cpx + 17.3f, cpy + 31.7f, base_seed ^ 0x13579BDFU);
    float cg_fine = ValueNoise2D(cpx + 59.1f, cpy + 11.9f, base_seed ^ 0x2468ACE0U);
    float cb_fine = ValueNoise2D(cpx + 83.7f, cpy + 47.2f, base_seed ^ 0xFEDCBA98U);

    float cr_clump = ValueNoise2D(cpx * 0.45f + 11.2f, cpy * 0.45f + 23.4f, base_seed ^ 0x31415926U);
    float cg_clump = ValueNoise2D(cpx * 0.45f + 37.8f, cpy * 0.45f + 19.1f, base_seed ^ 0x27182818U);
    float cb_clump = ValueNoise2D(cpx * 0.45f + 71.5f, cpy * 0.45f + 53.6f, base_seed ^ 0x16180339U);

    float cr_raw = cr_fine * (1.0f - 0.45f * clump_mix) + cr_clump * (0.45f * clump_mix);
    float cg_raw = cg_fine * (1.0f - 0.45f * clump_mix) + cg_clump * (0.45f * clump_mix);
    float cb_raw = cb_fine * (1.0f - 0.45f * clump_mix) + cb_clump * (0.45f * clump_mix);

    // Orthogonalize against luminance so chromatic grain is pure color-difference noise (zero luma shift, zero tint)
    float chroma_luma = 0.299f * cr_raw + 0.587f * cg_raw + 0.114f * cb_raw;
    out_chroma_r = (cr_raw - chroma_luma) * dens_atten * c_scale;
    out_chroma_g = (cg_raw - chroma_luma) * dens_atten * c_scale;
    out_chroma_b = (cb_raw - chroma_luma) * dens_atten * c_scale;
}

inline void SampleSensorAndGrainNoise(
    float u,
    float v,
    int width,
    int height,
    float time_sec,
    float temporal_speed,
    int user_seed,
    float grain_size,
    float clumping,
    float contrast,
    float density,
    bool sample_chroma,
    float& out_luma_grain,
    float& out_chroma_r,
    float& out_chroma_g,
    float& out_chroma_b)
{
    SampleSensorAndGrainNoise(
        u, v, width, height,
        time_sec, temporal_speed, user_seed,
        grain_size, clumping, contrast, density,
        SENSOR_COMPACT_CCD,
        sample_chroma,
        out_luma_grain, out_chroma_r, out_chroma_g, out_chroma_b);
}

} // namespace DigitalCamNoise

#endif // NOISE_ENGINE_H
