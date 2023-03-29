CXX = g++
CXXFLAGS = -Wall -O3 -lbluetooth
LDFLAGS = -O3 -lbluetooth
SOURCES = test.cpp raspberry_bluetooth.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = bluetooth

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
