LINK.o = $(LINK.cc)
CXXFLAGS += -std=c++23 -Wall -Werror -Wconversion -Wpedantic -fsanitize=address
CPPFLAGS += -Iinclude -MMD -MP
LDLIBS += -lglfw -lglbinding -lglbinding-aux

SRCS = $(shell find src -type f -name '*.cpp')

.PHONY: clean cleanall

01-intro: $(SRCS)

clean:
	$(RM) 01-intro

cleanall: clean
	rm $(shell find . -type f -name '*.d')

-include $(shell find . -type f -name '*.d')
