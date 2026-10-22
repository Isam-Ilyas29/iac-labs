#!/bin/bash
# usage: doit <top module>   (run from any labN/taskM folder)
set -e
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

NAME=${1%.sv}
[ -z "$NAME" ] && { echo "usage: doit <top module>"; exit 1; }

# Vbuddy tasks: copy the shared config in if missing, attach the USB port
if [ -f vbuddy.cpp ]; then
    [ -f vbuddy.cfg ] || cp "$ROOT/vbuddy.cfg" .
    ~/Documents/iac/lab0-devtools/tools/attach_usb.sh
fi

rm -rf obj_dir
rm -f *.vcd

verilator -Wall --cc --trace *.sv --top-module ${NAME} --exe ${NAME}_tb.cpp
make -j -C obj_dir/ -f V${NAME}.mk V${NAME}
obj_dir/V${NAME}
