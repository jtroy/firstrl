CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-parameter -pedantic -pipe
LIBS = -lncurses -lm
OBJDIR = build
OBJECTS = actor.o item.o main.o
OBJECTS :=  $(addprefix ${OBJDIR}/,${OBJECTS})

main: $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJECTS) -o firstrl

${OBJDIR}/%.o : %.c
	@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi #create directory if it doesn't exist
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) firstrl
