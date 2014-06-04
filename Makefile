RM = rm -f

CCX = g++
CFLAGS = -O2 -std=c++11 \
			#-DTEST_BASE_CASE

SRCDIR = src
OBJDIR = obj
BINDIR = bin

FILES = Directed_weighted_graph.hpp Bellman_Ford_algorithms.hpp

DEPS = $(FILES:%.hpp=$(SRCDIR)/%.hpp)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
EXEC = $(BINDIR)/driver

all: $(EXEC)

$(EXEC): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CCX) $(CFLAGS) $^ -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	@mkdir -p $(OBJDIR)
	$(CCX) $(CFLAGS) -c $< -o $@ 

clean:
	$(RM) -r bin
	$(RM) -r obj

.PHONY: all clean
