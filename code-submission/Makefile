CC=gcc
CFLAGS=-fsanitize=address -Wvla -Wall -Werror -std=gnu11 -lasan -s -Os -ffunction-sections -fdata-sections
LDFLAGS=-Wl,--gc-sections

# fill in all your make rules

vm_x2017: BinaryOperation.h BinaryOperation.c Interpreter.h Interpreter.c main.c BasicOperation.h BasicOperation.c Runtime.h Runtime.c
	cc $(CFLAGS) $(LDFLAGS) -DRUNTIME -o vm_x2017 BinaryOperation.h BinaryOperation.c Interpreter.h Interpreter.c BasicOperation.h BasicOperation.c Runtime.h Runtime.c main.c

objdump_x2017: BinaryOperation.h BinaryOperation.c Interpreter.h Interpreter.c main.c BasicOperation.h BasicOperation.c Runtime.h Runtime.c
	cc $(CFLAGS) $(LDFLAGS) -DINTERPRET -o objdump_x2017 BinaryOperation.h BinaryOperation.c Interpreter.h Interpreter.c BasicOperation.h BasicOperation.c Runtime.h Runtime.c main.c

tests: vm_x2017 objdump_x2017
	./test.sh

run_tests:vm_x2017 objdump_x2017
	./test.sh

clean:
	echo "clean"

