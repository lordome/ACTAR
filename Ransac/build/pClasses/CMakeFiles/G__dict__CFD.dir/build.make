# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lorenzo/Desktop/ARDAC/ARDA

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lorenzo/Desktop/ARDAC/build

# Utility rule file for G__dict__CFD.

# Include the progress variables for this target.
include pClasses/CMakeFiles/G__dict__CFD.dir/progress.make

pClasses/CMakeFiles/G__dict__CFD: pClasses/G__dict__CFD.cxx
pClasses/CMakeFiles/G__dict__CFD: pClasses/libdict__CFD_rdict.pcm
pClasses/CMakeFiles/G__dict__CFD: pClasses/libdict__CFD.rootmap


pClasses/G__dict__CFD.cxx: /home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc/pCFDLinkDef.h
pClasses/G__dict__CFD.cxx: /home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc/pCFD.h
pClasses/G__dict__CFD.cxx: /home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc/pCFD.h
pClasses/G__dict__CFD.cxx: /home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc/pCFDLinkDef.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/lorenzo/Desktop/ARDAC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating G__dict__CFD.cxx, libdict__CFD_rdict.pcm, libdict__CFD.rootmap"
	cd /home/lorenzo/Desktop/ARDAC/build/pClasses && /usr/bin/cmake -E env LD_LIBRARY_PATH=/home/lorenzo/root/lib:/home/root/lib/:/home/lorenzo/root/lib:/home/root/lib/:/home/lorenzo/root/lib /home/lorenzo/root/bin/rootcling -v2 -f G__dict__CFD.cxx -s /home/lorenzo/Desktop/ARDAC/build/pClasses/libdict__CFD.so -rml libdict__CFD.so -rmf /home/lorenzo/Desktop/ARDAC/build/pClasses/libdict__CFD.rootmap -I/home/lorenzo/root/include -I/home/lorenzo/Desktop/ARDAC/ARDA/inc -I/home/lorenzo/Desktop/ARDAC/ARDA/libMFM -I/home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc -I/home/lorenzo/Desktop/ARDAC/ARDA/leuvenPSA/inc -I/home/lorenzo/Desktop/ARDAC/ARDA/pClasses pCFD.h /home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc/pCFDLinkDef.h

pClasses/libdict__CFD_rdict.pcm: pClasses/G__dict__CFD.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate pClasses/libdict__CFD_rdict.pcm

pClasses/libdict__CFD.rootmap: pClasses/G__dict__CFD.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate pClasses/libdict__CFD.rootmap

G__dict__CFD: pClasses/CMakeFiles/G__dict__CFD
G__dict__CFD: pClasses/G__dict__CFD.cxx
G__dict__CFD: pClasses/libdict__CFD_rdict.pcm
G__dict__CFD: pClasses/libdict__CFD.rootmap
G__dict__CFD: pClasses/CMakeFiles/G__dict__CFD.dir/build.make

.PHONY : G__dict__CFD

# Rule to build all files generated by this target.
pClasses/CMakeFiles/G__dict__CFD.dir/build: G__dict__CFD

.PHONY : pClasses/CMakeFiles/G__dict__CFD.dir/build

pClasses/CMakeFiles/G__dict__CFD.dir/clean:
	cd /home/lorenzo/Desktop/ARDAC/build/pClasses && $(CMAKE_COMMAND) -P CMakeFiles/G__dict__CFD.dir/cmake_clean.cmake
.PHONY : pClasses/CMakeFiles/G__dict__CFD.dir/clean

pClasses/CMakeFiles/G__dict__CFD.dir/depend:
	cd /home/lorenzo/Desktop/ARDAC/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lorenzo/Desktop/ARDAC/ARDA /home/lorenzo/Desktop/ARDAC/ARDA/pClasses /home/lorenzo/Desktop/ARDAC/build /home/lorenzo/Desktop/ARDAC/build/pClasses /home/lorenzo/Desktop/ARDAC/build/pClasses/CMakeFiles/G__dict__CFD.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : pClasses/CMakeFiles/G__dict__CFD.dir/depend

