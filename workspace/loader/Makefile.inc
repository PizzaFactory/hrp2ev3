APPL_COBJS += cli_main.o apploader.o gui.o

include $(APPLDIR)/zmodem/Makefile

include $(TARGETDIR)/dmloader/Makefile
