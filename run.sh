cmake --build build && \
./build/drawBmp $1 && \
python3 tools/view_dump_rgba8.py imgdump.bin