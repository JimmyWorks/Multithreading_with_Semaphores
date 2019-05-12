#   Exploring Multiple Threads and Semaphores
#
#   Author: Jimmy Nguyen
#   Email:  Jimmy@JimmyWorks.net
#
#   Commands:
#   make		Make all executables.
#   make clean		Clean all intermediate files
#   make test -i	Test the program in command terminal ignoring errors
#   make backup 	Make a backup of the current project

# Project name for make backup
PROJECTNAME = OS_Concepts_Project2

# Filenames
 # Source
SRCS = main.cc \
       customer.cc \
       bellhop.cc \
       frontdesk.cc
 # Executables
EXE = program.exe 
 # Input Files
#INPUT1 = sample1.txt

 # Directories 
INPUTDIR = ../input/
OUTPUTDIR = ../output/
BIN_DIR = ../bin/
SRC_DIR = ../src/

# Compilers and Flags

CXX = g++
CXXFLAGS =  -pthread -I../include/ -std=c++11 
CPPFLAGS = -I../include/

# Make Targets
OBJS=$(SRCS:cc=o)

 # make
all: $(EXE) 

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)$@ 

 # make clean
clean: backup
	rm -f *.o $(OUTPUTDIR)* *.d*

 # make test
test: $(all)
	@clear
	@clear
	$(BIN_DIR)$(EXE)

Makefile: $(SRCS:.c=.d)

 # Pattern for .d files.
%.d:%.cc
	@echo Updating .d Dependency File
	@set -e; rm -f $@; \
	$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

 # make backup (Backup Target)
backup: 
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename `pwd`))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME)
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!


 # Include the dependency files
-include $(SRCS:.cc=.d)

