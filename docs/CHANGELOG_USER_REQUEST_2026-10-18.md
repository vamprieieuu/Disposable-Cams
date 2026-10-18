# Disposable Cams fixes (2026-10-18)

- Original Mix now spans -100..100. Positive values blend toward source; negative values extrapolate away from source to intensify the camera rendering.
- Exposure range widened to -10..10; implementation uses 2^EV, so positive values can intentionally clip to white.
- Added a Blend Colors From Image checkbox. When enabled, chroma-grain channel energy is weighted by source-image chromatic content.
- Registered/checks out/checks in the added parameter and all extra Color Mixer/Curve/Grading/Detail parameters consistently.
- Color Grading Balance now affects shadow/highlight weighting.
- Tone Curve sliders now apply a stronger tonal bend, with separate channel bends. These are still parametric controls, not an interactive spline graph like After Effects' Curves effect; a real graph editor requires a dedicated custom UI implementation.
- Sharpen Radius now scales the sharpening response; noise reduction no longer simply desaturates the whole image as aggressively.
- Strengthened profile contrast/saturation separation for cameras that were reading as too similar.

Build/runtime note: source-level checks only; this archive has not been compiled with the Windows After Effects SDK or tested in After Effects in this environment.
