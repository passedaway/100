#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmap.h"
typedef struct {
	unsigned char *data;
	unsigned long long len;
}bitmap_t;

static bitmap_t bm = {0, 0ULL};

static unsigned long long str2ll(const char *s);

void bitmap_creat(unsigned long long max_length)
{
	/*  if data is null, then create it.
	 *  if already create , then check the lengh  
	 *  if max_length > bm.len , then recreat it,
	 *  if <, then memset the bm.data, donot change the max_length
	 */

//	printf("%s:max_length %llu bm.len = %llu in\n", __FUNCTION__,max_length, bm.len);
	if( !bm.data || (max_length > bm.len) )
	{
		if(bm.data)
			free(bm.data);

		printf("%s:need memory size %dM\n", __FUNCTION__, (int)(max_length>>23));
		bm.data = (unsigned char *)malloc(max_length>>3);
		if(bm.data == NULL)
		{
			printf("%s:cannot malloc %llu memory\n", __FUNCTION__, max_length>>3);
			exit(0);
		}
		bm.len = max_length;
	}

	if( bm.data && (max_length <= bm.len) )
	{
//		printf("%s:bm.len=%llu, memeory len=%llu\n", __FUNCTION__, bm.len, max_length>>3);
		memset(bm.data, 0, max_length>>3);
		bm.len = max_length;
	}

//	printf("%s:bm.data = %p\nbm.len = %llu out\n", __FUNCTION__, bm.data, bm.len);
}

int bitmap_set(unsigned long long offs)
{
	unsigned long long pos = offs/8;
	int bits = offs%8;
	unsigned char *data = (unsigned char *)(bm.data + pos);

//	printf("%s:pos=%llu bits=%d offs=%llu data=0x%02x\n", __FUNCTION__,pos, bits, offs, *data);
	if(offs > bm.len)
		return 0;
	if( *data & (1<<bits))
		return 0;
	*data |= 1<<bits;
	return 1;
}

int bitmap_get(unsigned long long offs, int *isIn)
{
	unsigned long long pos = offs/8;
	int bits = offs%8;
	unsigned char *data = (unsigned char *)(bm.data + pos);
	int ret = *data & 1<<bits?1:0;
//	printf("%s:pos=%llu bits=%d offs=%llu data=0x%02x\n", __FUNCTION__,pos, bits, offs, *data);
	if(isIn)
		*isIn = ret;

	return ret;
}

int bitmap_setstr(char *str)
{
	char *_s = str+1;
	unsigned long long tmp = str2ll(_s);

	return bitmap_set(tmp);
}

int bitmap_getstr(char *str, int *isIn)
{
	char *_s = str+1;
	unsigned long long tmp = str2ll(_s);

	return bitmap_get(tmp, isIn);
}

void bitmap_destroy()
{
	if(bm.data)
	{
		free(bm.data);
		bm.data = 0;
		bm.len = 0;
	}
} 

unsigned long long str2ll(const char *s)
{
	char *str = (char *)s;
	unsigned long long ret = 0;
	long long qen = 1, queue = 10;

	char *end = str + strlen(str) - 1;

	if( *str=='0' && (*(str+1) == 'x' || *(str+1)=='X'))
		str+=2;
	
	while(end >= str)
	{
		if(*end >= '0' && *end <= '9')
			ret += (*end-'0')*qen;
		else if( *end >= 'a' && *end <= 'f')
			ret += (*end-'a'+10)*qen;
		else if(*end>='A'&&*end<='F')
			ret += (*end-'A'+10)*qen;
		else
			return ret;
//			return -1;

		end--;
		qen *= queue;
	}

	return ret;
}


