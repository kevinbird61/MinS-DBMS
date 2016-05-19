CC = gcc
EXEC = SQL
CFLAG = -Wall -std=c11
OBJS = MinS_SQL_shell.o stringOperation.o main.o
DEPS = MinS_SQL_shell.h stringOperation.h

%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< 
	
$(EXEC): $(OBJS)
	gcc -o $@ $^ $(CFLAG)

run:
	./$(EXEC)
clean:
	rm *.o $(EXEC)
