SOURCES = src/main.c src/status.c
INCLUDES := -Isrc/
OBJDIR := obj/
OUTDIR := bin/

CHIP := 16f1825
FRAMEWORK_OUTDIR := framework/bin/pic$(CHIP)
INCLUDES += -Iframework/mcu/include -Iframework/mcu/pic$(CHIP) -Isrc

all: $(OUTDIR)/firmware.hex

$(OUTDIR)/firmware.hex: $(FRAMEWORK_OUTDIR)/framework.lpp $(SOURCES)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OUTDIR)
	xc8 --chip=$(CHIP) -o$@ \
		--objdir=$(OBJDIR) --outdir=$(OUTDIR) \
		$^ $(INCLUDES)

$(FRAMEWORK_OUTDIR)/framework.lpp:
	make --directory=framework/ CHIP=$(CHIP)

.PHONY: clean
clean:
	make --directory=framework CHIP=$(CHIP) clean
	rm -Rf $(OUTDIR)
	rm -Rf $(OBJDIR)
