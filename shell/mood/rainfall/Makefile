# ---- Configs ----
SHELL        := /bin/bash
SCRIPT       := rainfall.sh
ASSETS_DIR   := assets
CAST_FILE    := $(ASSETS_DIR)/demo.cast
CAST_FILE_DG := $(ASSETS_DIR)/demo.v2.cast # downgraded for agg compatibilities
GIF_FILE     := $(ASSETS_DIR)/demo.gif

SHELLCHECK   := $(shell command -v shellcheck 2>/dev/null)
ASCIINEMA    := $(shell command -v asciinema 2>/dev/null)
AGG 			   := $(shell command -v agg 2>/dev/null) # https://github.com/asciinema/agg
DUR 				 ?= 8 # Default recording time

DEFAULT_GOAL := run


.PHONY: all run install lint gif record clean help test

all: run

help:
	@printf "%s\n" \
"Targets:" \
"  run     - execute the rainfall" \
"  install - make exec" \
"  lint    - run shellcheck (if install)" \
"  record  - guide to record a terminal cast with asciinema -> $(CAST_FILE)" \
"  gif     - convert $(CAST_FILE) -> $(GIF_FILE) using agg" \
"  clean   - remove generated GIF and cast"


run:
	chmod +x $(SCRIPT)
	./$(SCRIPT)



install:
	chmod +x $(SCRIPT)
	@echo " Installed executable bit on $(SCRIPT). Run with: ./$(SCRIPT) "



lint:
ifdef SHELLCHECK
	$(SHELLCHECK) $(SCRIPT)
else
	@echo "shellcheck not found. Install shellcheck first"
	@echo " - macOS (brew):  brew install shellcheck"
	@echo " - Debian:        sudo apt-get install shellcheck"
	@echo " - Arch:          sudo pacman -S shellcheck"
	@exit 0
endif



record: 
ifdef ASCIINEMA
	@echo "Recording with asciinema. You'll press Ctrl-D to finish."
	@echo "The cast will be saved to $(CAST_FILE)."
	@mkdir -p $(ASSETS_DIR)
	@$(ASCIINEMA) rec -q -c "timeout ${DUR} ./$(SCRIPT)" $(CAST_FILE)
	@echo "Finish recording!"
else
	@echo "asciinema not found."
	@echo "Install it: https://asciinema.org/"
	@echo "Then run: make record"
	@exit 0
endif



gif: $(GIF_FILE)
$(CAST_FILE_DG) : $(CAST_FILE)
ifdef ASCIINEMA
	@echo "Converting $(CAST_FILE) (v3) -> $(CAST_FILE_DG) (v2) . . ."
	@asciinema convert -f asciicast-v2 $(CAST_FILE) $(CAST_FILE_DG)
else
	@echo "asciinema not found."
	@echo "Install it: https://asciinema.org/"
	@echo "Then run: make record"
	@exit 0
endif

$(GIF_FILE): $(CAST_FILE_DG)
ifdef AGG
	@echo "Converting $(CAST_FILE_DG) -> $(GIF_FILE) ..."
	@$(AGG) --cols 80 --rows 24 $(CAST_FILE_DG) $(GIF_FILE)
	@echo "Done. Embed in README: ![demo]($(GIF_FILE))"
else
	@echo "agg not found."
	@echo "Install it: https://github.com/asciinema/agg"
	@echo "Then run: make gif"
	@exit 0
endif



clean:
	@rm -f $(GIF_FILE) $(CAST_FILE)
	@echo "Cleaned generated media."



test:
	@echo "Running basic tests for ${SCRIPT}"
	@command -v tput >/dev/null || (echo "tput not found" && exit 1)
	@command -v clear >/dev/null || (echo "clear not found" && exit 1)
	@echo "Running script briefly . . ."
	@timeout 1 ./${SCRIPT} >/dev/null 2>&1 || echo "Script exited with an error"
	@echo "Test completed"

