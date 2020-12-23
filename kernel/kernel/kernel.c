#include <stdio.h>
#include <stdlib.h>

#include <kernel/tty.h>
#include <kernel/screen.h>

#pragma GCC push_options
#pragma GCC optimize ("O0")

void wait() {
	for(int i = 0; i < 100000; i++) {
		((void)0);
	}
}

#pragma GCC pop_options

void print_logo();

static int text_mode = 0;
static int protected = 0;

void kernel_early() {
	asm("mov %%cr0, %0\n\t"
		:"=a"(protected));
		
	protected &= 1;

	if(!text_mode) {
		if(screen_initialize()) {
			text_mode = 1;
			kernel_early();
		}
	} else {
		terminal_initialize();
		int color = 4 - protected * 2;
		char* prot = protected ? "on" : "off";

		printf("Protected mode: %C%s%C\n", color, prot, 7);
		printf("%CGraphics mode not initialized correctly%C", 4, 7);
	}
}

void kernel_main() {
	if(!text_mode) {
		uint8_t r, g, b = 0;
		uint32_t color = 0;
		while(1) {
			if(r < 255 && g == 0) {
				r++;
			}
			if(r == 255 && g < 255) {
				g++;
			}
			if(g == 255 && r > 0) {
				r--;
			}
			if(b < 255 && g == 255 && r == 0) {
				b++;
			}
			if(b == 255 && g > 0) {
				g--;
			}
			if(b > 0 && r == 255) {
				b--;
			}

			color = (r << 16) | (g << 8) | b;

			putpixel(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, color);
			wait();
		}

	} else {
		print_logo();
		printf("Welcome to %Ctynvos%C kernel (v 0.0.0.1)\n", 3, 7);
		puts("Made by frogstair");
	}
}

void print_logo() {
	printf("%C", 3);
	puts("");
	puts("            ######            ");
	puts("          ######              ");
	puts("        ######                ");
	puts("      ######                  ");
	puts("    #############             ");
	puts("  ################        ####");
	puts(" ######       ######    ######");
	puts("#####           ####  ######  ");
	puts("####             #########    ");
	puts("####             #######      ");
	puts("#####           ######        ");
	puts(" ######       ######          ");
	puts("  ################            ");
	puts("    ############              ");
	puts("       ######                 ");
	puts("");
	printf("%C", 7);
}