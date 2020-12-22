#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/screen.h>

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
	if(text_mode) {
		((void)0);
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