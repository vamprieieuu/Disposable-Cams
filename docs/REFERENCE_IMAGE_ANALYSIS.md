# Reference image analysis — Disposable Cams

## Scope and naming

The supplied archive contains 19 camera-look references plus `NORMAL.jpeg` as the neutral comparison. The visible names CPM35, Classic U, IR, FQS, CCD R, FXN and DHalf are preserved. The remaining images are referenced by filename because the source images do not establish their exact camera models.

## What can be inferred visually

The set depicts a consistent portrait subject, which is useful for comparing render responses. The important comparison axes are: skin hue and saturation; neutral-wall color cast; shadow density; direct-flash hardness and falloff; highlight clipping/roll-off; edge contrast and apparent sharpening; chroma noise; and vignette/optical falloff. These axes should be evaluated against NORMAL, not against unrelated photographs.

## Limits

A reference JPEG/DNG is an output sample, not the original camera pipeline. The exact sensor, lens, film stock, processing recipe and app implementation cannot be proven from these files alone. Therefore IMG_* labels remain filename-based and the profile coefficients are hand-authored approximations. DNG files require a raw-capable decoder for complete sensor-level analysis; they should not be treated as equivalent to JPEGs without raw development.

## Implementation map

`CameraProfiles.cpp` holds the per-engine response values. `ColorEngine.h` applies the color/tonal response and optical characteristics. `NoiseEngine.h` provides the grain/noise functions. `DigitalCam.cpp` defines the native AE UI and render entry points. The Windows GitHub Actions workflow remains the required build verification step.
