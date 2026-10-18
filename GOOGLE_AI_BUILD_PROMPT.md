# MAJEED — FINAL CAMERA EMULATION ENGINE BUILD PROMPT

You are finishing a real native Adobe After Effects C++ effect plugin named **MAJEED Camera Emulation**.

## NON-NEGOTIABLE GOAL

The goal is NOT a LUT pack, preset pack, JSX script, stack of native AE effects, texture overlay, reference-image overlay, or fake camera filter.

The goal is:

> Put a modern photo/video into After Effects, choose a real historical camera/phone profile, and make the output look as if the source had actually been captured and processed by that camera/phone.

Every camera must have its own rendering DNA.

The plugin must generate its processing procedurally in C++.

Reference JPGs are calibration/reference evidence only. NEVER use them as runtime textures, overlays, mattes, lookup images, or compositing layers.

## ENVIRONMENT

Target:
- Adobe After Effects 23.2.1
- Windows 11
- Windows x64
- Visual Studio 2022 / MSVC
- Adobe After Effects SDK supplied in this repository

## SDK — MUST USE extern

The Adobe SDK is already included under:

`extern/AfterEffectsSDK/`

Use that SDK directly.

Expected paths include:
- `extern/AfterEffectsSDK/Examples/Headers`
- `extern/AfterEffectsSDK/Examples/Util`
- `extern/AfterEffectsSDK/Examples/Resources/PiPLtool.exe`

Do NOT remove `extern`.
Do NOT replace it with a guessed SDK.
Do NOT require the user to download another SDK unless absolutely necessary.
Do NOT use a different API generation without proving compatibility.

The SDK was obtained by the project owner from Adobe. Keep it in the PRIVATE build package. Before any public redistribution, respect Adobe SDK licensing/redistribution terms.

## FIRST: AUDIT THE EXISTING CODE

Do not start by writing a proposal.
Open and inspect every project file first.
Then implement.

Inspect:
- CMakeLists.txt
- src/MAJEED_CameraEmulation.cpp
- src/MAJEED_CameraEmulation.h
- src/CameraProfiles.cpp
- src/CameraProfiles.h
- src/NoiseEngine.h
- src/ColorEngine.h
- src/MAJEED_CameraEmulationPiPL.r
- profiles/profiles.json
- docs/*
- reference_images/*
- extern/AfterEffectsSDK/Examples/Effect/SDK_Noise/*
- other relevant Adobe SDK examples, especially SmartFX/render examples

Use the included SDK examples as the authoritative implementation reference.

## IMPORTANT BUGS TO VERIFY AND FIX

The project has already been audited and these areas require attention:

1. `PF_OutFlag_NON_PARAM_VARY` is required because the procedural noise/grain changes over time.
2. `PF_OutFlag2_SUPPORTS_SMART_RENDER` must match the actual implementation.
3. `PF_OutFlag2_FLOAT_COLOR_AWARE` must match the actual 32-bpc path.
4. `PF_OutFlag2_SUPPORTS_THREADED_RENDERING` must only be used if all state is thread-safe.
5. PiPL `AE_Effect_Global_OutFlags` and `_2` MUST exactly match the actual `GlobalSetup` flags.
6. Use the included SDK_Noise PiPL/source as the reference. Its documented values are:
   - Global OutFlags: `0x02000404`
   - Global OutFlags 2: `0x08001400`
7. Do not advertise SmartFX in PiPL/global flags while implementing only legacy PF_Cmd_RENDER.
8. Implement the correct SmartFX pre-render/smart-render flow if those flags are retained.
9. Correctly check out input/output worlds and release them.
10. Correctly support 8-bit, 16-bit and 32-bit float.
11. Do not use unsafe shared mutable global state during threaded rendering.
12. Temporal noise must be deterministic from frame/time + seed but different from frame to frame.
13. The effect must not require extra AE solids, precomps, hidden layers, or generated compositions.
14. The effect must process only the current input layer.
15. Do not use `https://example.invalid` as a fake production support URL. Use the actual project/repository URL if available, or remove/replace it appropriately.

## CAMERA ENGINE ARCHITECTURE

Implement a real profile-driven pipeline.

Recommended conceptual order:

Input RGB
→ exposure / sensitivity response
→ sensor response approximation
→ optional CFA/demosaic statistical model where appropriate
→ white balance
→ camera color matrix / channel response
→ tone curve
→ highlight rolloff
→ shadow response
→ dynamic-range behavior / clipping
→ lens optical character
→ distortion / CA / vignette
→ camera-specific denoise
→ camera-specific sharpening
→ sensor/read/chroma noise
→ temporal noise
→ flash illumination model when enabled
→ camera-specific ISP/JPEG behavior
→ native camera effect
→ output

The exact implementation order may change if the math requires it, but the final result must behave like an image-processing pipeline rather than a collection of independent sliders.

## IMPORTANT: DO NOT EXPOSE FAKE UNIVERSAL CONTROLS

The current UI contains generic controls such as:
- Grain
- Grain Size
- Grain Speed
- Luma Noise
- Chroma Noise
- Exposure
- Contrast
- Saturation
- Warmth
- Sharpening
- Denoise
- Lens Character
- Vignette
- Flash Character
- JPEG/ISP
- ISO
- Native Effect

Keep useful controls if they make sense, but the selected camera profile must determine the default behavior and internal coefficients.

Do not make every camera behave identically and merely change a color multiplier.

## CAMERA PROFILES — 23 TARGETS

Implement and retain all 23 profiles:

### Digital cameras
1. Panasonic Lumix TZ7 / ZS3
2. Sony Cyber-shot DSC-W180
3. Sony Cyber-shot H7 / H9
4. Sony Handycam family
5. Canon PowerShot A-series CCD
6. Canon AE-1
7. Sony NEX-5
8. Canon EOS 400D / Rebel XTi
9. Sony a6000
10. Sony DSC-T70
11. Kodak PIXPRO FZ55
12. Olympus mju 5000
13. Wezein Excel 2

### Phones
14. Nokia 7610
15. Nokia N73
16. Nokia N82
17. Nokia N95
18. Nokia N93
19. Nokia N86
20. Nokia N8
21. Nokia 808 PureView
22. Sony Ericsson K750i
23. Sony Ericsson K800i

## PROFILE-SPECIFIC RESEARCH REQUIREMENT

Do not flatten these profiles into one generic old-camera look.

### Panasonic TZ7 / ZS3
Research and implement:
- compact CCD-era color response
- low ISO clean output
- higher ISO noise/softening progression
- default soft JPEG sharpening
- limited CA
- flash behavior
- JPEG quality behavior
- Film Grain scene
- Pin Hole scene
- scene-specific processing where documented

### Sony DSC-W180
Research and implement:
- ISO Auto/100/200/400/800/1600/3200 behavior
- Normal/Sepia/B&W color mode
- WB modes
- flash behavior and flash level
- scene selection
- compact Sony JPEG/ISP character
- do NOT invent user-facing contrast/sharpness controls that the camera does not provide

### Sony H7 / H9
Treat H7/H9 as a distinct Sony BIONZ compact-camera family, and separate them internally if evidence supports it.
Important observed H9 traits:
- strong noise reduction
- watercolor-like smearing
- chromatic aberration/purple fringing
- wide-end barrel distortion
- telephoto pincushion distortion
- edge softness
- DRO-like shadow/highlight contrast masking
- JPEG compression characteristics
- default output can look somewhat overcooked

### Sony Handycam
Do NOT claim one exact model's behavior is universal to all Handycams.
Implement a family approximation while preserving model-specific effect definitions where documented.
Native picture effects documented on representative older models include:
- NEG. ART
- SEPIA
- B&W
- SOLARIZE
- SLIM
- STRETCH
- PASTEL
- MOSAIC

### Canon PowerShot A-series CCD
This is a family profile, not a claim about every A-series model.
Implement:
- CCD compact-camera response
- Canon compact color rendering
- JPEG/ISP
- sharpening/NR
- WB
- flash
- scene behavior
Keep the profile architecture extensible so an exact A-series model can later replace the family approximation.

### Canon AE-1
THIS IS FILM, NOT DIGITAL.
Do NOT apply a CCD/CMOS/JPEG pipeline.
Use a separate film simulation architecture:
- exposure response
- film stock abstraction
- lens response
- film contrast curve
- color response
- grain structure
- grain clustering
- halation
- shoulder/toe
- development character
- scan response

Do not falsely claim that the AE-1 itself has a specific film stock. The AE-1 is a camera body; film stock/development must be represented as a selectable/defined simulation layer.

### Sony NEX-5
Research and implement:
- APS-C response
- Sony JPEG color
- Creative Style behavior
- DRO / dynamic range behavior
- noise reduction progression
- sharpening
- WB
- highlight/shadow behavior
- JPEG output character

### Canon EOS 400D / XTi
Implement:
- Picture Style Standard
- Portrait
- Landscape
- Neutral
- Faithful
- Monochrome
- sharpness/contrast/color-tone behavior
- monochrome filter effects: Yellow/Orange/Red/Green
- monochrome toning: Sepia/Blue/Purple/Green
- Canon JPEG tone/color behavior

### Sony a6000
Implement:
- Sony JPEG pipeline
- context-sensitive noise reduction behavior
- sharpening behavior
- Creative Style
- WB
- DRO / dynamic-range processing
- highlight/shadow behavior
- JPEG output

Research indicates its NR can smooth continuous-tone areas aggressively enough to erode real detail. Reproduce the characteristic spatially, not as a global blur.

### Sony DSC-T70
Implement:
- compact Sony color response
- elevated reds
- skin/purple-blue shifts
- barrel distortion
- strong lateral CA/purple/blue fringing
- ISO-dependent noise
- noise that becomes visibly sharpened at high ISO
- high-ISO detail loss
- flash behavior
- BIONZ-era JPEG processing
- documented Color Mode: Normal/Vivid/Natural/Sepia/B&W
- documented in-camera effect behavior where applicable, including Retro/Soft Edge/Cross/Partial Color/Fisheye/Radiation-type effects only when verified for the actual model/manual

### Kodak PIXPRO FZ55
Implement documented:
- Normal
- Vivid
- Black & White
- Sepia
- Dreamy
- Vignetting
- Salon
- playback Grainy effect where relevant
- WB including manual/color-temperature behavior
- HDR
- quality/compression levels
- compact-camera JPEG character

Do not confuse playback-only effects with capture-time effects.

### Olympus mju 5000
Implement:
- ISO progression
- visible low-ISO noise
- increasing noise/detail loss at higher ISO
- green/purple chroma contamination at higher ISO
- Beauty skin smoothing
- WB limitations/blue cast behavior where documented
- flash warming behavior
- compact Olympus JPEG/ISP character

### Wezein Excel 2
CRITICAL:
The identity is unresolved.
Do NOT invent specifications.
Do NOT fabricate a manufacturer, sensor, lens, ISP, ISO range, effects, or sample behavior.
Keep a dedicated profile slot and clearly mark it as calibration/reference target.
If reliable sources can identify the exact device, document them; otherwise implement only evidence-backed behavior from supplied reference material.

## PHONE CAMERA DNA

### Nokia 7610
Treat as early phone-camera rendering:
- small sensor behavior
- low-light noise
- limited dynamic range
- legacy JPEG compression
- color response
- sharpening/NR
- documented effects only

### Nokia N73
Important:
- 3.2MP-era phone rendering
- low-light noise
- very strong NR that can destroy detail and create watercolor-like smearing
- slight purple fringing
- less-vivid color than modern phones
- documented WB
- exposure compensation
- Normal/Sepia/B&W/Negative/Vivid color tones
- Low/Medium/High light sensitivity
- flash behavior

### Nokia N82
Implement:
- Xenon flash behavior
- color tone
- WB
- exposure compensation
- sharpness
- contrast
- light sensitivity
- N-series JPEG/NR/sharpening
- low-light color/noise behavior

### Nokia N95
Implement:
- color tone
- WB
- exposure
- sharpness
- contrast
- light sensitivity
- higher sensitivity → more grain/noise
- LED flash behavior
- N-series JPEG/NR/sharpening

### Nokia N93
Implement:
- higher noise than K800 in comparable tests
- cooler rendering than K800
- realistic/pleasant general colors
- weak close-range LED flash behavior
- small-lens/small-sensor limitations
- indoor noise + motion blur character
- downsampling behavior

### Nokia N86
Implement:
- oversaturated red/pink bias
- significant dark-area noise
- detail preservation vs noise tradeoff
- Nokia sharpening/NR behavior
- documented color effects and ISO modes

### Nokia N8
Implement:
- larger sensor than typical phone peers of its era
- good noise handling
- preserved detail
- centre-weighted metering behavior as the default documented model
- limited dynamic range
- black crush/highlight clipping behavior
- Xenon flash
- natural but vibrant color strategy
- avoid over-sharpened modern smartphone look

### Nokia 808 PureView
This profile MUST be materially different from normal phone profiles.
Implement a PureView-like oversampling stage:
- high-resolution virtual sensor source
- multi-pixel oversampling/downsampling model
- noise reduction through oversampling
- improved highlight/detail retention
- reduced random noise
- characteristic sharpening after oversampling
- Creative mode controls:
  - saturation
  - contrast
  - sharpness
  - Normal/Vivid/Sepia/B&W
- Normal/Superfine JPEG behavior

Do not fake PureView by merely applying blur + sharpen.

### Sony Ericsson K750i
Implement documented:
- B&W
- Negative
- Sepia
- Solarize
- WB Auto/Daylight/Cloudy/Fluorescent/Incandescent
- night mode
- quality/compression differences
- early Sony Ericsson color/NR/sharpening
- flash behavior

### Sony Ericsson K800i
Implement documented:
- B&W
- Negative
- Sepia
- Solarize
- WB
- Normal/Spot metering
- flash Off/Auto/Red-eye
- Normal/Fine quality
- Xenon flash
- night mode
- low-light flash/noise/color behavior
- early Sony Ericsson JPEG processing

## GRAIN / NOISE REQUIREMENT

Noise must be internally generated.

It must NOT be:
- a static bitmap
- a supplied texture
- a reference JPG
- a duplicated AE native Noise effect

Use profile-specific models for:
- luma noise
- chroma noise
- sensor/read noise
- fixed-pattern tendencies where appropriate
- temporal noise
- ISO scaling
- shadow-dependent noise
- highlight-dependent noise
- denoising interaction
- grain/crystal structure for film

The grain/noise must move automatically through time.
Frame N and frame N+1 must not be identical when temporal noise is enabled.
The movement must be deterministic from frame/time + seed so renders remain reproducible.

## LENS MODEL

Where evidence supports it, implement:
- radial distortion
- barrel/pincushion distortion
- chromatic aberration
- corner falloff
- edge softness
- profile-specific optical strength

Do not call simple vignette + RGB offset a complete lens simulation.
If true resampling is required, implement it safely in the render pipeline.

## FLASH MODEL

Flash is not just a brightness slider.
Model, where evidence supports it:
- distance falloff
- center/axis illumination
- color-temperature bias
- foreground/background relationship
- exposure interaction
- red-eye behavior only where relevant
- Xenon vs LED character

Keep the model procedural.

## JPEG / ISP MODEL

Do not merely posterize/quantize RGB and call it JPEG.
Implement the strongest feasible deterministic approximation, preferably:
- RGB → YCbCr
- chroma subsampling where appropriate
- block transform
- quantization strength
- inverse transform
- ringing/blocking behavior
- camera-specific sharpening/NR before JPEG

If a true JPEG codec cannot be implemented safely inside the plugin, implement the best deterministic approximation and document that limitation. Never falsely claim a real codec if it is not one.

## REFERENCE IMAGES

The project contains 13 JPG references.
Use them for:
- visual calibration
- color distribution study
- noise study
- flash behavior study
- sharpening/edge study
- shadow/highlight study

NEVER use them as runtime image data.

Do not invent a one-to-one mapping between the 13 JPG filenames and the 23 profiles unless EXIF/source evidence proves it.

## UI

Current priority is rendering correctness.
Keep the native AE effect UI functional.
Organize controls logically.
Do not create extra AE layers or compositions.

A future custom tabbed UI is optional and must not compromise the native plugin build.

## PERFORMANCE

Use tiled/thread-safe processing where appropriate.
Avoid per-pixel expensive dynamic allocations.
Precompute profile constants.
Keep render deterministic.
Do not use unsafe global mutable noise state.

## BUILD REQUIREMENTS

Build target:
- Windows x64
- Visual Studio 2022 / MSVC
- Release
- `.aex`

The repository MUST build with:

`cmake -B build -G "Visual Studio 17 2022" -A x64`

then:

`cmake --build build --config Release --parallel`

The SDK path should default to:

`extern/AfterEffectsSDK`

PiPLtool must actually execute.

The final output must be a real:

`MAJEED_CameraEmulation.aex`

Do not say build succeeded unless the `.aex` file actually exists and the linker and PiPL steps succeeded.

## VALIDATION

Before claiming completion, verify:

1. `.aex` exists.
2. PiPLtool succeeds.
3. EffectMain is exported/registered correctly.
4. AE 23.2.1 can load the plugin without reinstall-plugin errors.
5. 8-bit works.
6. 16-bit works.
7. 32-bit float works.
8. SmartFX path works if advertised.
9. Temporal noise changes frame-to-frame.
10. Seed produces repeatable renders.
11. Profile switching actually changes rendering DNA.
12. No extra layers/solids/precomps are created.
13. No reference JPG is loaded during rendering.
14. No native AE noise/grain effect is secretly added.
15. All 23 profiles appear.
16. Film AE-1 does not use a digital sensor/JPEG model.
17. 808 PureView has a distinct oversampling stage.
18. H9 has distinct heavy NR/optical/JPEG behavior.
19. N73 has distinct watercolor-like low-light NR behavior.
20. T70 has distinct CA/high-ISO/sharpened-noise behavior.
21. FZ55 has its documented color-effect family.
22. 400D has Picture Style/Monochrome filter/toning behavior.
23. K800 has Xenon-flash + documented effect behavior.
24. W180 does not expose undocumented contrast/sharpness controls as if they were camera controls.
25. Wezein Excel 2 is not fabricated.

## FAILURE LOOP

If any build step fails:

- read the FULL compiler/linker/PiPL error
- identify the root cause
- edit the actual source/CMake/PiPL
- rebuild
- repeat

Do not stop after describing the error.
Do not give the user a checklist instead of fixing it.
Do not claim success from a partial build.

## FINAL REPORT

When finished, report:

- exact files changed
- exact SDK path used
- exact build command
- exact output `.aex` path
- whether PiPLtool succeeded
- whether 8/16/32-bpc paths were tested
- whether SmartFX was tested
- which camera profiles received documented behavior
- which behaviors are measured from public sample/review evidence
- which behaviors remain emulation coefficients rather than factory calibration
- any unresolved target, especially Wezein Excel 2

Never claim access to proprietary manufacturer ISP coefficients unless they are actually public and sourced.

## RESEARCH SOURCES ALREADY VERIFIED FOR THIS PASS

Use authoritative manuals and reputable technical reviews as implementation references, including:
- Adobe After Effects SDK examples, especially SDK_Noise
- Sony official DSC-W180 specifications/support/manuals
- Panasonic official TZ7 manual/support
- Canon 400D documentation
- Kodak FZ55 official manual
- Nokia N73/N82/N95 user guides
- Nokia 808 technical/review material
- Sony Ericsson K800 official white paper
- reputable technical camera reviews for measured image behavior

Do additional web research whenever a profile has missing evidence. Prefer manufacturer manuals/support pages and reputable technical camera reviews over SEO/speculation pages.

## ABSOLUTE RULE

Do not simplify the task into:

`camera = LUT + grain + sharpen`

That is explicitly rejected.

The deliverable is a native C++ After Effects camera-emulation engine with 23 materially different profiles and procedural, temporal, profile-specific rendering.
