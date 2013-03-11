#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#if USE_QUEUE
#include "queue.h"
queue_t dq = {&dq, &dq};
#elif USE_TREE
#include "tree.h"
tree_t *tree;
#elif USE_BITMAP
#include "bitmap.h"
#else
#error "should define USE_TREE or USE_QUEUE = 1"
#endif

#define GEN_LEN	100*10*1000

/* time_diff return ms */
#define time_diff()	end.tv_nsec>start.tv_nsec?\
	((end.tv_sec-start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec)/1000/1000):\
	((end.tv_sec-start.tv_sec-1)*1000 + (start.tv_nsec - end.tv_nsec)/1000/1000)

#define START()  clock_gettime(CLOCK_MONOTONIC,&start);\
		  		printf("\nstart:%lu S %lu NS\n\n", \
						start.tv_sec, start.tv_nsec)

#define END()    clock_gettime(CLOCK_MONOTONIC,&end);\
		  			printf("\n end :%lu S %lu NS\n"\
						"last :%ld MS\n\n", \
						end.tv_sec, end.tv_nsec, time_diff())
	
extern int gen_data(const char *filename, unsigned long long len);

int main(int argc, char **argv)
{
	char *filename1 = argc>=2 ? argv[1]:"out1.txt";
	char *filename2 = argc>=3? argv[2]:"out2.txt";
	char *outfile = argc>=4? argv[3]:"result.txt";

	int fd, result_fd;

	int i = 0,j = 0;
	int repeat = 0, ret = 0;

	struct timespec start,end;

#if USE_QUEUE
	ele_t	*eitem;
#endif

	printf("generataing data 1...\n");
	START();
	gen_data(filename1, GEN_LEN);
	END();
	printf("generataing data 2...\n");
	START();
	gen_data(filename2, GEN_LEN);
	END();
	printf("generate data over!\n");
	printf("press ENTER key to continue <ENTER>\n");
	getchar();

	printf("reload two file data, then combine them\n");
	START();
#if USE_BITMAP
	{
		/*  load file1 to the data */
		int fd=open(filename1, O_RDWR);
		char *data, *p, *wpos;
		unsigned long long tmp;

		result_fd = open(outfile, O_RDWR|O_CREAT|O_TRUNC, 0755);
		if( result_fd<0 )
		{
			printf("Error : cannot creat file %s.\n", outfile);
			exit(0);
		}

		if( fd < 0 )
		{
			printf("Error : reload file %s error.\n", filename1);
			exit(0);
		}

		data = (char *)malloc(GEN_LEN*12);
		if( data == NULL )
		{
			printf("Error : cannnot alloc memory size %d \n", GEN_LEN*12);
			exit(0);
		}
		memset(data, 0, GEN_LEN*12);
		read(fd, data, GEN_LEN*12);

		/* copy the first file to the result file */
		write(result_fd, data, GEN_LEN*12);

									   /*18610670370*/
		bitmap_creat((unsigned long long)10000000000);
		/*  the data file 1 shoule not have the same data */
		for(p=data, i=0;i<GEN_LEN;i++)
		{
			*(p+11)=0;
			ret = bitmap_setstr(p);
			if( ret == 0 )
			{
//					printf("tmp %llu\n data[%d]=%s\n", tmp, i,data[i]);
				printf("Warn : this maybe error . %s repeat index %d\n", p, i);
					exit(0);
				repeat++;
			}
			p+=12;
		}
	
		memset(data, 0, GEN_LEN*12);
		close(fd);
		fd=open(filename2, O_RDWR);
		if( fd < 0 )
		{
			printf("Error : reload file %s error.\n", filename1);
			exit(0);
		}
		read(fd, data, GEN_LEN*12);
		close(fd);

		printf("Result:\n");
		for(wpos=data,p=data,i=0;i<GEN_LEN;i++)
		{
			*(p+11)=0;
			ret = bitmap_setstr(p);
			if( ret == 0 )
			{
				printf("\t%s repeat index %d\n", p, i);
				*(p+11) = '\n';
				repeat++;
				write(result_fd, wpos, p-wpos);
				wpos = p+12;
			}

			*(p+11) = '\n';
			p+=12;
			if( p-wpos >= 4096 )
			{
				write(result_fd, wpos, p-wpos);
				wpos = p;
			}
		}

		write(result_fd, wpos, p-wpos);
		close(result_fd);
		printf("Total:%d  Repeat:%d\n", 2*GEN_LEN, repeat);
	}
#endif
	END();
	
	while(1)
	{
		char tmp;
		printf("Do you want write the result to file By order?<y/N>");
		tmp = getc(stdin);
		if( tmp == 'N' || tmp == 'n' )
			goto EXIT;

		if( tmp == 'Y' || tmp == 'y' )
			break;
	}

	START();
#if USE_BITMAP
	{
		char *buf, *wpos;
		int ret;
		unsigned long long tmp_len;
		int order_fd = 0;

		order_fd = open("order_result.txt", O_RDWR|O_CREAT|O_TRUNC, 0766);
		buf=(char*)malloc(4500);
		if(buf == NULL)
		{
			printf("Error:can not allocate memory 4KB\n");
			exit(0);
		}
		memset(buf, 0, 4150);
		for(tmp_len=3000000000UL,wpos=buf;tmp_len != 9000000000; tmp_len++)
		{
			ret = bitmap_get(tmp_len, 0);
			if(ret == 1)
			{
//				printf("\ntmp_len : %llu \n", tmp_len);
				sprintf(wpos, "1%010llu\n", tmp_len);
				wpos += 12; 

				if(wpos-buf >= 4096)
				{
					write(order_fd, buf, wpos-buf);
					wpos=buf;
					memset(buf, 0, 4150);
#if 0
					printf(".");
					fflush(stdout);
#endif
				}
			}

			if( tmp_len == 4000000000UL )
				tmp_len = 4999999999;
			if( tmp_len == 6000000000UL )
				tmp_len = 7999999999;
		}
		write(order_fd, buf, wpos-buf);
		printf("write over!\n");
		close(order_fd);
		free(buf);
	}
#endif
	END();


EXIT:
#if USE_BITMAP
	bitmap_destroy();
#endif

	printf("process over!\n");
	return 0;
}

