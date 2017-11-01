#include <AOS-unix/multiboot.h>
//#include <AOS-unix/vga.h>
#include <AOS-unix/printk.h>
#include <AOS-unix/descriptor_tables.h>
//#include <AOS-unix/vga_graph.h>
//#include <AOS-unix/timer.h>

int start_kernel(uintptr_t mb_info_phys)
{ 
	monitor_init();
	/*const struct multiboot_info * const info = mb_info_phys;	
	monitor_init();
	monitor_clear();
	monitor_write_dec(info->mmap_length);
	printk("hello user %lu=%lu\n", info->mmap_length, info->mmap_addr);
	printk("\n12");*/
	gdt_idt_init();
	//printk("gdt_init\n");
	//monitor_put_char('a');
	//monitor_write("Your kernel run bro\n");
	//monitor_write("Your kernel run bro\n");
	//vbe_set(1200, 1200, 1200);
	//vbe_set(200, 200, 200);
	//printk("Hello 0x%lu-0x%lu", 52320390239, 42342);

	//VGA_init();
	//monitor_clear();
	//printk("Hello 0x%lu-0x%lu", 52320390239, 42342);
	//monitor_write("Your kernel run bro\n");
//	init_timer(100);
	//printk("timer init");
	asm volatile ("int $0x3");
	asm volatile ("int $0x4");
//	asm volatile ("int $0x12");
//	asm volatile ("int $0x13");
}
