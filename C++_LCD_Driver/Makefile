CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Iinterface -Ihal -Idisplay -Iinput -Iutil -Iframework -Iipc -Ishared

PLATFORM ?= host
ifeq ($(PLATFORM),device)
    CXXFLAGS += -DTARGET_DEVICE
endif

BUILD_DIR = build
DIST_DIR  = dist

# --- paths injected at compile time ---
APP_MANAGER_PATH ?= $(abspath $(BUILD_DIR)/app_manager)
APPS_CONF_PATH   ?= $(abspath app_manager/apps.conf)

SERVER_DEFINES = -DAPP_MANAGER_PATH='"$(APP_MANAGER_PATH)"'
APPMGR_DEFINES = -DAPPS_CONF_PATH='"$(APPS_CONF_PATH)"'

# --- server (init) ---
SERVER_BIN  = $(BUILD_DIR)/lcd_driver
SERVER_SRCS = main.cpp \
              $(wildcard hal/*.cpp) \
              $(wildcard display/*.cpp) \
              $(wildcard input/*.cpp) \
              $(wildcard framework/*.cpp) \
              $(wildcard framework/commands/*.cpp) \
              $(wildcard ipc/*.cpp) \
              $(wildcard util/*.cpp)
SERVER_OBJS = $(addprefix $(BUILD_DIR)/, $(SERVER_SRCS:.cpp=.o))

# --- sdk static library ---
SDK_LIB  = $(DIST_DIR)/lib/libsdk.a
SDK_SRCS = $(wildcard sdk/*.cpp) \
           $(wildcard sdk/internal/*.cpp) \
           ipc/UdsClient.cpp \
           ipc/UdsChannel.cpp
SDK_OBJS = $(addprefix $(BUILD_DIR)/, $(SDK_SRCS:.cpp=.o))

# --- public headers exported to dist ---
PUBLIC_HDRS = $(wildcard sdk/*.hpp)
DIST_HDRS   = $(patsubst sdk/%.hpp,$(DIST_DIR)/include/sdk/%.hpp,$(PUBLIC_HDRS))

# --- app_manager (bundled with server, fork/exec'd at boot) ---
APPMGR_BIN  = $(BUILD_DIR)/app_manager
APPMGR_SRCS = $(wildcard app_manager/*.cpp) \
              $(wildcard app_manager/FileReader/*.cpp)
APPMGR_OBJS = $(addprefix $(BUILD_DIR)/, $(APPMGR_SRCS:.cpp=.o))

.PHONY: all clean server sdk app_manager
all: server sdk app_manager
server:      $(SERVER_BIN)
sdk:         $(SDK_LIB) $(DIST_HDRS)
app_manager: $(APPMGR_BIN)

# server objects need APP_MANAGER_PATH define (only main.cpp uses it,
# but applying to all server objs is harmless)
$(SERVER_BIN): $(SERVER_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/main.o: main.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(SERVER_DEFINES) -c $< -o $@

$(SDK_LIB): $(SDK_OBJS)
	@mkdir -p $(@D)
	ar rcs $@ $^

$(DIST_DIR)/include/sdk/%.hpp: sdk/%.hpp
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

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)
