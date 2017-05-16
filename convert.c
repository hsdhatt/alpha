#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int width = 262;
int height = 196;

int arr[262 * 196];
uint16_t bgr_arr[262 * 196];

void convert_rgba_to_bgra()
{
	int h, w, offset = 0;

	for (h = 0; h < height; h++) {
		int *src = &arr[offset];

		for (w = 0; w < width; w++) {
			int *pix = &src[w];
			int r, g, b, a;

			r = (*pix) & 0xFF;
			g = (*pix >> 8) & 0xFF;
			b = (*pix >> 16) & 0xFF;
			a = (*pix >> 24) & 0xFF;

			*pix = 0;
			*pix = b | (g << 8) | (r << 16) | (a << 24);
		}
		offset += width;
	}
}

void convert_rgba_to_bgr()
{
	int h, w, offset = 0;

	for (h = 0; h < height; h++) {
		int *src = &arr[offset];
		uint16_t *dst = &bgr_arr[offset];

		for (w = 0; w < width; w++) {
			int *srcpix = &src[w];
			uint16_t *dstpix = &dst[w];
			int r, g, b, a;

			r = (*srcpix) & 0xFF;
			g = (*srcpix >> 8) & 0xFF;
			b = (*srcpix >> 16) & 0xFF;
			a = (*srcpix >> 24) & 0xFF;

			printf("%d %d %d - >", r, g, b);
			r = (r >> 3) & 0x1F;
			g = (g >> 2) & 0x3F;
			b = (b >> 3) & 0x1F;

			printf("%d %d %d - >", r, g, b);
			*dstpix = b | (g << 5) | (r << 11);
			printf("%x\n", *dstpix);

		}
		offset += width;
	}
}
int main(int argc, char *argv[])
{
	FILE *fg = fopen("fg.rgba", "r");
	FILE *bg = fopen("bg.rgba", "r");

	FILE *fgout = fopen("fg.argb", "wb");
	FILE *bgout = fopen("bg.argb", "wb");

	fread(arr, sizeof(int), width * height, fg);
	convert_rgba_to_bgra();
	fwrite(arr, sizeof(int), width * height, fgout);

	fclose(fgout);

	fgout = fopen("fg.bgr", "wb"); 

	fseek(fg, 0, SEEK_SET);
	fread(arr, sizeof(int), width * height, fg);
	convert_rgba_to_bgr();
	fwrite(bgr_arr, sizeof(uint16_t), width * height, fgout);

	fread(arr, sizeof(int), width * height, bg);
	convert_rgba_to_bgra();
	fwrite(arr, sizeof(int), width * height, bgout);

	fclose(bgout);

	bgout = fopen("bg.bgr", "wb"); 

	fseek(bg, 0, SEEK_SET);
	fread(arr, sizeof(int), width * height, bg);
	convert_rgba_to_bgr();
	fwrite(bgr_arr, sizeof(uint16_t), width * height, bgout);

	fclose(fg);
	fclose(bg);
	fclose(fgout);
	fclose(bgout);
	return 0;
}

