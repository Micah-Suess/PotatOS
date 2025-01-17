# PotatOS - a potato operating system for potato computers
# Created by NicePotato 2022-2025

ARCH ?= NONE
FORCE ?= false

CCDIR := ~/tools/cross-compiler

.PHONY: download-tools
download-tools:
# For some dumb reason I have to hardcode the directory for wget or it doesn't show progress
	@if [ ! -d $(CCDIR)/$(ARCH) ] || [ "$(FORCE)" = "true" ]; then \
		echo Downloading cross-compiler; \
		rm -rf $(CCDIR)/$(ARCH); \
		mkdir -p $(CCDIR); \
		if [ "$(ARCH)" = "x86_64" ]; then \
			wget https://github.com/lordmilko/i686-elf-tools/releases/download/13.2.0/x86_64-elf-tools-linux.zip -O ~/tools/cross-compiler/x86_64.zip; \
		fi; \
		if [ "$(ARCH)" = "i686" ]; then \
			wget https://github.com/lordmilko/i686-elf-tools/releases/download/13.2.0/i686-elf-tools-linux.zip -O ~/tools/cross-compiler/i686.zip; \
		fi; \
		unzip $(CCDIR)/$(ARCH).zip -d $(CCDIR)/$(ARCH); \
		rm -rf $(CCDIR)/$(ARCH).zip; \
	fi