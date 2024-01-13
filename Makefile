CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99
LDFLAGS =
EXEC = tournoi
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DOC_DIR = doc

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS)-o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

run: $(EXEC)
	./$(EXEC)

doc:
	doxygen Doxyfile
	awk '/<p class="brief">/,/<\/p>/' $(DOC_DIR)/html/*.html > comments.html

clean:
	rm -f $(OBJ) $(EXEC)
	rm -rf $(DOC_DIR) comments.html

.PHONY: all run doc clean
