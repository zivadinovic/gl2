

set -e

# $1 $2 $3

GL2_SOURCE="$1"
ASM_OCTAL_OUT="$2"
ASM_UNARY_OUT="$3"

OBJ_FILE="${ASM_UNARY_OUT%.s}.o"
BIN_FILE="${ASM_UNARY_OUT%.s}"

./bin/gl2 "$GL2_SOURCE" > "$ASM_OCTAL_OUT"
echo ".globl main" > "$ASM_UNARY_OUT"

python3 conv2ascii.py "$ASM_OCTAL_OUT" >> "$ASM_UNARY_OUT"

as --64 "$ASM_UNARY_OUT" -o "$OBJ_FILE"

ld -m elf_x86_64    \
    "$OBJ_FILE"     \
    bin/grt2.o      \
    bin/mem.o       \
    bin/str.o       \
    bin/io.o        \
    bin/math.o      \
    bin/def.o       \
    bin/arr.o       \
    bin/casts.o     -o "$BIN_FILE"
