TARGET = alarm-client
DIR = /usr/local/bin

SOURCES += $(wildcard *.cpp)
OBJECTS += $(SOURCES:%.cpp=%.o)

CXX ?= g++

CXXFLAGS += -c -Wall $(shell pkg-config --cflags opencv)
LDFLAGS += $(shell pkg-config --libs --static opencv) -lwiringPi

all: $(TARGET)

$(TARGET): $(OBJECTS); $(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS) 

%.o: %.cpp; $(CXX) $< -o $@ $(CXXFLAGS) 

clean: ; rm -f $(OBJECTS) $(TARGET)


install: ;
	mkdir /data
	mkdir /etc/$(TARGET)
	cp config /etc/$(TARGET)/config
	cp $(TARGET) $(DIR)
	cp initd_script /etc/init.d/$(TARGET)
	chmod +x /etc/init.d/$(TARGET)
	systemctl daemon-reload

remove: ;
	rm -rf $(DIR)$(TARGET)
	rm -rf /etc/$(TARGET)
	rm -rf /data
	rm -rf /etc/init.d/$(TARGET)

.PHONY: clean install remove all