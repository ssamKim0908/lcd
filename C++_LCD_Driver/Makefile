CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Iinterface -Ihal -Idisplay -Iinput -Iutil -Iframework -Iipc -Ishared

PLATFORM ?= host
ifeq ($(PLATFORM),device)
    CXXFLAGS += -DTARGET_DEVICE
endif

BUILD_DIR = build
DIST_DIR  = dist

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

.PHONY: all clean server sdk
all: server sdk
server: $(SERVER_BIN)
sdk:    $(SDK_LIB) $(DIST_HDRS)

$(SERVER_BIN): $(SERVER_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SDK_LIB): $(SDK_OBJS)
	@mkdir -p $(@D)
	ar rcs $@ $^

$(DIST_DIR)/include/sdk/%.hpp: sdk/%.hpp
	@mkdir -p $(@D)
	cp $< $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)
