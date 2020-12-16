#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	printf("                              \n            +my.              \n          +mMMy-              \n        +mMMy-                \n      +mMMd:                  \n    +mMMMMMMMmy/`             \n  +NMMmy+//+sdMMNo        -hd:\n`hMMh-        .yMMd`    -hMMm/\nsMMy            oMMh  -hMMm/  \nNMM`             NMM/hMMm/    \nNMM.             NMMMMm/      \noMMh`           sMMMm/        \n sMMm/`       :dMMm/          \n  :dMMMdsooshNMMm/            \n    .+ymNMMNmho-              \n\nWelcome to tynvos\n\n");
	printf("Terminal initialized, this works :)\n\nMade by frogstair");

}
