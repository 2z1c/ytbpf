
CLANG ?= clang
build:
	${CLANG} -O2 -target bpf -c hello_krn.bpf.c -o hello_krn.bpf.o

dis:
	llvm-objdump-14 -S hello_krn.bpf.o

readelf:
	llvm-readelf-14 -S -s hello_krn.bpf.o