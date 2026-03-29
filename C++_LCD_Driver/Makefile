CXX = g++

CXXFLAGS = -std=c++17 -Wall

SRCS = $(wildcard ./*.cpp)
OBJS = $(patsubst ./%.cpp,./%.o,$(SRCS))

TARGET = test_spi


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(OBJS):./%.o:./%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)