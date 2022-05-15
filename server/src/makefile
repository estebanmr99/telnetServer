SOURCEC = main.c Server.c
TARGETC = server

.PHONY: all

all: c1

c1:
	$(CC) $(SOURCEC) -o $(TARGETC) -lpthread

clean:
	-rm -f *.o
	-rm -f $(TARGETC)