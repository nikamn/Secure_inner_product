CC = g++
CFLAGS = -g -I.
DEPS = functions.h

%.o: %cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ALL =	hEnc

all: $(ALL)

hEnc: functions.o hEnc.o
	$(CC) -o hEnc hEnc.o functions.o -pthread -lntl -lgmp

clean:
	rm *.o hEnc