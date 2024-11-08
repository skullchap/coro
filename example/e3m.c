#include <stdio.h>
#include <string.h>
#include <time.h>
#include "coro.h"
#include "corodef.h"

static char filetodownload[4096] = "hello";

void
sleep100ms()
{
    clock_t start = clock();
    clock_t s = CLOCKS_PER_SEC / 10;
    while (clock() - start < s)
	;
}

int
downloadfile(Coro *c, void *buf, int *n)
{
	COROBEGIN(c){
		for (*n = 0; *n < sizeof(filetodownload); *n += 128){  
			/* yield every downloaded 128 bytes, 
			instead of waiting for whole file to complete */ 
			memcpy(buf + *n, filetodownload + *n, 128);
			sleep100ms();
			YIELDLOOP(c);
		}
	}COROEND(c)

	return 0;
}

int
main()
{
	Coro c1;
	
	int n;
	char buf[4096];

	initcoro(&c1);
	while (!corodone(&c1)){
		downloadfile(&c1, buf, &n);
		fprintf(stderr, "\rDownloading: %d", n);
	}
	
	printf("\nFile Download Completed.\n");
	printf("Content: %s\n", buf);
	return 0;
}
