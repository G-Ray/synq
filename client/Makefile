CFLAGS = -Wall -L/usr/lib -lssl -lcrypto
TARGET = synq-client
OBJECTS =  ../common/protocol.o ../common/utils.o ../common/linked_list.o  ../common/ssl.o synq-client.o

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

clean:
	rm $(OBJECTS)
	rm $(TARGET)
