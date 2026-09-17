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
twinit();
struct twin *
twnew(struct twin *p);
size_t
twchildren(struct twin *w);
struct twin *
twroot(struct twin *w);
void
twdel(struct twin *w);
void
twmove(struct twin *w, uint16_t r, uint16_t c);
void
twwrite(struct twin *w, void *buf, size_t size);
void
twread(struct twin *w, void *buf, size_t size);
void
twprint(struct twin *w, char *fmt, ...);
void
twscan(struct twin *w, char *fmt, ...);
