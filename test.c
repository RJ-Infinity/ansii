#include <stdio.h>

int main()
{
	printf("%c[%dmHello World! With Colour!!%c[%dm\n", 0x1B, 32, 0x1B, 0);
}