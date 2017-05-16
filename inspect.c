#include <stdio.h>
#include <stdlib.h>

int arr[1024*768];

int main(int argc, char *argv[])
{
	FILE *fp;
	int x = atoi(argv[1]);
	int y = atoi(argv[2]);
	int val, r, g, b, a;

	fp = fopen("bg.rgba.random", "r");
	fread((void *)arr, sizeof(int), 1024*768, fp);

	val = arr[(y*x + x)];

	r = val & 0xFF;
	g = (val >> 8) & 0xFF;
	b = (val >> 16) & 0xFF;
	a = (val >> 24) & 0xFF;

	//printf("%d %d %d %d\n", r, g, b, a);
	printf("offset %d\n", (y*x + x)*4);
	printf("%x %x %x %x\n", r, g, b, a);
	
	fclose(fp);
	
	return 0;
}

