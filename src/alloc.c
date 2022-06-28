#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "alloc.h"
#include "print.h"

extern uint32_t HEAP_START;
extern uint32_t HEAP_SIZE;
extern uint32_t TEXT_START;
extern uint32_t TEXT_END;
extern uint32_t DATA_START;
extern uint32_t DATA_END;
extern uint32_t RODATA_START;
extern uint32_t RODATA_END;
extern uint32_t BSS_START;
extern uint32_t BSS_END;
extern uint32_t KERNEL_STACK_START;
extern uint32_t KERNEL_STACK_END;

uint32_t num_reserved_pages = 512;
uint32_t num_pages;
uint32_t alloc_start;
uint32_t alloc_end;


enum PAGE_BITS {
    PAGE_EMPTY = 0,
    PAGE_TAKEN = 1,
    PAGE_LAST = 2,
};

struct free_page {
    struct free_page *next;
};

struct page {
    uint8_t flags;
};

static uint32_t align_page(uint32_t addr, uint32_t order)
{
    uint32_t mask = (1 << order) - 1;

    return (addr + mask) & ~mask;
}

static uint32_t get_num_pages()
{
    return (HEAP_SIZE / PAGE_SIZE) - num_reserved_pages;
}

/*
 *    ----------------------------------- HEAP_START
 *      Reservation for for page structs
 *    ----------------------------------- alloc_start
 * 
 *      Pages (each page is 4096 bytes)) 
 *    ----------------------------------- HEAP_END (alloc_end)
 * 
 */
void page_init()
{
    struct page *pages = (struct page *)HEAP_START;
    int num_pages = get_num_pages();

    for (int i; i < num_pages; i++) {
        pages[i].flags = PAGE_EMPTY;
    }

    alloc_start = align_page(HEAP_START + num_reserved_pages * PAGE_SIZE, PAGE_ORDER);
    alloc_end = alloc_start + num_pages * PAGE_SIZE;

    printk("Heap: %x - %x\n", HEAP_START, HEAP_START + HEAP_SIZE);
    printk("Alloc: %x - %x\n", alloc_start, alloc_end);
    printk("Number of pages: %u\n", num_pages);
}

void *kmalloc(uint32_t size)
{
    uint32_t num_requested_pages = align_page(size, PAGE_ORDER) / PAGE_SIZE;
    uint32_t num_pages = get_num_pages();
    struct page *pages = (struct page *)HEAP_START;

    for (int i = 0; i < num_pages - num_requested_pages; i++) {
        bool found = false;

        if (pages[i].flags != PAGE_TAKEN) {
            found = true;
            for (int j = 0; j < num_requested_pages; j++) {
                if (pages[i + j].flags == PAGE_TAKEN) {
                    found = false;
                    break;
                }
            }
        }

        if (found) {
            for (int j = 0; j < num_requested_pages; j++) {
                pages[i + j].flags = PAGE_TAKEN;
            }

            // Mark last page as last
            pages[i + num_requested_pages - 1].flags |= PAGE_LAST;

            return (void *)(alloc_start + i * PAGE_SIZE);
        }
    }
    return NULL;
}

void kfree(void *ptr)
{
    uint32_t num_freed_pages = ((uint32_t)ptr - alloc_start) / PAGE_SIZE;
    struct page *pages = (struct page *)(HEAP_START + num_freed_pages);

    while (pages->flags & PAGE_TAKEN && !(pages->flags & PAGE_LAST)) {
        pages->flags = PAGE_EMPTY;
        pages++;
    }

    pages->flags = PAGE_EMPTY;
}
