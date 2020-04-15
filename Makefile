all: bin/dfi

bin/dfi: src/dfi.sh
	mkdir -p bin
	cp src/dfi.sh bin/dfi
	chmod a+x bin/dfi

clean:
	rm -rf bin

.PHONY: all clean
