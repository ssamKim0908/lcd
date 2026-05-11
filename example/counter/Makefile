CXX = g++

SDK_DIR     = ../../C++_LCD_Driver/dist
SDK_INC     = $(SDK_DIR)/include
SDK_LIB_DIR = $(SDK_DIR)/lib
SDK_LIB     = $(SDK_LIB_DIR)/libsdk.a

CXXFLAGS = -std=c++17 -Wall -I$(SDK_INC)
LDFLAGS  = -L$(SDK_LIB_DIR) -lsdk

PLATFORM ?= host
ifeq ($(PLATFORM),device)
    CXXFLAGS += -DTARGET_DEVICE
endif

BUILD_DIR = build
TARGET    = $(BUILD_DIR)/counter

SRCS = $(wildcard *.cpp)
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))

.PHONY: all clean sdk
all: $(TARGET)

$(SDK_LIB):
	$(MAKE) -C ../../C++_LCD_Driver sdk

sdk: $(SDK_LIB)

$(TARGET): $(OBJS) $(SDK_LIB)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.cpp $(SDK_LIB)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
