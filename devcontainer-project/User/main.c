#include <stdio.h>
#include <string.h>
#include "debug.h"

int main() {
	USART_Printf_Init(115200);
   	while (1) printf("hello\n\n");
   	return 0;
}
