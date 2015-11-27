
USRC = userapp.c
TGT  = userapp
OPT  = -I$(HEAD) -lpthread -lthreadpool

$(TGT):
	$(CC) -g -o $(TGT) $(USRC) $(OPT)
	@date

clean :
	$(RM) $(TGT)
