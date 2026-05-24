DRIVER_DIR = ../C++_LCD_Driver
DRIVER_DIST = $(DRIVER_DIR)/dist

SDK_DIR     = sdk
SDK_INC_DIR = $(SDK_DIR)/include
SDK_LIB_DIR = $(SDK_DIR)/lib
SDK_LIB     = $(SDK_LIB_DIR)/libsdk.a

EXAMPLES = hello counter

.PHONY: all clean distclean sync-sdk $(EXAMPLES)

all: $(EXAMPLES)

$(EXAMPLES):
	$(MAKE) -C $@

sync-sdk:
	$(MAKE) -C $(DRIVER_DIR) sdk
	@mkdir -p $(SDK_INC_DIR) $(SDK_LIB_DIR)
	cp -r $(DRIVER_DIST)/include/. $(SDK_INC_DIR)/
	cp    $(DRIVER_DIST)/lib/libsdk.a $(SDK_LIB)
	@echo "[sync-sdk] example/sdk 갱신 완료"

clean:
	@for d in $(EXAMPLES); do $(MAKE) -C $$d clean; done

distclean: clean
	rm -rf $(SDK_DIR)
