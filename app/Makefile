CXX = g++

# SDK 위치 — 외부 환경에서 가져다 쓰는 형태
SDK_DIR     = ../C++_LCD_Driver/dist
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
TARGET    = $(BUILD_DIR)/lcd_app

SRCS = $(wildcard *.cpp)
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))

.PHONY: all clean sdk
all: $(TARGET)

# sdk 가 없으면 driver Makefile 호출해서 만들고 옴
$(SDK_LIB):
	$(MAKE) -C ../C++_LCD_Driver sdk

sdk: $(SDK_LIB)

$(TARGET): $(OBJS) $(SDK_LIB)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.cpp $(SDK_LIB)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
