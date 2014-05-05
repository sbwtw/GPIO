
MAIN = test

CC = g++

OBJS = gpio.o \
		$(MAIN).o

LIBS = -L .

INCS = -I .

CPPFLAGS = -Wall -std=c++11

$(MAIN) : $(OBJS)
	$(CC) $(CPPFLAGS) $(LIBS) $(OBJS) -o $@

$(OBJS) : %.o :%.cpp
	$(CC) $(CPPFLAGS) $(INCS) -c -o $@ $<

clean:
	rm -rf *.o *~ $(MAIN)
