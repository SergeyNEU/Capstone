CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -lwiringPi -lm 
LDFLAGS = -lwiringPi -lm -O3

SRCDIR = .
OBJDIR = .

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

TARGET = sensehat

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJDIR)/*.o $(TARGET)

.PHONY: all clean
