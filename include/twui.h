#pragma once

#include <stddef.h>
#include <stdint.h>
#include <termios.h>

extern struct termios orig;

struct twin {
	struct twin *parent, **children;
	struct { uint16_t y, x; } pos;
	struct { uint16_t h, w; } size;
	struct { uint16_t y, x; } cursor;
	struct termios term;
};

struct twin *
winit();
struct twin *
wnew(struct twin *p);
size_t
wchildren(struct twin *w);
struct twin *
wroot(struct twin *w);
void
wdel(struct twin *w);
void
wmove(struct twin *w, uint16_t r, uint16_t c);
void
wwrite(struct twin *w, void *buf, size_t size);
void
wread(struct twin *w, void *buf, size_t size);
void
wprint(struct twin *w, char *fmt, ...);
void
wscan(struct twin *w, char *fmt, ...);
