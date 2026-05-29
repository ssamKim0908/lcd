CXX = g++
SDK_DIR     = ../sdk
SDK_INC     = $(SDK_DIR)/include
SDK_LIB_DIR = $(SDK_DIR)/lib
SDK_LIB     = $(SDK_LIB_DIR)/libsdk.a

CXXFLAGS = -std=c++17 -Wall -I$(SDK_INC)
LDFLAGS  = -L$(SDK_LIB_DIR) -lsdk

BUILD_DIR = build
TARGET    = $(BUILD_DIR)/stress

SRCS = $(wildcard *.cpp)
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))

APPS_D_DIR ?= ../../C++_LCD_Driver/app_manager/apps.d
APP_NAME   ?= Stress
REG_FILE    = $(APPS_D_DIR)/$(APP_NAME).conf

.PHONY: all clean register unregister
all: $(TARGET)

$(SDK_LIB):
	@echo "ERROR: $(SDK_LIB) 없음. 먼저 'make -C .. PLATFORM=device sync-sdk' 실행하세요." >&2
	@exit 1

$(TARGET): $(OBJS) $(SDK_LIB)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.cpp $(SDK_LIB)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

register: $(TARGET)
	@mkdir -p $(APPS_D_DIR)
	@printf "%s | %s\n" "$(APP_NAME)" "$(abspath $(TARGET))" > $(REG_FILE)
	@echo "[register] $(REG_FILE) <- $(APP_NAME) | $(abspath $(TARGET))"

unregister:
	@rm -f $(REG_FILE)
	@echo "[unregister] $(REG_FILE) 제거"
