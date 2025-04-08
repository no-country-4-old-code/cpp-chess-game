import os
import sys

def get_input_arguments():
    number_of_arguments = len(sys.argv) - 1;

    if number_of_arguments != 1:
        print(f"ERROR: Wrong number of arguments ! (Expected 1 ; Got {number_of_arguments})")
        print("Usage: python <script-name.py> <directory>")
        sys.exit(1)
    
    directory = sys.argv[1]
    return directory



def find_source_files(directory):
    extensions = {'.h', '.hpp', '.c', '.cpp', '.cc'}
    files = {}

    for root, _, filenames in os.walk(directory):
        for filename in filenames:
            if any(filename.endswith(ext) for ext in extensions):
                files[filename] = os.path.join(root, filename)
    
    return files