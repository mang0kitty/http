all: $(notdir $(CURDIR))

$(notdir $(CURDIR)): $(subst .cpp,.o,$(wildcard *.cpp))
		$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o "$@"