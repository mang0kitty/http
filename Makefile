all: $(notdir $(CURDIR)) web-client web-server

web-client: $(subst .cpp,.o,$(wildcard *.cpp))
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o "web-client" -pthread

web-server: $(subst .cpp,.o,$(wildcard *.cpp))
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o "web-server" -pthread

$(notdir $(CURDIR)): $(subst .cpp,.o,$(wildcard *.cpp))
		$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o "$@" -pthread