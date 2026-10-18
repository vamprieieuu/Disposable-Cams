# Disposable Cams — Research Update 2026-10-07

This update is implementation guidance, not a claim of factory calibration.

## Confirmed rendering traits

- Panasonic TZ7: Film Grain and Pin Hole are documented scene modes. Reviews report very clean ISO 80–200 output, visible noise/softening at higher ISO, and slightly soft default JPEG sharpening. The plugin should model the TZ7 as a compact CCD-era JPEG/ISP pipeline rather than simply adding grain.
- Sony DSC-W180: Sony documents ISO Auto/100–3200, Normal/Sepia/B&W color mode, multiple WB modes, flash levels, and scene selection. It has no user contrast/sharpness controls in the official specification, so those should be internal profile coefficients rather than exposed fake controls.
- Sony H7/H9: H9 testing shows strong BIONZ-era NR, watercolor-like low-contrast smearing, chromatic aberration/purple fringing, wide-end distortion, and DRO-like highlight/shadow contrast masking. H7 and H9 should be separable profiles if enough sample data exists.
- Sony Handycam: the exact model matters. Documented older Handycam picture effects include NEG.ART, SEPIA, B&W, SOLARIZE, SLIM, STRETCH, PASTEL and MOSAIC. These should not be presented as universal behavior for every Handycam model.
- Canon EOS 400D/XTi: Picture Style includes Standard, Portrait, Landscape, Neutral, Faithful and Monochrome; sharpness and contrast are adjustable and monochrome supports filter/toning effects. JPEG rendering should be profile-specific.
- Sony DSC-T70: testing shows purple/blue noise blobs at higher ISO, sharpening of noise at ISO 800+, elevated reds, skin/purple-blue shifts, strong chromatic aberration and some barrel distortion. The profile should reproduce these characteristics in the pipeline, not as a generic grain overlay.
- Kodak PIXPRO FZ55: official manual documents Normal/Vivid/B&W/Sepia/Dreamy/Vignetting/Salon color effects, WB options, HDR and multiple JPEG quality/compression levels. Playback also exposes Grainy as a separate color effect.
- Olympus mju 5000: testing reports visible noise even at ISO64, more detail loss and green/purple contamination around ISO400, and a Beauty mode that smooths skin.
- Nokia N73: documented WB, exposure compensation, color tone and light sensitivity; reviews report severe low-light noise reduction that destroys detail and creates a watercolor-like appearance, plus slight purple fringing and muted/less-vivid color.
- Nokia N82/N95: documented color tone, WB, exposure, sharpness, contrast, light sensitivity and flash controls. N95 documentation explicitly notes that higher sensitivity can increase graininess.
- Nokia N93: reviews show significantly higher noise than K800 at 200% and a cooler rendering than K800; weak LED flash is effective mainly at close range. Downsampling masks some noise.
- Nokia N86: tests report consistently oversaturated red/pink bias and significant noise in darker chart areas.
- Nokia N8: reviews describe good noise handling with preserved detail, but limited dynamic range; highlights can clip and blacks can collapse. Centre-weighted metering is reported.
- Nokia 808 PureView: PureView oversampling is a distinct image formation stage. Creative mode exposes saturation, contrast, sharpness and Normal/Vivid/Sepia/B&W color tones. Superfine JPEG reduces visible JPEG artifacts relative to Normal.
- Sony Ericsson K800i: official white paper documents B&W/Negative/Sepia/Solarize, WB, Normal/Spot metering, Normal/Fine quality and Xenon flash. Low-light/flash behavior should be modeled as part of exposure/noise/flash processing.

## Unresolved identity

Wezein Excel 2 remains an unresolved target identity. Do not invent specifications, sensor data, effects, or manufacturer behavior. Keep it as a reference target until exact model identity or reliable source material is available.

## Architecture requirement

The emulation must be profile-driven. A profile is not a LUT. Each profile should define its own sensor/noise/NR/sharpening/tone/color/lens/flash/JPEG/ISP coefficients and native effects. The same source image should therefore produce materially different outputs for different profiles.

Reference images are calibration evidence only. Never ship them as image textures or overlays.
