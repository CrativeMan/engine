CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = -lGL -lglfw -lGLEW -lcglm -lm
TESTLDFLAGS = -lcglm -lm
SRCDIR = src/engine
OBJDIR = obj
TARGETDIR = bin
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
TARGETMAIN = $(TARGETDIR)/main

all: $(TARGETMAIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGETDIR):
	mkdir -p $(TARGETDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGETMAIN): $(OBJS) | $(TARGETDIR)
	mkdir -p logs
	mkdir -p logs/imgs
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(OBJDIR)
	rm -rf $(TARGETDIR)

tail:
	tail -f logs/engine.log
count:
	cloc --exclude-list-file=.clocignore .
run:
	./bin/main
cleanforce: clean
	rm -rf logs

.PHONY: all clean tail count run
