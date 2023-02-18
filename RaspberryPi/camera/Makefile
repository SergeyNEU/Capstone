CXX = g++
CXXFLAGS = -Wall -O3 `pkg-config --cflags --libs opencv4`
LDFLAGS = `pkg-config --cflags --libs opencv4`
SOURCES = test.cpp camera.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = camera

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
