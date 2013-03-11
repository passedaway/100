CFG_USE_QUEUE ?= 0
CFG_USE_BITMAP ?= 1
CFG_USE_TREE ?= 0

OBJS = generate.o main.o

CFLAGS += -g

ifeq ($(strip ${CFG_USE_TREE}), 1)
	OBJS += tree.o
endif

ifeq ($(strip ${CFG_USE_QUEUE}), 1)
	OBJS += queue.o
endif

ifeq ($(strip ${CFG_USE_BITMAP}), 1)
	OBJS += bitmap.o
endif

CFLAGS += -DUSE_QUEUE=${CFG_USE_QUEUE}
CFLAGS += -DUSE_TREE=${CFG_USE_TREE}
CFLAGS += -DUSE_BITMAP=${CFG_USE_BITMAP}
LDFLAGS += -lrt

all:clean $(OBJS)
	@echo [LD	100]
	@cc $(OBJS) -o 100 $(LDFLAGS)

%.o : %.c
	@echo [CC	$<]
	@cc $(CFLAGS) -c -o $@ $<


.PHONY:clean
clean:
	@echo use queue $(CFG_USE_QUEUE)
	@echo 'use tree $(CFG_USE_TREE)'
	@echo 'use bit map $(CFG_USE_BITMAP)'
	@echo 'cflags $(CFLAGS)'
	@echo 'lflags $(LDFLAGS)'
	@echo 'objs $(OBJS)'
	@-rm gen *.o -rf

