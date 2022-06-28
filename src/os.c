#include <stdint.h>
#include "uart.h"
#include "print.h"
#include "alloc.h"

int os_init()
{
	uart_init();
	page_init();
}


int os_main(void)
{
	os_init();

	printk("Hello World! OS!\n");

	while (1) {
		;
	}
	
	return 0;
}