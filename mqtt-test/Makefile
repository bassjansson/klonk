CC       = gcc
CXX      = g++
CCFLAGS  = -Wall
CXXFLAGS = -Wall # -Wno-undefined-var-template
LDFLAGS  =
LDLIBS   = -lpaho-mqtt3as

TARGET  = mqtt-test
OBJECTS = mqtt-test.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJECTS) $(LDLIBS)
	rm $(OBJECTS)

.cpp.o:
	$(CXX) -c $< $(CXXFLAGS)

clean:
	rm $(TARGET)
	rm $(OBJECTS)

touch:
	touch ./*.cpp
	touch ./*.h
