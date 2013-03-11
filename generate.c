#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#if USE_BITMAP
#include "bitmap.h"
#else
#error "should define USE_BITMAP = 1"
#endif

int gen_data(const char *filename, unsigned long long len)
{
	unsigned long long l = 0ULL, _len;
	int i = 0, ret, repeat = 0;
	int fd = 0;
	char *data, *p;
	time_t t;

	if( filename == NULL )
	{
		printf("%s:donnot support memory mode!\n", __FUNCTION__);
		return -1;
	}

	fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0755);
	if( fd < 0 )
	{
		printf("Error : open file %s error!\n", filename);
		return -1;
	}

	_len = len + 10000;
	printf("%s:need memeory %d\n", __FUNCTION__, (int)(_len*12>>20));
	data = (unsigned char *)malloc(_len*12);
	if( data == 0 )
	{
		printf("%s:error no memory.\n", __FUNCTION__);
		exit(-1);
	}
	memset(data,0,_len*12);

	printf("generataing data (%llu) ...\n", _len);
	srand((unsigned)time(&t));
	for(p=data, l=0; l != _len;l++)
	{
		int j = 1;
		*p='1';
		/* for telphone number */
		while(1)
		{
			int tmp = (int)(10.0 * rand()/(RAND_MAX+1.0));
			if( tmp != 3 && tmp != 5 && tmp != 8 )
				continue;
			*(p+1) = tmp+'0';
			break;
		}

		for(j=2; j < 11; j++)
		{
			int tmp = (int)(10.0 * rand()/(RAND_MAX+1.0));
			tmp %= 10;
			*(p+j)=tmp+'0';
		}

		*(p+11)=0;
		p += 12;
	}
	printf("\rgenerate OK!\n");

	printf("check if have repeate data & write to file ...\n");

	{
		unsigned long long tmp;
		char *wpos = data;

		bitmap_creat(BITMAP_LEN);
		for(p = data, i=0;(len+repeat<_len) && (i<len+repeat);i++)
		{
			ret = bitmap_setstr(p);
			if( ret == 0 )
			{
				printf("%s repeat index %d\n", p, i);
				repeat++;

				write(fd, wpos, p-wpos);
				wpos = p+12;
			}

			*(p+11) = '\n';
			p+=12;
			if( p-wpos >= 4096 )
			{
				write(fd, wpos, p-wpos);
				wpos = p;
			}
		}
		/* flush the write buffer */
		write(fd, wpos, p-wpos);
		printf("Total:%llu  repeat:%d i=%d len=%llu\n", _len, repeat, i, len);
	}

	free(data);
	close(fd);
	bitmap_destroy();

	printf("%s:generate data success. write to file %s success\n", __FUNCTION__, filename);
	return 0;
}


#if 0
/* test gen_data */
int main(void)
{
	gen_data("data1", 100*10000);
	gen_data("data2", 100*10000);

	return 0;
}
#endif

