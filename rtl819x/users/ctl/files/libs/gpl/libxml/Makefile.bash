include $(CTL_DIR)/global.def
all:
	cd libxml2-2.7.7 && bash ./build.sh $(TARGET_PLATFORM) $(TARGET_DIR) $(CROSS)

clean:
	cd libxml2-2.7.7 && bash ./clean.sh $(TARGET_DIR)
