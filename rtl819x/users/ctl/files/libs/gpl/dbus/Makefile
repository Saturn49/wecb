include $(CTL_DIR)/global.def
all:
ifeq ($(TARGET_PLATFORM),pk5000)
	make -C dbus-1.0.3
else
ifeq ($(TARGET_PLATFORM),bhr1)
	make -C dbus-0.36.2
else
	make -C dbus-1.4.0
endif
endif

ifeq ($(TARGET_PLATFORM),bhr2)
bhr2clean:
else
ifeq ($(TARGET_PLATFORM),bhr1)
bhr1clean:
else
clean:
endif
endif
	make -C dbus-1.4.0 clean
	make -C dbus-1.0.3 clean
	make -C dbus-0.36.2 clean
