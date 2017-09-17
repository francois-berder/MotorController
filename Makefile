SOURCES = src/main.c src/status.c src/radio.c src/motor.c
INCLUDES := -Isrc/
OBJDIR := obj/
OUTDIR := bin/
debug: DEBUG = 1

CHIP := 16f1825
FRAMEWORK_OUTDIR := framework/bin/pic$(CHIP)
INCLUDES += -Iframework/mcu/include -Iframework/mcu/pic$(CHIP) -Isrc

all: release

release: $(OUTDIR)/firmware.hex
debug: $(OUTDIR)/firmware.hex

$(OUTDIR)/firmware.hex: $(FRAMEWORK_OUTDIR)/framework.lpp $(SOURCES)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OUTDIR)
	xc8 --chip=$(CHIP) -o$@ \
		--objdir=$(OBJDIR) --outdir=$(OUTDIR) \
		$^ $(INCLUDES) $(if $(DEBUG),,-DNDEBUG)
	$(if $(DEBUG),, ./writebuildinfo.py $@)

$(FRAMEWORK_OUTDIR)/framework.lpp:
	$(MAKE) --directory=framework/ CHIP=$(CHIP) $(if $(DEBUG),debug, release)

.PHONY: clean
clean:
	$(MAKE) --directory=framework CHIP=$(CHIP) clean
	rm -Rf $(OUTDIR)
	rm -Rf $(OBJDIR)
