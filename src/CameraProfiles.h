#ifndef CAMERA_PROFILES_H
#define CAMERA_PROFILES_H

#include <vector>

enum CameraEffectType {
    EFFECT_NONE = 0,
    EFFECT_STANDARD = 1,
    EFFECT_VIVID = 2,
    EFFECT_NATURAL = 3,
    EFFECT_PORTRAIT = 4,
    EFFECT_LANDSCAPE = 5,
    EFFECT_SUNSET = 6,
    EFFECT_NEUTRAL = 7,
    EFFECT_FAITHFUL = 8,
    EFFECT_MONOCHROME = 9,
    EFFECT_BW = 10,
    EFFECT_SEPIA = 11,
    EFFECT_NEGATIVE = 12,
    EFFECT_SOLARIZE = 13,
    EFFECT_PASTEL = 14,
    EFFECT_MOSAIC = 15,
    EFFECT_NEG_ART = 16,
    EFFECT_FILM_GRAIN = 17,
    EFFECT_PIN_HOLE = 18,
    EFFECT_BEAUTY_SOFT = 19,
    EFFECT_DREAMY = 20,
    EFFECT_VIGNETTING = 21,
    EFFECT_SALON = 22,
    EFFECT_GRAINY = 23,
    EFFECT_TOY_CAMERA = 24,
    EFFECT_POP_COLOR = 25,
    EFFECT_CLEAR = 26,
    EFFECT_DEEP = 27,
    EFFECT_LIGHT = 28,
    EFFECT_FILM_PORTRA = 29,
    EFFECT_FILM_GOLD = 30,
    EFFECT_FILM_SUPERIA = 31,
    EFFECT_FILM_HP5 = 32,
    EFFECT_FILM_TRIX = 33,
    EFFECT_WIZEN_WARM = 34,
    EFFECT_WIZEN_MATTE = 35,
    EFFECT_WIZEN_BW = 36,
    EFFECT_POSITIVE_FILM = 37
};

enum SensorResponseType {
    SENSOR_GENERIC = 0,
    SENSOR_COMPACT_CCD = 1,
    SENSOR_SONY_BIONZ = 2,
    SENSOR_CANON_DIGIC = 3,
    SENSOR_PHOTOCHEMICAL_FILM = 4,
    SENSOR_EARLY_MOBILE_CMOS = 5,
    SENSOR_PUREVIEW_OVERSAMPLED = 6
};

struct CameraEffectDesc {
    CameraEffectType type;
    const char* name;
    float default_strength;
};

struct CameraProfile {
    int id;
    const char* name;
    const char* category;
    const char* sensor_desc;
    float color_matrix[9];
    float wb_r;
    float wb_g;
    float wb_b;
    float exposure_bias;
    float contrast;
    float saturation;
    float gamma;
    SensorResponseType sensor_type;
    float black_level;
    float highlight_clip;
    float highlight_rolloff;
    float jpeg_quality;
    float chromatic_aberration;
    float vignette_strength;
    float nr_smear;
    bool has_watercolor_nr;
    float luma_noise;
    float chroma_noise;
    float grain_size;
    bool has_flash;
    float flash_falloff;
    float flash_intensity;
    float flash_wb_r;
    float flash_wb_g;
    float flash_wb_b;
    bool is_film;
    bool is_pureview;
    bool is_calibration_only;
    std::vector<CameraEffectDesc> effects;
};

class CameraProfileManager {
public:
    static void EnsureInitialized();
    static const std::vector<CameraProfile>& GetAllProfiles();
    static const CameraProfile& GetProfile(int index);
    static int GetProfileCount();
};

#endif // CAMERA_PROFILES_H
