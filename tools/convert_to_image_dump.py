from PIL import Image
import struct
import sys

def generate_binary_dump(input_image_path, output_file_path):
    # Open the image file
    with Image.open(input_image_path) as img:
        # Ensure the image is in RGBA mode (4 channels)
        img = img.convert('RGBA')

        # Get image dimensions
        width, height = img.size
        
        # Prepare the binary data
        binary_data = bytearray()
        
        # Add width and height (4 bytes each)
        binary_data.extend(struct.pack('I', width))  # Width
        binary_data.extend(struct.pack('I', height))  # Height
        
        # Get pixel data (row by row)
        for y in range(height):
            for x in range(width):
                r, g, b, a = img.getpixel((x, y))
                # Append the pixel in r8g8b8a8 format (4 bytes)
                binary_data.extend(struct.pack('BBBB', r, g, b, a))

        # Write the binary data to the output file
        with open(output_file_path, 'wb') as output_file:
            output_file.write(binary_data)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python binary_image_dump.py <input_image_path> <output_file_path>")
        sys.exit(1)

    input_image_path = sys.argv[1]
    output_file_path = sys.argv[2]
    
    generate_binary_dump(input_image_path, output_file_path)
