

set -e

CXX="g++"
CXXFLAGS="-Wall -Werror -Wextra -Wpedantic -Wshadow -O2 -std=c++20"
ASFLAGS="--64"

HOST_BIN="bin/host"
SRC_MAIN="src/main.cc"

RUNTIME_OBJS="bin/grt2.o bin/mem.o bin/str.o bin/io.o bin/math.o bin/def.o bin/arr.o bin/casts.o"

mkdir -p bin out

echo "==========================================="
echo " compiling cpp source file "
echo "==========================================="
$CXX $CXXFLAGS $SRC_MAIN -o $HOST_BIN

echo "==========================================="
echo " compiling gl2 runtime library "
echo "==========================================="

echo "processing def.gl2..."
echo -e ".globl isprint\n.globl isspace\n.globl isdigit\n.globl isalpha\n.globl isalnum\n.globl isident\n.globl ischar\n.globl islower\n.globl isupper\n.globl tolower\n.globl toupper\n.globl exit" > out/def.s
./$HOST_BIN in/lib/def.gl2 >> out/def.s
as $ASFLAGS out/def.s -o bin/def.o

echo "processing arr.gl2..."
echo -e ".globl arrinit\n.globl arradd\n.globl arrrem\n.globl arrclean\n.globl arrfind" > out/arr.s
./$HOST_BIN in/lib/arr.gl2 >> out/arr.s
as $ASFLAGS out/arr.s -o bin/arr.o

echo "processing casts.gl2..."
echo -e ".globl int\n.globl string" > out/casts.s
./$HOST_BIN in/lib/casts.gl2 >> out/casts.s
as $ASFLAGS out/casts.s -o bin/casts.o

echo "processing io.gl2..."
echo -e ".globl read\n.globl write\n.globl inputn\n.globl printn\n.globl errorn\n.globl inputf\n.globl printf\n.globl errorf\n.globl input\n.globl print\n.globl error\n.globl open\n.globl close\n.globl lseek" > out/io.s
./$HOST_BIN in/lib/io.gl2 >> out/io.s
as $ASFLAGS out/io.s -o bin/io.o

echo "processing math.gl2..."
echo -e ".globl madd\n.globl msub\n.globl mmul\n.globl mdiv\n.globl mmod" > out/math.s
./$HOST_BIN in/lib/math.gl2 >> out/math.s
as $ASFLAGS out/math.s -o bin/math.o

echo "processing str.gl2..."
echo -e ".globl strfmtes\n.globl strfmt\n.globl strlen\n.globl strbuf\n.globl strcat\n.globl strcpy\n.globl strdup\n.globl strcmp" > out/str.s
./$HOST_BIN in/lib/str.gl2 >> out/str.s
as $ASFLAGS out/str.s -o bin/str.o

echo "processing mem.gl2..."
echo -e ".globl memalloc\n.globl memrealloc\n.globl memfree" > out/mem.s
./$HOST_BIN in/lib/mem.gl2 >> out/mem.s
as $ASFLAGS out/mem.s -o bin/mem.o

as $ASFLAGS grt2/main.s -o bin/grt2.o

echo "==========================================="
echo " building generation 1 (client1) "
echo "==========================================="
echo "Processing main1.gl2..."
echo ".globl main" > out/main.s
./$HOST_BIN in/main.gl2 >> out/main.s
as $ASFLAGS out/main.s -o bin/main.o

echo "linking client1..."
ld -m elf_x86_64 bin/main.o $RUNTIME_OBJS -o bin/client1
echo "++ generation 1 built successfully"

echo "==========================================="
echo " bootstrapping gl2 compiler "
echo "==========================================="
echo "compiling gl2 using client1..."
./bin/client1 in/main.gl2 > out/octal__gl2.s

echo "running conv2ascii..."
echo ".globl main" > out/gl2.s
python3 conv2ascii.py "out/octal__gl2.s" >> out/gl2.s

echo "assembling and linking gl2..."
as $ASFLAGS out/gl2.s -o bin/gl2.o
ld -m elf_x86_64 bin/gl2.o $RUNTIME_OBJS -o bin/gl2

echo "==========================================="
echo " success; bin/gl2 created. "
echo "==========================================="
