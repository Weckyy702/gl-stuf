LINK.o = $(LINK.cc)

CXXFLAGS += -std=c++23 -Wall -Werror -Wconversion -Wpedantic
CPPFLAGS += -Iinclude -MMD -MP
LDLIBS += -lglfw -lGL

SRCS = $(shell find src -type f -name '*.cpp')

01-intro: $(SRCS)

-include $(shell find . -type f -name '*.d')
