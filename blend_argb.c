#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int populate_buffer(FILE *fp, void **arr, int width, int height, int size)
{
	*arr = malloc(width * height * size);
	if (arr == NULL) {
		printf("couldn't allocate memory\n");
		return -1;
	}

	return fread(*arr, size, width * height, fp);
}

unsigned int blend_pixel_argb_argb(int *fg, int *bg)
{
	float fg_a, fg_r, fg_g, fg_b;
	float bg_a, bg_r, bg_g, bg_b;
	unsigned int new_a, new_r, new_g, new_b, new_pixel = 0;

	fg_b = (*fg) & 0xFF;
	fg_g = (*fg >> 8) & 0xFF;
	fg_r = (*fg >> 16) & 0xFF;
	fg_a = (*fg >> 24) & 0xFF;
	fg_a /= 255;
	
	bg_b = (*bg) & 0xFF;
	bg_g = (*bg >> 8) & 0xFF;
	bg_r = (*bg >> 16) & 0xFF;
	bg_a = (*bg >> 24) & 0xFF;
	bg_a /= 255;

	new_r = (float)(fg_r * fg_a) + (float)(1 - fg_a) * bg_a * bg_r;
	new_g = (float)(fg_g * fg_a) + (float)(1 - fg_a) * bg_a * bg_g;
	new_b = (float)(fg_b * fg_a) + (float)(1 - fg_a) * bg_a * bg_b;

	new_a = (fg_a + ((1 - fg_a) * bg_a)) * 0xFF;	

	new_pixel = (new_b ) | (new_g << 8 ) | (new_r << 16) | (new_a << 24);

	return new_pixel;
}

int blend_argb_argb(char *fgname, char *bgname, char *outname, int width, int height)
{
	FILE *fg, *bg, *fout;
	int h, w, offset = 0;
	int *fgpix, *bgpix, *newpix;

	fg = fopen(fgname, "r");
	if (fg == NULL) {
		printf("Couldn't open %s\n", fgname);
		return -1;
	}

	bg = fopen(bgname, "r");
	if (fg == NULL) {
		printf("Couldn't open %s\n", bgname);
		return -1;
	}

	fout = fopen(outname, "wb");
	if (fg == NULL) {
		printf("Couldn't open %s\n", outname);
		return -1;
	}

	if (populate_buffer(fg, (void **)&fgpix, width, height, sizeof(int)) < 0)
		return -1;

	if (populate_buffer(bg, (void**)&bgpix, width, height, sizeof(int)) < 0)
		return -1;

	newpix = malloc(width * height * sizeof(int));

	for (h = 0; h < height; h++) {
		int *fg = &fgpix[offset];
		int *bg = &bgpix[offset];
		int *new = &newpix[offset];

		for (w = 0; w < width; w++) {
			int *fgpixel = &fg[w];
			int *bgpixel = &bg[w];
			int *newpixel = &new[w];
			*newpixel = blend_pixel_argb_argb(fgpixel, bgpixel);
		}
		offset += width;
	}

	fwrite(newpix, sizeof(int), width * height, fout);

	free(newpix);
	free(fgpix);
	free(bgpix);

	fclose(fout);
	fclose(fg);
	fclose(bg);

	return 0;
}
