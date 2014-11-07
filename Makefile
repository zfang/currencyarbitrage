RM = rm -f

CCX = g++
CFLAGS = -O2 -std=c++11 \
			#-DTEST_BASE_CASE
LDFLAGS = -L$(PREFIX)/lib -ljsoncpp

SRCDIR = src
OBJDIR = obj
BINDIR = bin
LIBDIR = lib

FILES = Directed_weighted_graph.hpp Bellman_Ford_algorithms.hpp

DEPS = $(FILES:%.hpp=$(SRCDIR)/%.hpp)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
EXEC = $(BINDIR)/driver
LIB = $(LIBDIR)/libcurrenciesarbitrage.so

all: $(EXEC)
lib: $(LIB)

$(EXEC): $(OBJDIR)/main.o
	@mkdir -p $(BINDIR)
	$(CCX) $(CFLAGS) $^ $(LDFLAGS) -o $@ 

$(LIB):
	@mkdir -p $(LIBDIR)
	$(CCX) -fPIC $(CFLAGS) $(SRCDIR)/lib.cpp $(LDFLAGS) -shared -o $@

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(DEPS)
	@mkdir -p $(OBJDIR)
	$(CCX) $(CFLAGS) -c $< -o $@ 

clean:
	$(RM) -r $(BINDIR) 
	$(RM) -r $(OBJDIR)
	$(RM) -r $(LIBDIR)

.PHONY: all clean
