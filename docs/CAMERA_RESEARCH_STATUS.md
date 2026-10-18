# Camera research and calibration status

Legend:
- **DOCUMENTED**: control/feature confirmed by a manual or manufacturer/reputable technical source.
- **REFERENCE**: behavior inferred from the user's supplied images.
- **CALIBRATION**: requires side-by-side sample-image measurement; do not present as a measured physical specification.

## Digital cameras

| Target | Documented controls/features | Calibration targets |
|---|---|---|
| Panasonic TZ7/ZS3 | REC menu includes picture size, quality/aspect, Intelligent ISO, sensitivity, WB, metering, i.Exposure, color mode; scene modes include Film Grain/Pinhole/etc. | color matrix, WB bias, ISP denoise/sharpen, JPEG, scene/filter curves |
| Sony W180 | Sony provides model-specific manuals/support; WB behavior is documented. | color mode/effects, JPEG, flash, noise and sharpening from original samples |
| Sony H7/H9 | camera-specific scene/color/WB behavior to be verified from exact model manual | H7 vs H9 should not be collapsed if their output differs |
| Sony Handycam | model family varies; scene selection/picture effects/WB are model dependent | exact Handycam model must be selected for literal reproduction |
| Canon PowerShot A-series CCD | My Colors / scene / WB vary by A-series model | exact A-model should be fixed before claiming literal emulation |
| Canon AE-1 | film camera; no digital ISP | film stock, lens, metering, development, scan, grain |
| Sony NEX-5 | Creative Style, Picture Effect, WB and DRO/Auto HDR are documented on NEX family manuals | exact NEX-5 firmware/image processor behavior |
| Canon EOS 400D/XTi | Picture Styles Standard/Portrait/Landscape/Neutral/Faithful/Monochrome; adjustable sharpness/contrast/color saturation/tone; monochrome filter/toning effects | JPEG tone curve and color matrices |
| Sony a6000 | Creative Style/Picture Effect/WB/DRO/Auto HDR documented in Sony manuals | exact default JPEG rendering |
| Sony DSC-T70 | model manuals available; exact effect/WB menu needs extraction from operating guide | color/JPEG/noise/flash |
| Kodak PIXPRO FZ55 | WB options, Color Effect Normal/Vivid/B&W/Sepia/Dreamy/Vignetting/Salon, HDR and scene controls documented | FZ55 output/noise/sharpen/JPEG |
| Olympus mju 5000 | WB, ISO, scene modes, Shadow Adjustment, image quality/compression, panorama documented | color/JPEG/noise and scene behavior |
| Wezein Excel 2 | **UNRESOLVED IDENTIFICATION** | user must confirm exact model/brand or provide metadata/manual; supplied reference images remain calibration evidence |

## Phone cameras

| Target | Documented controls/features | Calibration targets |
|---|---|---|
| Nokia 7610 | legacy camera-phone pipeline | sample set needed |
| Nokia N73 | camera-specific scene/WB/colour tone/sharpness behavior | low-light denoise, sharpening, color |
| Nokia N82 | flash, colour tone, WB, exposure compensation, sharpness, contrast, light sensitivity; settings are camera/mode specific | Xenon flash + ISP/JPEG/noise |
| Nokia N95 | legacy N-series camera pipeline | exact firmware/sample set |
| Nokia N93 | camera/video-oriented N-series pipeline | video vs photo processing |
| Nokia N86 | 8MP N-series pipeline; sharpening/noise behavior needs sample measurement | noise reduction and edge halos |
| Nokia N8 | larger sensor + Xenon/Zeiss generation | JPEG color/noise/flash |
| Nokia 808 PureView | 41MP PureView oversampling is a distinct pipeline | oversampling/noise/detail/color |
| Sony Ericsson K750i | early 2MP autofocus phone camera | flash/noise/JPEG/color |
| Sony Ericsson K800i | early 3.2MP autofocus phone camera with Xenon flash; native color effects include B&W/negative/sepia/solarize in documentation | flash, JPEG, low-light noise, color effects |

## User reference images

The 13 supplied JPGs were visually inspected as a set. They contain strong evidence of:
- direct-flash behavior
- warm/amber indoor rendering
- green/cyan shadow casts
- compact-camera noise
- sharpening/edge treatment
- deep blacks and rapid flash falloff
- beach/daylight color response
- motion/low-light behavior

**Important:** filenames were not mapped one-to-one to the 13 camera names in the conversation, so no filename-to-camera mapping is assumed. The Wezein Excel 2 target also needs exact identity confirmation before we claim a literal camera match.
