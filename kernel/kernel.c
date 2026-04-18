#include "./../drivers/screen.h"
#include "./../drivers/keyboard.h"
#include "./../cpu/isr.h"
#include "./../cpu/idt.h"
#include "./../memory/memory.h"
#include "./../memory/loader.h"
#include "./../process/process.h"

void test_program() {
    volatile char* vga = (volatile char*)0xB8000;
    vga[0] = 'O';
    vga[1] = 0x07;
    vga[2] = 'S';
    vga[3] = 0x07;

    while (1);
}

void main(){
    init_memory();
    clear_screen();
    isr_install();
    
    // Demonstracte interrupt
    // __asm__ __volatile__("int $2");
    // __asm__ __volatile__("int $4");
    // __asm__ __volatile__("int $15");
    // __asm__ __volatile__("int $16");

    // Demonstracte Timer Interrupt
    asm volatile("sti");
    // init_timer(50);
    init_keyboard();
    void* prog_addr = load((uchar_8*)test_program, 512);
    Process p;
    p.entry = prog_addr;

    void* stack = alloc_block();
    p.stack = (void*)((uint_32)stack + 4096);
    run(&p);
    while (1);
}