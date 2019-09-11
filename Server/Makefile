#Directories
SRCDIR	:= src/
HDRDIR	:= src/headers/
LIBDIR  := lib/
INCDIR  := src/include/
OBJDIR	:= obj/
SUBDIRS := $(OBJDIR)

#Files
SOURCES := $(shell find $(SRCDIR) -name '*.cpp')
OBJECTS := $(subst $(SRCDIR),$(OBJDIR),$(SOURCES:%.cpp=%.o))
BINARY 	:= SERVER

#Compiler set-up
CC		:= g++
LDFLAGS := -Wl,-rpath=$(LIBDIR)

INCLUDE := -I$(HDRDIR) -I$(INCDIR)raknet/raknet
LIBS	:= -L$(LIBDIR) -lraknet -lRakNetLibStatic
#Make binary
$(BINARY): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(INCLUDE) $(LIBS)

#Make objects
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	make setup
	$(CC) -o $@ -c $^ $(INCLUDE)

#Create object directories
setup:
	mkdir -p $(SUBDIRS)

#Deletes object files
clean:
	rm -R -f $(OBJDIR)
	rm -f $(BINARY)
	rm -f $(BINARY).exe

#Makes binary (previous clean)
cleanc:
	make clean
	make

#Runs after compiling
run:
	make
	./$(BINARY)

#Cleans, compiles and runs
cleanr:
	make clean
	make
	./$(BINARY)

#Prints sources, objects and headers lists
info:
	$(info $(SOURCES))
	$(info $(OBJECTS))
	$(info $(INCLUDE))
	$(info $(LIBS))