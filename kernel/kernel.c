#include "./../drivers/screen.h"
#include "./../drivers/keyboard.h"
#include "./../cpu/isr.h"
#include "./../cpu/idt.h"
#include "./../cpu/vga_port.h"
#include "./../memory/memory.h"
#include "./../memory/loader.h"
#include "./../process/process.h"
#include "./shell.h"

void test_program() {
    volatile char* vga = (volatile char*)0xB8000;
    vga[0] = 'O';
    vga[1] = 0x07;
    vga[2] = 'S';
    vga[3] = 0x07;

    while (1);
}

void main(){
    isr_install(); // Install ISRs and IRQs
    int exit_flag = 0; // Set shutdown flag to 0
    while (!exit_flag) {
        init_memory();
        heap_init();
        clear_screen();

        asm volatile("sti"); // Enable interrupts
        init_keyboard();

        // void* prog_addr = load((uchar_8*)test_program, 512);
        // Process p;
        // p.entry = prog_addr;

        // void* stack = alloc_block();
        // p.stack = (void*)((uint_32)stack + 4096);
        // run(&p);

        exit_flag = shell_run(); // Run the shell and get the exit flag when it exits
    }

    port_word_out(0x604, 0x2000); // Send shutdown command to ACPI
}