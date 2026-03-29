CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRCS = $(wildcard *.cpp)
BUILD_DIR = build
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))

TARGET = $(BUILD_DIR)/lcd_driver

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)