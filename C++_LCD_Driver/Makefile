CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Iinterface -Icommunication -Ifactory -Ilcd

BUILD_DIR = build
TARGET = $(BUILD_DIR)/lcd_driver

SRCS = $(wildcard *.cpp) \
       $(wildcard communication/*.cpp) \
       $(wildcard factory/*.cpp) \
       $(wildcard lcd/*.cpp)

OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
