import struct
import numpy as np
from PIL import Image
import sys

def load_image_from_binary(file_path):
    with open(file_path, "rb") as f:
        # Read the width and height (assuming they are stored as 4-byte integers each)
        width, height = struct.unpack('II', f.read(8))
        
        # Read the RGBA8 image data (width * height * 4 bytes)
        data = f.read(width * height * 4)
        
        # Convert the binary data to a numpy array
        img_array = np.frombuffer(data, dtype=np.uint8).reshape((height, width, 4))
        
        # Create an image using PIL
        img = Image.fromarray(img_array, 'RGBA')
        return img

def display_image(file_path):
    img = load_image_from_binary(file_path)
    img.show()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python view_image.py <binary_file>")
        sys.exit(1)
    
    # Get the binary file path from command-line arguments
    binary_file_path = sys.argv[1]
    
    # Display the image
    display_image(binary_file_path)
