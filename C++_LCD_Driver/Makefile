CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Iinterface -Ihal -Idisplay -Iinput -Iutil -Iframework -Iipc -Ishared

PLATFORM ?= host
ifeq ($(PLATFORM),device)
    CXXFLAGS += -DTARGET_DEVICE
endif

BUILD_DIR = build
TARGET = $(BUILD_DIR)/lcd_driver

SRCS = $(wildcard *.cpp) \
       $(wildcard hal/*.cpp) \
       $(wildcard display/*.cpp) \
       $(wildcard input/*.cpp) \
       $(wildcard framework/*.cpp) \
       $(wildcard framework/commands/*.cpp) \
       $(wildcard ipc/*.cpp) \
       $(wildcard util/*.cpp)

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
