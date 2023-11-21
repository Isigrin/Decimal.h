CC0=gcc -g
CC1=gcc -g
CFLAGS=-c
LDFLAGS= #-Wall -Wextra -Werror
OS := $(shell uname -s)

ifeq ($(OS), Darwin)
	CC1 += -D OS_MAC
else
	CC1 += -D OS_LINUX
endif

all: clean decimal.a test  

test: decimal.a test.c
ifeq ($(OS), Darwin)
	$(CC0) $(LDFLAGS) -o test test.c decimal.a -lcheck
	./test
else
	gcc test.c decimal.a -o test -lcheck -lsubunit -lrt -lpthread -lm
	./test
endif

decimal.a: decimal.o
	ar rcs decimal.a decimal.o
	ranlib $@
	cp $@ lib$@
	
decimal.o: decimal.c
	$(CC1) $(LDFLAGS) -c -o decimal.o decimal.c

clean:
	rm -rf test *.a *.o *.out fizz *.gc* *.info report

rebuild: clean all

gcov_report: 
ifeq ($(OS), Darwin)
	$(CC1) --coverage $(LDFLAGS) test.c decimal.c -o test -lcheck
else
	$(CC1) --coverage $(LDFLAGS) test.c decimal.c -o test -lcheck -lsubunit -lrt -lpthread -lm
endif
	./test
	lcov -t "test" -o test.info -c -d ./
	genhtml -o report test.info
