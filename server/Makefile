CFLAGS = -Wall -L/usr/lib -lssl -lcrypto
TARGET = synq-server
OBJECTS =  synq-server.o ../common/protocol.o ../common/utils.o ../common/linked_list.o ../common/ssl.o

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

clean:
	rm  $(OBJECTS)
	rm $(TARGET)
