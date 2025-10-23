#include <sys/ioctl.h>
#include <unistd.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <twui.h>

struct termios orig;

struct twin *
winit()
{
	struct twin *root = malloc(sizeof(struct twin));
	struct winsize ws;
	ioctl(0, TIOCGWINSZ, &ws);
	ioctl(0, TCGETS, &orig);
	root->term = orig;
	root->term.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	root->term.c_oflag &= ~(OPOST);
	root->term.c_cflag |= (CS8);
	root->term.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	root->term.c_cc[VMIN] = 0;
	root->term.c_cc[VTIME] = 0;
	ioctl(0, TCSETSF, &root->term);
	root->pos.y = 0;
	root->pos.x = 0;
	root->cursor.y = 0;
	root->cursor.x = 0;
	root->size.h = ws.ws_row;
	root->size.w = ws.ws_col;
	root->parent = NULL;
	root->children = NULL;
	write(1, "\e[?1049h\e[J\e[H", 14);
	return root;
}

struct twin *
wnew(struct twin *p)
{
	size_t n = wchildren(p);
	if (!p->children)
		p->children = malloc(2*sizeof(struct twin*));
	else
		p->children = realloc(p->children, n*(sizeof(struct twin*)+1));
	p->children[n] = malloc(sizeof(struct twin));
	p->children[n]->pos.y = 0;
	p->children[n]->pos.x = 0;
	p->children[n]->cursor.y = 0;
	p->children[n]->cursor.x = 0;
	p->children[n]->size.h = p->size.h;
	p->children[n]->size.w = p->size.w;
	p->children[n]->parent = p;
	p->children[n+1] = 0;
	return p->children[n];
}

size_t
wchildren(struct twin *w)
{
	size_t n = 0;
	if (w->children == 0)
		return 0;
	while (w->children[n])
		n++;
	return n;
}

struct twin *
wroot(struct twin *w)
{
	while (w->parent)
		w = w->parent;
	return w;
}

void
wdel(struct twin *w)
{
	if (w->children) {
		size_t n = wchildren(w);
		for (size_t i = 0; i < n; i++)
			wdel(w->children[i]);
		free(w->children);
	}
	if (w->parent) {
		size_t i, n = wchildren(w->parent);
		for (i = 0; w->parent->children[i] == w; i++);
		n--;
		for (; i < n; i++)
			w->parent->children[i] = w->parent->children[i+1];
	} else {
		ioctl(0, TCSETS, &orig);
		write(1, "\e[?1049l", 8);
	}
	free(w);
}

void
wmove(struct twin *w, uint16_t r, uint16_t c)
{
	w->cursor.y = r;
	w->cursor.x = c;
	printf("\e[%d;%dH", w->pos.y+r+1, w->pos.x+c+1);
}

void
wwrite(struct twin *w, void *buf, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		write(1, buf+i, 1);
		w->cursor.x++;
		if (w->cursor.x >= w->size.w) {
			w->cursor.x = 0;
			w->cursor.y++;
			printf("\e[%d;%dH", w->pos.y+w->cursor.y+1, w->pos.x+w->cursor.x+1);
		}
	}
	
}

void
wread(struct twin *w, void *buf, size_t size)
{
	read(0, buf, size);
}

void
wprint(struct twin *w, char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	wwrite(w, buffer, strlen(buffer));
	va_end(args);
}

void
wscan(struct twin *w, char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[1024];
	wread(w, buffer, sizeof(buffer));
	vsscanf(buffer, fmt, args);
	va_end(args);
}
