

OUTPUT := $(abspath .output)
BPFTOOL_SRC := $(abspath ./bpftool/src)
BPFTOOL_OUTPUT ?= $(abspath $(OUTPUT)/bpftool)
BPFTOOL ?= $(BPFTOOL_OUTPUT)/bootstrap/bpftool

$(OUTPUT) ${BPFTOOL_OUTPUT}:
	mkdir -p $@

build:	| ${OUTPUT} ${BPFTOOL_OUTPUT}
	$(Q)$(MAKE) -C $(BPFTOOL_SRC) ARCH= CROSS_COMPILE=  OUTPUT=$(BPFTOOL_OUTPUT)/  bootstrap -j22
