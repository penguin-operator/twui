src := $(wildcard src/*.c)
obj := $(patsubst src/%.c,/tmp/%.o,$(src))
lib := lib/libtwui.a
dll := lib/libtwui.so
libdir := lib/

CC_FLAGS := -Iinclude -O0 -g -fPIC -std=c23
LD_FLAGS := -shared
AR_FLAGS := rcs

$(lib) $(dll): | lib
lib:
	mkdir -p ./lib

all: ${lib} ${dll}

clean:
	@rm -rf $(obj) $(libdir)

/tmp/%.o: src/%.c
	@echo "\e[1mBUILD\e[0m $@"
	@gcc $(CC_FLAGS) -c -o $@ $<

$(lib): $(obj)
	@echo "\e[1mBUILD\e[0m $@"
	@ar $(AR_FLAGS) $@ $^

$(dll): $(obj)
	@echo "\e[1mBUILD\e[0m $@"
	@ld $(LD_FLAGS) -o $@ $^

test: $(lib) $(dll)
	@echo "\e[1mBUILD\e[0m /tmp/test"
	@LD_RUN_PATH=./lib gcc -o /tmp/test test/test.c -Iinclude -Llib -ltwui || exit $$?
	@gcc -o /tmp/test test/test.c -Iinclude -Llib -ltwui -static || exit $$?
	@echo "\e[1mRUN\e[0m /tmp/test"
	@LD_LIBRARY_PATH=./lib /tmp/test
	@rm -f /tmp/test
