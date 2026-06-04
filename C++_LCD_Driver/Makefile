CXX = g++
CXXFLAGS = -std=c++17 -Wall -I. -MMD -pthread

PLATFORM ?= host
ifeq ($(PLATFORM),device)
    CXXFLAGS += -DTARGET_DEVICE
endif

BUILD_DIR = build
DIST_DIR  = dist

# --- paths injected at compile time ---
APP_MANAGER_PATH ?= $(abspath $(BUILD_DIR)/bin/app_manager)
APPS_DIR_PATH    ?= $(abspath app_manager/apps.d)

SERVER_DEFINES = -DAPP_MANAGER_PATH='"$(APP_MANAGER_PATH)"'
APPMGR_DEFINES = -DAPPS_DIR_PATH='"$(APPS_DIR_PATH)"'

# --- server (init) ---
SERVER_BIN  = $(BUILD_DIR)/lcd_driver
SERVER_SRCS = main.cpp \
              $(wildcard osal/*.cpp) \
              $(wildcard osal/ipc/*.cpp) \
              $(wildcard osal/driver/display/*.cpp) \
              $(wildcard osal/driver/input/*.cpp) \
              $(wildcard osal/process/*.cpp) \
              $(wildcard framework/*.cpp) \
              $(wildcard framework/ipc/*.cpp) \
              $(wildcard framework/commands/*.cpp) \
              $(wildcard util/*.cpp)
SERVER_OBJS = $(addprefix $(BUILD_DIR)/, $(SERVER_SRCS:.cpp=.o))

# --- sdk static library ---
SDK_LIB  = $(DIST_DIR)/lib/libsdk.a
SDK_SRCS = $(wildcard sdk/*.cpp) \
           $(wildcard sdk/internal/*.cpp) \
           $(wildcard sdk/internal/ipc/*.cpp) \
           osal/ipc/UdsClient.cpp \
           osal/ipc/UdsChannel.cpp \
           util/IpcSerializer.cpp \
           util/Font16.cpp
SDK_OBJS = $(addprefix $(BUILD_DIR)/, $(SDK_SRCS:.cpp=.o))

# --- public headers exported to dist ---
SDK_HDRS  = $(wildcard sdk/*.hpp)
EXT_HDRS  = shared/KeyEvent.hpp shared/Screen.hpp util/Font.hpp util/span.hpp util/color.hpp
DIST_HDRS = $(patsubst %,$(DIST_DIR)/include/%,$(SDK_HDRS) $(EXT_HDRS))

# --- app_manager ---
APPMGR_BIN  = $(BUILD_DIR)/bin/app_manager
APPMGR_SRCS = $(wildcard app_manager/*.cpp) \
              $(wildcard app_manager/FileReader/*.cpp) \
              $(wildcard osal/process/*.cpp)
APPMGR_OBJS = $(addprefix $(BUILD_DIR)/, $(APPMGR_SRCS:.cpp=.o))

.PHONY: all clean server sdk app_manager
all: server sdk app_manager
server:      $(SERVER_BIN)
sdk:         $(SDK_LIB) $(DIST_HDRS)
app_manager: $(APPMGR_BIN)

$(SERVER_BIN): $(SERVER_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/main.o: main.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(SERVER_DEFINES) -c $< -o $@

$(SDK_LIB): $(SDK_OBJS)
	@mkdir -p $(@D)
	rm -f $@
	ar rcs $@ $^

# Generic header copy (handles sdk/, shared/, util/)
$(DIST_DIR)/include/%.hpp: %.hpp
	@mkdir -p $(@D)
	cp $< $@

$(APPMGR_BIN): $(APPMGR_OBJS) $(SDK_LIB) $(DIST_HDRS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(DIST_DIR)/include -o $@ $(APPMGR_OBJS) -L$(DIST_DIR)/lib -lsdk

$(BUILD_DIR)/app_manager/%.o: app_manager/%.cpp $(DIST_HDRS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(APPMGR_DEFINES) -I$(DIST_DIR)/include -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(SERVER_OBJS:.o=.d) $(SDK_OBJS:.o=.d) $(APPMGR_OBJS:.o=.d)

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)
