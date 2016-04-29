CC = gcc
EXEC = SQL
OBJS = MinS_SQL_shell.o main.o
DEPS = MinS_SQL_shell.h

%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< 
	
$(EXEC): $(OBJS)
	gcc -o $@ $^
	
clean:
	rm *~ *.o $(EXEC)
