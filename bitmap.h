#ifndef _BITMAP_
#define _BITMAP_

									  /*18610670370*/
#define BITMAP_LEN ((unsigned long long)10000000000)

void bitmap_creat(unsigned long long max_length);
void bitmap_destroy();

int bitmap_set(unsigned long long offs);
int bitmap_setstr(char *);

int bitmap_get(unsigned long long offs, int *isIn);
int bitmap_getstr(char *, int *isIn);

#endif

