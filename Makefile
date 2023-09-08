.PHONY: run
.PHONY: build
.PHONY: clean

build:
	cd FAMP && make

run:
	cd FAMP && make run

clean:
	cd FAMP && make clean