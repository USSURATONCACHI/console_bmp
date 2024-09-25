import struct
import sys
import numpy as np

def create_image_binary_dump(width, height, output_path):
    # Create an empty numpy array for RGBA8 values
    image_data = np.zeros((height, width, 4), dtype=np.uint8)
    
    # Fill the array based on the formula: r = x % 256, g = y % 256, b = 0, a = 255
    for y in range(height):
        for x in range(width):
            image_data[y, x] = [x % 256, y % 256, 0, 255]  # [R, G, B, A]
    
    # Flatten the data (convert to a 1D array of bytes)
    flattened_data = image_data.tobytes()
    
    # Write width, height and pixel data into a binary file
    with open(output_path, "wb") as f:
        # Write width and height as 4-byte unsigned integers (uint32)
        f.write(struct.pack('II', width, height))
        
        # Write the RGBA8 data
        f.write(flattened_data)
    
    print(f"Binary dump created: {output_path}")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python script.py <width> <height> <output_file>")
        sys.exit(1)
    
    # Get width, height, and output file from command-line arguments
    width = int(sys.argv[1])
    height = int(sys.argv[2])
    output_path = sys.argv[3]
    
    # Create the image binary dump
    create_image_binary_dump(width, height, output_path)
