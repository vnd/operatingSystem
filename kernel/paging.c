#include "paging.h"
#include "kheap.h"

/* указатель на страницы ядра */
page_directory_t *kernel_directory = 0;

/* указатель на страницы текущего процесса */
page_directory_t *current_directory = 0;

u32_int *frames;
u32_int nframes;

extern u32_int placement_address;
extern heap_t *kheap;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

/* функция для установки бита в наборе bitset для фреймов */
static void set_frame(u32_int frame_addr)
{
   u32_int frame = frame_addr/0x1000;
   u32_int idx = INDEX_FROM_BIT(frame);
   u32_int off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

/* функция для очистки бита */
static void clear_frame(u32_int frame_addr)
{
    u32_int frame = frame_addr/0x1000;
    u32_int idx = INDEX_FROM_BIT(frame);
    u32_int off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

/* функция для проверки бита */
static u32_int test_frame(u32_int frame_addr)
{
    u32_int frame = frame_addr/0x1000;
    u32_int idx = INDEX_FROM_BIT(frame);
    u32_int off = OFFSET_FROM_BIT(frame);

    return (frames[idx] & (0x1 << off));
}

/* функция для поиска первого свободного фрейма */
static u32_int first_frame()
{
    u32_int i, j;
    for (i = 0; i < INDEX_FROM_BIT(nframes); i++) {
        if (frames[i] != 0xFFFFFFFF) {
            for (j = 0; j < 32; j++) {
                u32_int to_test = 0x1 << j;
                if (!(frames[i]&to_test)) {
                    return i*4*8+j;
                }
            }
        }
    }
}

/* функция выделяет фрейм */
void alloc_frame(page_t *page, s32_int is_kernel, s32_int is_writeable)
{
    if (page->frame != 0) {
        return;
    } else {
        u32_int idx = first_frame();
        if (idx == (u32_int) - 1) {
            /* kernel panic */
        }
        set_frame(idx * 0x1000);
        page->present = 1;
        page->rw = (is_writeable)?1:0;
        page->user = (is_kernel)?0:1;
        page->frame = idx;
    }
}

/* функция возвращает фрейм */
void free_frame(page_t *page)
{
    u32_int frame;
    if (!(frame=page->frame)) {
        return;
    } else {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

void paging_init()
{
    /**
    	размер физической памяти; мы предполагаем,
   		что размер равен 16 MB.
     **/
    u32_int mem_end_page = 0x1000000;
    
    nframes = mem_end_page / 0x1000;
    frames = (u32_int *)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));
    
    /* создадим директорию страниц */
    kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    current_directory = kernel_directory;

    int i = 0;
    for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        get_page(i, 1, kernel_directory);

   /**
		нам нужна карта идентичности (физический адрес = виртуальный адрес) с адреса
		0x0 до конца используемой памяти с тем, чтобы у нас к ним был прозрачный 
		доступ как если бы страничная организация памяти не использовалась.
		ЗАМЕТЬТЕ, что мы преднамеренно используем цикл while.	
   		Внутри тела цикла мы фактически изменяем адрес placement_address
		с помощью вызова функции kmalloc(). Цикл while используется здесь, т.к. выход
		из цикла динамически, а не один раз после запуска цикла
    **/
    s32_int i = 0;
    while (i < placement_address + 0x1000)
    {
    	/*  код ядра можно читать из пользовательского режима, но нельзя в него записывать */
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);

    /* регистриуруем обработчик для page fault */
    register_interrupt_handler(14, page_fault);

    /* включаем страничную организацию памяти */
    switch_page_directory(kernel_directory);

        // The size of physical memory. For the moment we 
    // assume it is 16MB big.
    u32int mem_end_page = 0x1000000;
    
    nframes = mem_end_page / 0x1000;
    frames = (u32int*)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));
    
    // Let's make a page directory.
    kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    memset(kernel_directory, 0, sizeof(page_directory_t));
    current_directory = kernel_directory;

    // Map some pages in the kernel heap area.
    // Here we call get_page but not alloc_frame. This causes page_table_t's 
    // to be created where necessary. We can't allocate frames yet because they
    // they need to be identity mapped first below, and yet we can't increase
    // placement_address between identity mapping and enabling the heap!
    int i = 0;
    for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        get_page(i, 1, kernel_directory);

    // We need to identity map (phys addr = virt addr) from
    // 0x0 to the end of used memory, so we can access this
    // transparently, as if paging wasn't enabled.
    // NOTE that we use a while loop here deliberately.
    // inside the loop body we actually change placement_address
    // by calling kmalloc(). A while loop causes this to be
    // computed on-the-fly rather than once at the start.
    // Allocate a lil' bit extra so the kernel heap can be
    // initialised properly.
    i = 0;
    while (i < placement_address+0x1000)
    {
        // Kernel code is readable but not writeable from userspace.
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    // Now allocate those pages we mapped earlier.
    for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);

    // Before we enable paging, we must register our page fault handler.
    register_interrupt_handler(14, page_fault);

    // Now, enable paging!
    switch_page_directory(kernel_directory);

    // Initialise the kernel heap.
    kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->tables_physical));
    u32_int cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
}

page_t *get_page(u32_int address, s32_int make, page_directory_t *dir)
{
    /* превращаем адрес в индекс */
    address /= 0x1000;
    /* находим таблицу страниц, в которой есть этот адрес */
    u32_int table_idx = address / 1024;
    if (dir->tables[table_idx])	{
        return &dir->tables[table_idx]->pages[address%1024];
    } else if(make) {
        u32_int tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
        dir->tables_physical[table_idx] = tmp | 0x7;

        return &dir->tables[table_idx]->pages[address%1024];
    } else {
        return 0;
    }
}

void page_fault(registers_t regs)
{
    /* адрес прерывания запоминается в cr2 */
    u32_int faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    
    /* код ошибки */
    u32_int present   = !(regs.err_code & 0x1); // страница отсутствует
    u32_int rw = regs.err_code & 0x2;           // операция записи?
    u32_int us = regs.err_code & 0x4;           // процессор находится в user-space
    u32_int reserved = regs.err_code & 0x8;     // Переписаны зарезервированные биты?
    u32_int id = regs.err_code & 0x10;          // проблема во время выборки инструкций

   	/* выводим на экран сообщение об ошибке */
    screen_write("Page fault! ( ");
    if (present) {screen_write("present ");}
    if (rw) {screen_write("read-only ");}
    if (us) {screen_write("user-mode ");}
    if (reserved) {screen_write("reserved ");}
    screen_write(") at 0x");
    screen_write_hex(faulting_address);
    screen_write("\n");
    PANIC("Page fault");
}
