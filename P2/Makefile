TESTS = 
TESTBINS = $(patsubst %,tests/%, $(TESTS))
PRETESTBINS =  $(patsubst %,%test,$(TESTBINS))
TESTRESES = $(patsubst %,%_result.txt,$(TESTS))
TARGETS = spchk $(TESTBINS) $(PRETESTBINS)
FOLDERS = obj tests obj/tests

CC     = gcc
CFLAGS = -g -std=c99 -Wall -Wvla -Wunused-function -Wfatal-errors -fsanitize=undefined,address
LDFLAGS = -fsanitize=undefined,address
OBJECTS = obj/dictionary.o obj/ioutil.o obj/spchk.o

.PHONY: all
all: $(FOLDERS) $(TARGETS)

$(FOLDERS): %:
	mkdir -p $@
	chmod g+w $@

$(TARGETS): %: obj/%.o $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
	chmod g+w $@

obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) -o $@ $^
	chmod g+w $@

../pa1.tar: *.c *.h Makefile
	cd .. && tar cf pa1.tar src/*.c src/*.h src/Makefile

.PHONY: clean
clean:
	rm -rf bin obj tests $(TARGETS)

.PHONY: test
test: $(TESTBINS) $(PRETESTBINS)
	rm -f $(TESTRESES) 
	for i in $(PRETESTBINS) ; do ./$$i ; done ; for i in $(TESTBINS) ; do echo $$i ; ./$$i ; done
	

