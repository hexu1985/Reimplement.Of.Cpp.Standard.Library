import sys
import os
import shutil

cmake_file_root_dir="."

if len(sys.argv) == 2:
    cmake_file_root_dir = sys.argv[1]

if os.path.exists("build"):
    shutil.rmtree("build")

os.system("cmake -H{} -Bbuild".format(cmake_file_root_dir))
os.system("cmake --build build")
