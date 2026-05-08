CC = gcc
RM = -rm -rf

SRCS = $(wildcard src/*.c)
EXENAME = hi_gzip

all: gzip hi_gzip

gzip:
	tar -xf gzip-1.8.tar.gz
	cd gzip-1.8
	pushd gzip-1.8;patch -p1 < ../hi_gzip.patch;patch -p1 < ../gzip_glibc.patch;./configure;make -j12;popd

hi_gzip: $(OBJS) 
	mkdir bin -p
	cp gzip-1.8/gzip bin;
	@$(CC) $(SRCS) -o bin/$(EXENAME) 

clean:
	@$(RM) -rf *.bak *~ bin
	@$(RM) -rf gzip-1.8

.PHONY: all clean
