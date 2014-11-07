RM = rm -f

CCX = g++
CFLAGS = -O3 -std=c++11 -pthread \
			#-DTEST_BASE_CASE
LDFLAGS = -L$(PREFIX)/lib -ljsoncpp

SRCDIR = src
OBJDIR = obj
BINDIR = bin
LIBDIR = lib

FILES = Directed_weighted_graph.hpp Bellman_Ford_algorithms.hpp common.hpp

DEPS = $(FILES:%.hpp=$(SRCDIR)/%.hpp)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
EXEC = $(BINDIR)/driver
LIB = $(LIBDIR)/libcurrenciesarbitrage.so

all: $(EXEC)
lib: $(LIB)

$(EXEC): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CCX) $(CFLAGS) $^ $(LDFLAGS) -o $@ 

$(LIB): $(OBJECTS)
	@mkdir -p $(LIBDIR)
	$(CCX) $(OBJDIR)/common.o $(LDFLAGS) -shared -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	@mkdir -p $(OBJDIR)
	$(CCX) -fPIC $(CFLAGS) -c $< -o $@ 

clean:
	$(RM) -r $(BINDIR) 
	$(RM) -r $(OBJDIR)
	$(RM) -r $(LIBDIR)

.PHONY: all clean
