all: $(notdir $(CURDIR)) web-client web-server

web-client: $(subst .cpp,.o,$(wildcard *.cpp))
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o "web-client" -pthread

web-server: $(subst .cpp,.o,$(wildcard *.cpp))
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o "web-server" -pthread

web-test: $(subst .cpp,.o,$(wildcard *.cpp))
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o "web-test" -pthread

$(notdir $(CURDIR)): $(subst .cpp,.o,$(wildcard *.cpp))
		$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o "$@" -pthread