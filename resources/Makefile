
OUTDIRS := build/helloworld.objs
CFLAGS := -g -O2 -Wall -Wmissing-field-initializers -Werror
INCLUDES := -I${OUTDIRS}/
LIBBPF := build/libbpf.a

all:
	g++ ${CFLAGS}  --static unittests/helloworld.cpp ${LIBBPF} ${INCLUDES}  -lelf -lz -o helloworld.out