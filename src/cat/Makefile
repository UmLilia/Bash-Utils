CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11
SRC = s21_cat.c
HDR = $(SRC:.c=.h)
EXE = $(SRC:.c=)
CLANG = .clang-format

all: s21_cat clang

s21_cat:
	$(CC) $(FLAGS) $(SRC) -o $(EXE)

clean:
	rm -f $(EXE)

clang:
ifneq ("$(wildcard $(CLANG))","")
    $(shell clang-format -i *.c *.h)
else
	echo "$(CLANG) not found!"
endif

tests:
	cd test && chmod +x ./test.sh && ./test.sh

rebuild: clean all clang tests

.PHONY = all clean rebuild tests