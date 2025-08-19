// Going to be using the Limine Bootloader Protocol as it is simply easier to
// work with compared to writing my own bootloader from scratch.
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

// Gotta use base revision 3 as 0-2 are considered "Deprecated". Refer to
// the specifications sheet (spec.md) - more specifically for the Limine 
// Protocol - for more information.
__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

// !!! DO NOT MAKE ANY LIMINE REQUESTS OPTIMIZABLE BY THE COMPILER !!!
__attribute__((used , section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0
};

// The start and end markers for limine requests are in boot/limine_requests.c
// As is the specifications for compilation in src/string.c

// We also need to "halt and catch fire" (basically burn the place!)
static void hcf(void) {
	for (;;) {
		asm ("hlt");
	}
}

// This is the kernels entry point. Please do not rename it (like ever.).
// If you do - at least notify on mailing lists or in Discord, and update
// the linker script accordingly.
void kmain(void) {
	// Ask bootloader if it understands our revision. Burn if not.
	if (LIMINE_BASE_REVISION_SUPPORTED == false) {
		hcf();
	}

	// Ask for a framebuffer, if not, burn again.
	if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
		hcf();
	}

	// Okay, if we made it here, we're good and let's fetch the framebuffer.
	struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

	// We're assuming its RGB and 32-bit pixels by the way. Will amend in the future to detect and suit better.
	for (size_t i = 0; i < 100; i++) {
		volatile uint32_t *fb_ptr = framebuffer->address;
		fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
	}

	// We did it! Hang time anyway!
	hcf();
}
