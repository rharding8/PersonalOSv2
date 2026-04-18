
# $@ = target file
# $< = first dependency
# $^ = all dependencies
ADDRESS :=0x1000
BOOT_DIR:=boot
KERNEL_DIR:=kernel
DRIVER_DIR:=drivers
CPU_DIR:=cpu
LIB_DIR	:=libc
MEM_DIR :=memory
PROC_DIR :=process

OS_IMAGE:=os.bin
C_SRC:= $(wildcard $(KERNEL_DIR)/*.c $(DRIVER_DIR)/*.c $(CPU_DIR)/*.c $(LIB_DIR)/*.c $(MEM_DIR)/*.c $(PROC_DIR)/*.c)
C_HEADERS := $(wildcard $(KERNEL_DIR)/*.h $(DRIVER_DIR)/*.h $(CPU_DIR)/*.h $(LIB_DIR)/*.h $(MEM_DIR)/*.h $(PROC_DIR)/*.h)
SRC_ASM:=$(wildcard $(BOOT_DIR)/*.asm)
OBJ:= ${C_SRC:%.c=%.o $(CPU_DIR)/interrupt.o}

# GLD and GCC indicate our cross compiler as we installed them with prefix i386-elf 
# to prevent conflict with system gcc and other softwares
ASSM:=nasm
DISASSM := ndisasm
GLD := i386-elf-ld
GCC := i386-elf-gcc
GDB := gdb
C_FLAGS:= -ffreestanding -g



all: run
$(OS_IMAGE): $(BOOT_DIR)/bootsect.bin kernel.bin
	cat $^ > $@

kernel.bin: $(BOOT_DIR)/kernel_entry.o $(OBJ)
	@echo $(OBJ)
	$(GLD) -o $@ -Ttext $(ADDRESS) $^ --oformat binary

kernel.elf:$(BOOT_DIR)/kernel_entry.o $(OBJ)
	$(GLD) -o $@ -Ttext $(ADDRESS) $^


%.o: %.c $(C_HEADERS)
	$(GCC) $(C_FLAGS) -c $< -o $@

%.o: %.asm 
	$(ASSM) $< -f elf -o $@

%.bin: %.asm
	$(ASSM) $< -f bin -o $@

run: $(OS_IMAGE)
	qemu-system-i386 -fda $(OS_IMAGE)
	
debug: $(OS_IMAGE) kernel.elf
	qemu-system-i386 -s -fda $(OS_IMAGE) -d guest_errors,int &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

clean:
	rm -rf $(OS_IMAGE) *.bin *.dis *.o *.elf
	rm -rf $(KERNEL_DIR)/*.o $(BOOT_DIR)/*.bin $(DRIVER_DIR)/*.o $(BOOT_DIR)/*.o $(CPU_DIR)/*.o $(LIB_DIR)/*.bin $(CPU_DIR)/*.o $(LIB_DIR)/*.o $(MEM_DIR)/*.o $(PROC_DIR)/*.o

# .PHONY: all

