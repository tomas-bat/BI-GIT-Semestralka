all: bin bin/dfi bin/image_histogram

bin:
	mkdir -p bin

bin/dfi: src/dfi.sh
	cp src/dfi.sh bin/dfi
	chmod a+x bin/dfi

bin/image_histogram: src/image_histogram/image_histogram.c
	make -C src/image_histogram
	mv src/image_histogram/image_histogram bin/

clean:
	rm -rf bin

.PHONY: all clean
