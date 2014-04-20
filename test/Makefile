CC=g++
CXXFLAGS=-I../src -I../src/STEP_SDK -I../extra/hayai/src -Wall -O2
CMAKEFLAGS=

benchmark: test
	./test

test: test.o FORCE
	cd ../extra/hayai && cmake . -G 'Unix Makefiles' $(CMAKEFLAGS) && make
	make -C ../src
	$(CC) -o test $< ../extra/hayai/src/libhayai_main.a ../src/libSTEP_mid.a -static
FORCE:

.PHONY: benchmark FORCE
