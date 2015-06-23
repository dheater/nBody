CC = g++
CFLAGS = -O2 -std=c++11
DEPS = classBody.hpp
TARGET = driver

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o
	
$(TARGET).o: $(DEPS)
	$(CC) $(CFLAGS) -c $(TARGET).cpp
	
clean:
	$(RM) count *.o *~