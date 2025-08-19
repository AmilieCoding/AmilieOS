// Definintion file for the limine requests as seen in boot/limine_requests.c
#ifndef LIMINE_REQUESTS_H
#define LIMINE_REQUESTS_H

#include <limine.h>

// These are defined in /boot/limine_requests.c
// I'm unsure as if I even need this file to access - the spec sheet says
// I don't NEED to access them.
extern volatile LIMINE_REQUESTS_START_MARKER;
extern volatile LIMINE_REQUESTS_END_MARKER;
