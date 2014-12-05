
BINDIR=./bin
BUILDPATHS=raceTest
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
		rm -f $$bp/bin/*; \
		rm -f $$bp/obj/*; \
	done

test: default
	cd $(BINDIR) && \
	echo "TEST ONE" && \
	./raceTest 29 5 .1 .5
reclaim:
	ipcrm `ipcs -s | grep \`whoami\` | awk '{print "-s " $$2}'`
