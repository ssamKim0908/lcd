CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Iinterface -Ihal -Ifactory -Ilcd -Iservice

PLATFORM ?= host
ifeq ($(PLATFORM),device)
    CXXFLAGS += -DTARGET_DEVICE
endif

BUILD_DIR = build
TARGET = $(BUILD_DIR)/lcd_driver

SRCS = $(wildcard *.cpp) \
       $(wildcard hal/*.cpp) \
       $(wildcard factory/*.cpp) \
       $(wildcard lcd/*.cpp) \
       $(wildcard service/*.cpp)

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
