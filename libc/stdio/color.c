#include <stdint.h>

#if defined(__is_libk)
#include <kernel/screen.h>
#endif

void color(uint64_t color) {
	#if defined(__is_libk)
		screen_setcolor(color);
	#else
		// TODO: Implement stdio and the write system call.
	#endif
}