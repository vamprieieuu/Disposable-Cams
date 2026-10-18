# Quantitative reference-image pass

The supplied 13 JPGs were batch-inspected for dimensions, mean RGB, RGB standard deviation, luminance percentiles and mean saturation. These numbers are calibration clues only; without knowing which camera produced which image, they must not be treated as per-camera coefficients.

Notable clusters:
- IMG_3508 and IMG_3510 are strongly warm/high-saturation indoor frames.
- IMG_3504/3505/3511/3515 show strong flash/party-like contrast.
- IMG_3503 and IMG_3506 are darker low-light references with cooler/greenish behavior.
- IMG_3513 is a cool, low-saturation daylight/beach reference.
- IMG_3514 is a greener outdoor reference.
- IMG_3509 contains a very bright scene and camera/date overlay behavior worth excluding from color-stat calibration.
- IMG_3512 is a larger-resolution indoor/flash frame.

See `REFERENCE_IMAGE_ANALYSIS.json` for the raw measurements.
