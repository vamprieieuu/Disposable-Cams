# Disposable Cams - Develop controls

This revision adds native After Effects parameters for global develop adjustments and connects each parameter to the shared pixel-processing engine used by legacy and Smart Render paths.

Implemented controls:
- Exposure (stops)
- Contrast
- Highlights / Shadows
- Whites / Blacks
- Temperature / Tint
- Vibrance / Saturation
- Clarity (midtone-weighted contrast approximation)
- Dehaze (contrast/black-point approximation)
- Develop Vignette

The adjustments run after the selected camera response and before Original Mix. They default to neutral, so the default image remains controlled by the selected camera engine. Camera names use plain readable ASCII words/digits and do not contain the stray characters `ǎ` or `ẽ`.

Important scope note: this is not a complete Lightroom clone. HSL per-color mixer, editable RGB point curves, masks, geometry/crop, spatial sharpening, and spatial noise reduction are not implemented in this revision; these need additional UI/data structures and, for spatial operations, neighboring-pixel access. Clarity and Dehaze here are practical per-pixel approximations, not Lightroom's full local algorithms.

## Added in this source revision

- HSL color mixer controls for Red, Orange, Yellow, Green, Aqua, Blue, Purple, and Magenta (Hue/Saturation/Luminance).
- Parametric master and per-channel RGB curve controls.
- Three-way color grading controls for shadows, midtones, and highlights.
- Detail controls for sharpening, luminance noise reduction, and color noise reduction.

Implementation note: the existing renderer is a per-pixel effect. The curve controls are parametric tone-response controls rather than a draggable arbitrary spline; the detail controls are per-pixel approximations and are not equivalent to spatial-radius sharpening or neighborhood-based denoising. Crop/geometry and local masking require a separate spatial/mask implementation and are not represented as implemented here.
