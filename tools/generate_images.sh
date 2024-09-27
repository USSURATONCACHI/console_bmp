#!/bin/bash

# Check for minimum arguments
if [ "$#" -lt 3 ]; then
    echo "Usage: $0 <pixel_formats_file> <output_directory> <input_files...>"
    exit 1
fi

# Assign variables
pixel_formats_file="$1"
output_directory="$2"
shift 2  # Shift to get the input files
input_files=("$@")

# Create output directory if it doesn't exist
mkdir -p "$output_directory"

# Read pixel formats into an array
mapfile -t pixel_formats < "$pixel_formats_file"

# Loop through each input file and pixel format
for input_file in "${input_files[@]}"; do
    # Generate non-paletted and paletted images for each pixel format
    for format in "${pixel_formats[@]}"; do
        output_file="${output_directory}/$(basename "$input_file").${format}.bmp"
        ffmpeg -i "$input_file" -pix_fmt "$format" "$output_file"
    done

    # Generate palette version
    palette_file="${output_directory}/$(basename "$input_file").palette.png"
    ffmpeg -i "$input_file" -vf "palettegen" "$palette_file"

    palette_output_file="${output_directory}/$(basename "$input_file").pal8.bmp"
    ffmpeg -i "$input_file" -i "$palette_file" -lavfi "paletteuse" -pix_fmt pal8 "$palette_output_file"
done
