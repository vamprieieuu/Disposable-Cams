# Disposable Cams

Native Adobe After Effects effect plugin for Windows x64. Effect identity: **Disposable Cams**. Intended artifact: `Disposable Cams.aex`.

## Interface

The effect UI is now branded for Disposable Cams. The primary selector is **Camera Engine**, with 19 entries aligned to the supplied reference set. The former generic “Native Camera Effects” and “In-Camera Mode” labels have been replaced with **Color Rendering** and **Capture Conditions**, and the optics and grain controls are renamed for this plugin.

## Reference engines

Seven engines retain the labels visible in the supplied reference set: CPM35, Classic U, IR, FQS, CCD R, FXN, and DHalf. Twelve further engines are labelled by their source image filename (IMG_3792, IMG_3793, IMG_3794, IMG_3795 DNG, IMG_3797, IMG_3798, IMG_3799, IMG_3802 DNG, IMG_3804, IMG_3808, IMG_3809 DNG, IMG_3813). `NORMAL.jpeg` is a comparison baseline, not an engine.

The reference images guide hand-authored emulation parameters; they do not disclose the proprietary algorithms or exact source camera models. Do not interpret descriptive engine labels as verified hardware identifications. Engine differences are implemented through per-profile color matrices, white balance, exposure, contrast, saturation, gamma, highlight response, sensor/noise settings, grain, flash response, and optical falloff.

## Build

Use the existing GitHub Actions workflow or configure with Visual Studio 2022 x64 and the SDK in `extern/AfterEffectsSDK`. This source archive has not been verified by a Windows/MSVC build in this environment; run the repository workflow before treating the `.aex` as production-ready.
