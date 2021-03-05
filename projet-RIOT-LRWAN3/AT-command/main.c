#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "at.h"
#include "shell.h"
#include "xtimer.h"


int main(void)
{
    xtimer_sleep(5);
    puts("Hello from RIOT!");
    
    at_dev_t *dev = NULL;
    char *buf;
    buf = malloc(4*sizeof(char));

    
    int init = at_dev_init( dev , UART_DEV(0) , 433, buf, 4);
    puts("test");
    if(init == UART_OK) {
        puts("OK");
    }
    else{
        puts("NO OK");
    }
    at_send_cmd_wait_ok	(dev, "AT\n\r", 0);


    

    return 0;
}
