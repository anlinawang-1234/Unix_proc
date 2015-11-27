
all : $(OBJ)
	
$(COM)/%.o : $(SRC)/%.c
	$(CC) -fpic -g -c -o $@ $< -I$(HEAD)

.PHONY : clean

clean :
	$(RM) *.o
