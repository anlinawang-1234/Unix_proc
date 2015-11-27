export PROCPATH = $(shell pwd)
export SRC  = $(PROCPATH)/src
export HEAD = $(PROCPATH)/head
export HEADFILE1 = exitcode.h
export HEADFILE2 = pooldata.h
export HEADFILE3 = poolfunc.h
export HEADFILE4 = my_apue.h
export LIB  = $(PROCPATH)/lib
export BIN  = $(PROCPATH)/bin
export COM  = $(PROCPATH)/com
export OBJ  = $(COM)/threadpool.o \
              $(COM)/configread.o

USRD = $(PROCPATH)/user

TARGET = libthreadpool.so

all : clean  uninstall tgt install ur

tgt : $(TGT)
	@cd $(COM) && make
	$(CC) -shared -o $(LIB)/$(TARGET) $(OBJ) 
	@date

install :
	install -c $(LIB)/$(TARGET) /lib64
	install -c $(HEAD)/$(HEADFILE1)   /usr/include
	install -c $(HEAD)/$(HEADFILE2)   /usr/include
	install -c $(HEAD)/$(HEADFILE3)   /usr/include
	install -c $(HEAD)/$(HEADFILE4)   /usr/include
	@printf "{$(TARGET)}{$(HEADFILE1)}{$(HEADFILE2)}{$(HEADFILE3)}{$(HEADFILE4)} installed\n"

uninstall :
	@cd /lib64 && $(RM) $(TARGET)
	@cd /usr/include && $(RM) $(HEADFILE1)
	@cd /usr/include && $(RM) $(HEADFILE2)
	@cd /usr/include && $(RM) $(HEADFILE3)
	@cd /usr/include && $(RM) $(HEADFILE4)
	@printf "{$(TARGET)}{$(HEADFILE1)}{$(HEADFILE2)}{$(HEADFILE3)}{$(HEADFILE4)} uninstalled\n"

ur:
	@cd $(USRD) && make

clean :
	@cd $(LIB) && $(RM) $(TARGET)
	@cd $(COM) && make clean
	@cd $(USRD) && make clean
