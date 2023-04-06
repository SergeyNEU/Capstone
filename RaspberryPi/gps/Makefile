CXX = g++
CXXFLAGS = -Wall -O3
LDFLAGS = -O3
SOURCES = test.cpp gps.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = gps

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
