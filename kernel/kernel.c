#include "../cpu/isr.h"
#include "../drivers/monitor.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../cpu/heap.h"

/* start kernel */
void main(void)
{
 	clear_screen();
 	isr_init();
 	irq_init();

 	k_print("HEllo");

	init_pmm(0x20000000);
 	init_vmm();
 	//init_heap();

 	//u32_int *ptr = (u32_int*)0xA0000000;
   	//u32_int do_page_fault = *ptr;
}

void user_input(s8_int* input)
{
    if (strcmp(input, "END") == 0) {
        k_print("Stopping the CPU. Bye!\n");
        __asm__ __volatile__("hlt");
    }
    k_print("You said: ");
    k_print(input);
    k_print("\n> ");
}