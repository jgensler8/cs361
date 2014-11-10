
BINDIR=./bin
BUILDPATHS=parent child_one child_two
UTILPATH=util

default: clean build

.PHONY: build util $(BUILDPATHS)

build: util $(BUILDPATHS)

util:
	make -C $(UTILPATH)

$(BUILDPATHS):
	cp $(UTILPATH)/obj/* $@/obj/ &&\
	cp $(UTILPATH)/include/* $@/include/ &&\
	make -C $@ && \
	cp $@/bin/* $(BINDIR)

clean:
	make -C $(UTILPATH) clean && \
	for bp in $(BUILDPATHS); do \
		make -C $$bp clean; \
	done
