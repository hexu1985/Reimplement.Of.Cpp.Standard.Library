import sys
import os
import shutil

build_dir = "build"

if len(sys.argv) == 2:
    build_dir = sys.argv[1]

if os.path.exists(build_dir):
    shutil.rmtree(build_dir)

