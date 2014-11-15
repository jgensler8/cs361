
BINDIR=./bin
BUILDPATHS=mandelbrot mandelCalc mandelDisplay
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

test: default
	cd $(BINDIR) && \
	echo "TEST ONE" && \
	./parent < ../test/infile

reclaim:
	ipcrm `ipcs -q | grep \`whoami\` | awk '{print "-q " $$2}'`
	ipcrm `ipcs -m | grep \`whoami\` | awk '{print "-m " $$2}'`
