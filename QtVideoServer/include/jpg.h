#ifndef __JPG_H__
#define __JPG_H__

#define MAX_JPG_SIZE	(1024 * 1024 - sizeof (unsigned int))

struct jpg_buf_t {
	unsigned char buf[MAX_JPG_SIZE];
	unsigned int len;
};

#endif
