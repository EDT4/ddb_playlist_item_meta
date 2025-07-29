OUT?=playlist_item_meta.so

CC?=gcc
CFLAGS+=-O3 -Wall -g -fPIC -std=c99 -D_GNU_SOURCE
LDFLAGS+=-shared

SRC_DIR?=src
OUT_DIR?=out

SOURCES?=$(wildcard $(SRC_DIR)/*.c)
OBJS?=$(patsubst $(SRC_DIR)/%.c, $(OUT_DIR)/%.o, $(SOURCES))

define compile
	$(CC) $(CFLAGS) $1 $2 $< -c -o $@
endef

define link
	$(CC) $(LDFLAGS) $1 $2 $3 -o $@
endef

all: out

run: out
	cp out/playlist_item_meta.so ~/.local/lib/deadbeef/
	deadbeef

out: mkdir_out $(SOURCES) $(OUT_DIR)/$(OUT)

mkdir_out:
	@echo "Creating build directory"
	@mkdir -p $(OUT_DIR)

$(OUT_DIR)/$(OUT): $(OBJS)
	@echo "Linking"
	@$(call link, $(OBJS))
	@echo "Stripping"
	@strip -K playlist_item_meta_load $(OUT_DIR)/$(OUT)
	@echo "Done!"

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $(subst $(OUT_DIR)/,,$@)"
	@$(call compile)

clean:
	@echo "Cleaning files from previous build..."
	@rm -r -f $(OUT_DIR)
