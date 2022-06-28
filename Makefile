OBJDUMP=riscv64-unknown-elf-objdump
CC=riscv64-unknown-elf-gcc
CFLAGS=-nostdlib -fno-builtin -mcmodel=medany -march=rv32ima -mabi=ilp32 -Iinclude

QEMU=qemu-system-riscv32
QFLAGS=-nographic -smp 4 -machine virt -bios none

all: os.elf

os.elf: src/asm/boot.s src/asm/memory.s src/os.c src/uart.c src/print.c src/alloc.c
	$(CC) $(CFLAGS) -T src/lds/os.ld -o os.elf $^

qemu: $(TARGET)
	@qemu-system-riscv32 -M ? | grep virt >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	$(QEMU) $(QFLAGS) -kernel os.elf

clean:
	rm -f *.elf