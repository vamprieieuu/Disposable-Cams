#include "CameraProfiles.h"
#include <cstddef>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
static volatile LONG s_init_state = 0;
#else
#include <atomic>
static std::atomic<int> s_init_state{0};
#endif

static std::vector<CameraProfile> s_profiles;

static CameraProfile MakeProfile(
    int id, const char* name, const char* category, SensorResponseType sensor,
    float rr, float rg, float rb, float gr, float gg, float gb, float br, float bg, float bb,
    float wbR, float wbG, float wbB, float exposure, float contrast, float saturation,
    float gamma, float black, float clip, float rolloff, float jpeg,
    float ca, float vignette, float nr, float lumaNoise, float chromaNoise,
    float grainSize, bool flash, float falloff, float flashIntensity,
    float flashR, float flashG, float flashB, bool film, bool watercolor)
{
    CameraProfile p{};
    p.id = id; p.name = name; p.category = category;
    p.sensor_desc = (sensor == SENSOR_PHOTOCHEMICAL_FILM) ? "Photochemical disposable film response" :
                    (sensor == SENSOR_COMPACT_CCD) ? "Early compact CCD / JPEG response" :
                    (sensor == SENSOR_EARLY_MOBILE_CMOS) ? "Early mobile CMOS response" : "Digital compact response";
    p.color_matrix[0]=rr; p.color_matrix[1]=rg; p.color_matrix[2]=rb;
    p.color_matrix[3]=gr; p.color_matrix[4]=gg; p.color_matrix[5]=gb;
    p.color_matrix[6]=br; p.color_matrix[7]=bg; p.color_matrix[8]=bb;
    p.wb_r=wbR; p.wb_g=wbG; p.wb_b=wbB; p.exposure_bias=exposure;
    p.contrast=contrast; p.saturation=saturation; p.gamma=gamma; p.sensor_type=sensor;
    p.black_level=black; p.highlight_clip=clip; p.highlight_rolloff=rolloff; p.jpeg_quality=jpeg;
    p.chromatic_aberration=ca; p.vignette_strength=vignette; p.nr_smear=nr;
    p.has_watercolor_nr=watercolor; p.luma_noise=lumaNoise; p.chroma_noise=chromaNoise;
    p.grain_size=grainSize; p.has_flash=flash; p.flash_falloff=falloff;
    p.flash_intensity=flashIntensity; p.flash_wb_r=flashR; p.flash_wb_g=flashG; p.flash_wb_b=flashB;
    p.is_film=film; p.is_pureview=false; p.is_calibration_only=false;
    p.effects.push_back({EFFECT_STANDARD, "Camera Native", 1.0f});
    return p;
}

static void InitProfilesInternal() {
    if (!s_profiles.empty()) return;
    s_profiles.reserve(19);
    // Reference-derived profiles. Coefficients are hand-tuned emulation parameters, not claimed Dazz Cam internals.
    s_profiles.push_back(MakeProfile(1, "CPM35", "Disposable / film compact", SENSOR_PHOTOCHEMICAL_FILM, 1.10f,-.04f,-.02f, -.02f,1.02f,0.00f, -.03f,-.02f,.96f, 1.05f,1.00f,.94f, .03f,1.08f,1.12f,2.15f, .025f,.96f,.23f,.76f, .0012f,.28f,.08f, .055f,.032f,1.25f, true,.52f,1.38f,1.10f,1.02f,.90f,true,false));
    s_profiles.push_back(MakeProfile(2, "Classic U", "Disposable / film compact", SENSOR_PHOTOCHEMICAL_FILM, 1.08f,-.02f,-.03f, .00f,1.01f,-.01f, -.04f,.00f,.96f, 1.04f,1.00f,.96f, .01f,1.04f,1.06f,2.20f, .028f,.97f,.20f,.82f, .0008f,.18f,.04f, .040f,.025f,1.05f, true,.48f,1.30f,1.06f,1.00f,.94f,true,false));
    s_profiles.push_back(MakeProfile(3, "IR", "Infrared / stylized digital", SENSOR_COMPACT_CCD, 1.08f,.02f,-.10f, .02f,1.03f,-.05f, -.04f,.02f,1.12f, 1.02f,1.00f,1.10f, .04f,1.18f,1.24f,2.10f, .018f,.99f,.12f,.72f, .0010f,.16f,.10f, .035f,.048f,1.10f, true,.42f,1.32f,1.00f,1.02f,1.12f,false,false));
    s_profiles.push_back(MakeProfile(4, "FQS", "Digital compact", SENSOR_COMPACT_CCD, 1.13f,-.05f,-.03f, -.02f,1.05f,-.02f, -.02f,-.04f,1.08f, 1.04f,1.00f,1.02f, .02f,1.12f,1.16f,2.18f, .035f,.93f,.16f,.70f, .0018f,.25f,.20f, .050f,.044f,1.15f, true,.55f,1.42f,1.08f,1.00f,.94f,false,true));
    s_profiles.push_back(MakeProfile(5, "CCD R", "Digital compact CCD", SENSOR_COMPACT_CCD, 1.15f,-.08f,-.04f, -.03f,1.06f,-.02f, -.04f,-.02f,1.10f, 1.05f,1.00f,1.04f, .00f,1.10f,1.13f,2.22f, .040f,.92f,.18f,.68f, .0020f,.26f,.22f, .050f,.052f,1.20f, true,.58f,1.36f,1.08f,1.00f,.92f,false,true));
    s_profiles.push_back(MakeProfile(6, "FXN", "Digital compact", SENSOR_SONY_BIONZ, 1.06f,-.02f,-.01f, -.01f,1.03f,-.02f, -.02f,-.02f,1.08f, 1.02f,1.00f,1.04f, -.01f,1.06f,1.08f,2.20f, .022f,.96f,.22f,.82f, .0008f,.12f,.12f, .028f,.030f,.90f, true,.46f,1.28f,1.03f,1.00f,1.02f,false,false));
    s_profiles.push_back(MakeProfile(7, "DHalf", "Digital compact / half-frame character", SENSOR_EARLY_MOBILE_CMOS, 1.10f,-.06f,-.02f, -.02f,1.00f,.02f, -.03f,.02f,1.02f, 1.05f,1.00f,.96f, .02f,1.14f,1.18f,2.08f, .045f,.91f,.14f,.62f, .0024f,.32f,.24f, .065f,.070f,1.35f, true,.60f,1.45f,1.10f,1.00f,.90f,false,true));
    s_profiles.push_back(MakeProfile(8, "Pocket CCD 01 (IMG_3792)", "Digital compact", SENSOR_COMPACT_CCD, 1.12f,-.06f,-.03f, -.02f,1.03f,-.01f, -.03f,-.02f,1.09f, 1.03f,1.00f,1.03f, .01f,1.09f,1.10f,2.20f, .032f,.94f,.17f,.76f, .0015f,.22f,.18f, .044f,.040f,1.05f, true,.50f,1.34f,1.06f,1.00f,.95f,false,false));
    s_profiles.push_back(MakeProfile(9, "Pocket CCD 02 (IMG_3793)", "Digital compact", SENSOR_COMPACT_CCD, 1.07f,-.03f,-.02f, .00f,1.02f,-.02f, -.03f,-.01f,1.06f, 1.01f,1.00f,1.02f, -.02f,1.02f,1.03f,2.25f, .026f,.97f,.24f,.86f, .0007f,.10f,.08f, .030f,.028f,.88f, true,.43f,1.22f,1.03f,1.00f,.98f,false,false));
    s_profiles.push_back(MakeProfile(10, "Pocket CCD 03 (IMG_3794)", "Digital compact", SENSOR_COMPACT_CCD, 1.13f,-.04f,-.04f, -.02f,1.02f,.00f, -.03f,-.01f,1.08f, 1.04f,1.00f,1.00f, .01f,1.12f,1.14f,2.18f, .035f,.93f,.16f,.70f, .0014f,.24f,.17f, .046f,.040f,1.10f, true,.55f,1.38f,1.08f,1.00f,.93f,false,true));
    s_profiles.push_back(MakeProfile(11, "Pocket CCD 04 (IMG_3795 DNG)", "Digital compact / DNG reference", SENSOR_COMPACT_CCD, 1.06f,-.02f,-.01f, -.01f,1.03f,-.02f, -.02f,-.02f,1.05f, 1.01f,1.00f,1.01f, 0.0f,1.03f,1.04f,2.22f, .018f,.99f,.25f,.94f, .0004f,.08f,.04f, .018f,.018f,.80f, false,.40f,1.0f,1.0f,1.0f,1.0f,false,false));
    s_profiles.push_back(MakeProfile(12, "Pocket CCD 05 (IMG_3797)", "Digital compact", SENSOR_COMPACT_CCD, 1.10f,-.03f,-.02f, -.01f,1.03f,-.01f, -.03f,-.01f,1.07f, 1.02f,1.00f,1.03f, .02f,1.07f,1.08f,2.16f, .030f,.95f,.17f,.74f, .0010f,.18f,.12f, .038f,.034f,1.0f, true,.46f,1.30f,1.05f,1.00f,.95f,false,false));
    s_profiles.push_back(MakeProfile(13, "Pocket CCD 06 (IMG_3798)", "Digital compact", SENSOR_SONY_BIONZ, 1.08f,-.02f,-.02f, -.01f,1.02f,-.01f, -.02f,-.01f,1.06f, 1.02f,1.00f,1.02f, -.01f,1.04f,1.06f,2.24f, .022f,.97f,.23f,.84f, .0008f,.12f,.10f, .028f,.030f,.92f, true,.43f,1.24f,1.03f,1.00f,.98f,false,false));
    s_profiles.push_back(MakeProfile(14, "Pocket CCD 07 (IMG_3799)", "Digital compact", SENSOR_EARLY_MOBILE_CMOS, 1.11f,-.05f,-.02f, -.02f,1.01f,.01f, -.03f,.01f,1.03f, 1.04f,1.00f,.97f, .03f,1.13f,1.16f,2.10f, .042f,.92f,.14f,.64f, .0020f,.30f,.22f, .060f,.064f,1.30f, true,.58f,1.42f,1.09f,1.00f,.91f,false,true));
    s_profiles.push_back(MakeProfile(15, "Pocket CCD 08 (IMG_3802 DNG)", "Digital compact / DNG reference", SENSOR_COMPACT_CCD, 1.05f,-.02f,-.01f, -.01f,1.02f,-.01f, -.02f,-.01f,1.05f, 1.01f,1.00f,1.01f, 0.0f,1.02f,1.03f,2.22f, .016f,.99f,.24f,.95f, .0003f,.07f,.04f, .016f,.016f,.80f, false,.40f,1.0f,1.0f,1.0f,1.0f,false,false));
    s_profiles.push_back(MakeProfile(16, "Pocket CCD 09 (IMG_3804)", "Digital compact", SENSOR_COMPACT_CCD, 1.13f,-.06f,-.03f, -.02f,1.04f,-.01f, -.03f,-.02f,1.09f, 1.04f,1.00f,1.02f, .01f,1.10f,1.13f,2.18f, .034f,.94f,.17f,.73f, .0013f,.23f,.16f, .045f,.041f,1.08f, true,.52f,1.36f,1.07f,1.00f,.94f,false,false));
    s_profiles.push_back(MakeProfile(17, "Pocket CCD 10 (IMG_3808)", "Digital compact", SENSOR_EARLY_MOBILE_CMOS, 1.10f,-.04f,-.02f, -.01f,1.02f,.00f, -.03f,.01f,1.04f, 1.03f,1.00f,.98f, .02f,1.12f,1.14f,2.12f, .040f,.92f,.15f,.66f, .0018f,.28f,.20f, .055f,.060f,1.22f, true,.56f,1.40f,1.08f,1.00f,.92f,false,true));
    s_profiles.push_back(MakeProfile(18, "Pocket CCD 11 (IMG_3809 DNG)", "Digital compact / DNG reference", SENSOR_COMPACT_CCD, 1.06f,-.02f,-.01f, -.01f,1.02f,-.01f, -.02f,-.01f,1.05f, 1.01f,1.00f,1.01f, 0.0f,1.02f,1.03f,2.22f, .017f,.99f,.24f,.94f, .0003f,.07f,.04f, .017f,.017f,.80f, false,.40f,1.0f,1.0f,1.0f,1.0f,false,false));
    s_profiles.push_back(MakeProfile(19, "Pocket CCD 12 (IMG_3813)", "Digital compact", SENSOR_SONY_BIONZ, 1.08f,-.02f,-.02f, -.01f,1.03f,-.02f, -.02f,-.01f,1.07f, 1.02f,1.00f,1.03f, -.01f,1.05f,1.07f,2.23f, .022f,.97f,.22f,.83f, .0008f,.13f,.10f, .030f,.030f,.92f, true,.44f,1.25f,1.04f,1.00f,.98f,false,false));
}

void CameraProfileManager::EnsureInitialized() {
#ifdef _WIN32
    if (s_init_state == 2) {
        return;
    }
    if (InterlockedCompareExchange(&s_init_state, 1, 0) == 0) {
        InitProfilesInternal();
        InterlockedExchange(&s_init_state, 2);
    } else {
        while (s_init_state != 2) {
            Sleep(1);
        }
    }
#else
    if (s_init_state.load(std::memory_order_acquire) == 2) {
        return;
    }
    int expected = 0;
    if (s_init_state.compare_exchange_strong(expected, 1, std::memory_order_acq_rel)) {
        InitProfilesInternal();
        s_init_state.store(2, std::memory_order_release);
    } else {
        while (s_init_state.load(std::memory_order_acquire) != 2) {
        }
    }
#endif
}

const std::vector<CameraProfile>& CameraProfileManager::GetAllProfiles() {
    EnsureInitialized();
    return s_profiles;
}

int CameraProfileManager::GetProfileCount() {
    EnsureInitialized();
    return static_cast<int>(s_profiles.size());
}

const CameraProfile& CameraProfileManager::GetProfile(int index) {
    EnsureInitialized();
    if (index >= 0 && index < static_cast<int>(s_profiles.size())) {
        return s_profiles[static_cast<size_t>(index)];
    }
    return s_profiles[0];
}
