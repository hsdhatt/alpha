#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void set_alpha_argb(char *name, char *outname, int width, int height, int alpha)
{
	int h, w;
	FILE *fin, *fout;
	int *pixels;
	int offset = 0;

	fin = fopen(name, "r");
	fout = fopen(outname, "wb");

	pixels = malloc(width * height * sizeof(int));

	fread(pixels, sizeof(int), width * height, fin);

	for (h = 0; h < height; h++) {
		int *src =  &pixels[offset];

		for (w = 0;w < width; w++) {
			unsigned int *pix = &src[w];

			*pix = *pix & 0x00FFFFFF; /* clear the alpha */
			*pix = *pix | (alpha << 24); /* set 50 % alpha */
			//printf("%x %d %d\n", *pix, h, w);
		}
		offset += width;
	}

	fwrite(pixels, sizeof(int), width * height, fout);

	free(pixels);

	fclose(fin);
	fclose(fout);
}
