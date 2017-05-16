#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	set_alpha_argb("fg.argb", "fg.argb.alpha", 262, 196, 0xFF);
	set_alpha_argb("bg.argb", "bg.argb.alpha", 262, 196, 0xFF);

	blend_argb_argb("fg.argb.alpha", "bg.argb.alpha", "final.argb", 262, 196);

	/*blend ARGB8888 with RGB565 */
//	blend_argb_rgb565("fg.argb.alpha", "bg.bgr", "final.bgr", 262, 196);

	/*blend RGB565 with RGB565 */
	blend_rgb565_rgb565("fg.bgr", "bg.bgr", "final.bgr", 262, 196, 0x80);
}
