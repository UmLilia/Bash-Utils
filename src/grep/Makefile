CC = gcc
FLAGS = -Wall -Werror -Wextra
SRC = s21_grep.c
EXE = $(SRC:.c=)
CLANG = .clang-format

all: s21_grep clang

s21_grep:
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