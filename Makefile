TARGET = spam
SOURCES += $(wildcard *.cpp)
OBJECTS += $(SOURCES:%.cpp=%.o)

CXX ?= g++

CXXFLAGS += -c -Wall $(shell pkg-config --cflags opencv)
LDFLAGS += $(shell pkg-config --libs --static opencv) -lwiringPi

all: $(TARGET)

$(TARGET): $(OBJECTS); $(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS) 

%.o: %.cpp; $(CXX) $< -o $@ $(CXXFLAGS) 

clean: ; rm -f $(OBJECTS) $(TARGET)