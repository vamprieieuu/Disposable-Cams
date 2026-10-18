# Disposable Cams

Native Adobe After Effects effect plugin for Windows x64. The effect is named **Disposable Cams** and the production artifact is `Disposable Cams.aex`.

## Camera engine notes

The 19 camera entries are emulation profiles guided by the supplied Dazz Cam reference set. Labeled references retain their visible names (CPM35, Classic U, IR, FQS, CCD R, FXN, DHalf); unlabeled references use descriptive Pocket CCD labels tied to the original image filenames. `NORMAL.jpeg` is a neutral comparison reference, not a camera engine. These are authored approximations of visible output, not Dazz Cam's unpublished internal algorithms.

The profiles drive independent color matrices, white balance, exposure, contrast, saturation, gamma, highlight response, sensor/noise model, grain, flash response, and optical falloff. The existing build toolchain and SDK layout are intentionally retained.

## Build

Use the existing GitHub Actions workflow or configure with Visual Studio 2022 x64 and the SDK in `extern/AfterEffectsSDK`.
