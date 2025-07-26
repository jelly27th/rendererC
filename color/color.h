#ifndef _COLOR_H
#define _COLOR_H

typedef struct {
	union {
		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		};
		unsigned int ucolor;
		unsigned char rgba[4];
	};
} color_t;

#endif
