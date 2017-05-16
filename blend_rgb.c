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

uint16_t blend_pixel_argb_rgb565(int *fg, uint16_t *bg)
{
	float fg_a, fg_r, fg_g, fg_b;
	float bg_a, bg_r, bg_g, bg_b;
	uint32_t new_a, new_r, new_g, new_b;
	uint32_t new_pixel = 0;

	fg_b = (*fg) & 0xFF;
	fg_g = (*fg >> 8) & 0xFF;
	fg_r = (*fg >> 16) & 0xFF;
	fg_a = (*fg >> 24) & 0xFF;
	fg_a /= 255;

	/*scale down the fg channels */
	fg_b = ((int)fg_b >> 3) & 0x1F;
	fg_g = ((int)fg_g >> 2) & 0x3F;
	fg_r = ((int)fg_r >> 3) & 0x1F;

	bg_b = (*bg) & 0x1F;
	bg_g = (*bg >> 5) & 0x3F;
	bg_r = (*bg >> 11) & 0x1F;

	new_r = (float)(fg_r * fg_a) + (float)(1 - fg_a) * bg_r;
	new_g = (float)(fg_g * fg_a) + (float)(1 - fg_a) * bg_g;
	new_b = (float)(fg_b * fg_a) + (float)(1 - fg_a) * bg_b;

	new_r = new_r & 0x1F;
	new_g = new_g & 0x3F;
	new_b = new_b & 0x1F;

	new_pixel = new_b | (new_g << 5) | (new_r << 11);

	return new_pixel;
}

int blend_argb_rgb565(char *argb, char *rgb, char *outname, int width, int height)
{
	FILE *fg, *bg, *fout;
	int h, w, offset = 0;
	int *fgpix;
	uint16_t *bgpix, *newpix;

	fg = fopen(argb, "r");
	if (fg == NULL) {
		printf("Couldn't open %s\n", argb);
		return -1;
	}

	bg = fopen(rgb, "r");
	if (fg == NULL) {
		printf("Couldn't open %s\n", rgb);
		return -1;
	}

	fout = fopen(outname, "wb");
	if (fg == NULL) {
		printf("Couldn't open %s\n", outname);
		return -1;
	}

	if (populate_buffer(fg, (void **)&fgpix, width, height, sizeof(int)) < 0)
		return -1;

	if (populate_buffer(bg, (void **)&bgpix, width, height, sizeof(uint16_t)) < 0)
		return -1;

	newpix = malloc(width * height * sizeof(uint16_t));

	for (h = 0; h < height; h++) {
		int *fg = &fgpix[offset];
		uint16_t *bg = &bgpix[offset];
		uint16_t *new = &newpix[offset];

		for (w = 0; w < width; w++) {
			int *fgpixel = &fg[w];
			uint16_t *bgpixel = &bg[w];
			uint16_t *newpixel = &new[w];
			*newpixel = blend_pixel_argb_rgb565(fgpixel, bgpixel);
		}
		offset += width;
	}

	fwrite(newpix, sizeof(uint16_t), width * height, fout);

	free(newpix);
	free(fgpix);
	free(bgpix);

	fclose(fout);
	fclose(fg);
	fclose(bg);

	return 0;
}

uint16_t blend_pixel_rgb565_rgb565(uint16_t *fg, uint16_t *bg, int alpha)
{
	float fg_a, fg_r, fg_g, fg_b;
	float bg_r, bg_g, bg_b;
	uint32_t new_a, new_r, new_g, new_b;
	uint16_t new_pixel = 0;

	fg_b = (*fg) & 0x1F;
	fg_g = (*fg >> 5) & 0x3F;
	fg_r = (*fg >> 11) & 0x1F;
	fg_a = (float)alpha/255;

	bg_b = (*bg) & 0x1F;
	bg_g = (*bg >> 5) & 0x3F;
	bg_r = (*bg >> 11) & 0x1F;

	new_r = (float)(fg_r * fg_a) + (float)(1 - fg_a) * bg_r;
	new_g = (float)(fg_g * fg_a) + (float)(1 - fg_a) * bg_g;
	new_b = (float)(fg_b * fg_a) + (float)(1 - fg_a) * bg_b;

	new_r = new_r & 0x1F;
	new_g = new_g & 0x3F;
	new_b = new_b & 0x1F;

	new_pixel = new_b | (new_g << 5) | (new_r << 11);

	return new_pixel;
}

int blend_rgb565_rgb565(char *argb, char *rgb, char *outname, int width, int height, int alpha)
{
	FILE *fg, *bg, *fout;
	int h, w, offset = 0;
	uint16_t *fgpix, *bgpix, *newpix;

	fg = fopen(argb, "r");
	if (fg == NULL) {
		printf("Couldn't open %s\n", argb);
		return -1;
	}

	bg = fopen(rgb, "r");
	if (fg == NULL) {
		printf("Couldn't open %s\n", rgb);
		return -1;
	}

	fout = fopen(outname, "wb");
	if (fg == NULL) {
		printf("Couldn't open %s\n", outname);
		return -1;
	}

	if (populate_buffer(fg, (void **)&fgpix, width, height, sizeof(uint16_t)) < 0)
		return -1;

	if (populate_buffer(bg, (void **)&bgpix, width, height, sizeof(uint16_t)) < 0)
		return -1;

	newpix = malloc(width * height * sizeof(uint16_t));

	for (h = 0; h < height; h++) {
		uint16_t *fg = &fgpix[offset];
		uint16_t *bg = &bgpix[offset];
		uint16_t *new = &newpix[offset];

		for (w = 0; w < width; w++) {
			uint16_t *fgpixel = &fg[w];
			uint16_t *bgpixel = &bg[w];
			uint16_t *newpixel = &new[w];
			*newpixel = blend_pixel_rgb565_rgb565(fgpixel, bgpixel, alpha);
		}
		offset += width;
	}

	fwrite(newpix, sizeof(uint16_t), width * height, fout);

	free(newpix);
	free(fgpix);
	free(bgpix);

	fclose(fout);
	fclose(fg);
	fclose(bg);

	return 0;
}
