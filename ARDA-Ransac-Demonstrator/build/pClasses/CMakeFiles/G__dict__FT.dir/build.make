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

# Utility rule file for G__dict__FT.

# Include the progress variables for this target.
include pClasses/CMakeFiles/G__dict__FT.dir/progress.make

pClasses/CMakeFiles/G__dict__FT: pClasses/G__dict__FT.cxx
pClasses/CMakeFiles/G__dict__FT: pClasses/libdict__FT_rdict.pcm
pClasses/CMakeFiles/G__dict__FT: pClasses/libdict__FT.rootmap


pClasses/G__dict__FT.cxx: /home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc/pFTLinkDef.h
pClasses/G__dict__FT.cxx: /home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc/pFT.h
pClasses/G__dict__FT.cxx: /home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc/pFT.h
pClasses/G__dict__FT.cxx: /home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc/pFTLinkDef.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/lorenzo/Desktop/ARDAC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating G__dict__FT.cxx, libdict__FT_rdict.pcm, libdict__FT.rootmap"
	cd /home/lorenzo/Desktop/ARDAC/build/pClasses && /usr/bin/cmake -E env LD_LIBRARY_PATH=/home/lorenzo/root/lib:/home/root/lib/:/home/lorenzo/root/lib:/home/root/lib/:/home/lorenzo/root/lib /home/lorenzo/root/bin/rootcling -v2 -f G__dict__FT.cxx -s /home/lorenzo/Desktop/ARDAC/build/pClasses/libdict__FT.so -rml libdict__FT.so -rmf /home/lorenzo/Desktop/ARDAC/build/pClasses/libdict__FT.rootmap -I/home/lorenzo/root/include -I/home/lorenzo/Desktop/ARDAC/ARDA/inc -I/home/lorenzo/Desktop/ARDAC/ARDA/libMFM -I/home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc -I/home/lorenzo/Desktop/ARDAC/ARDA/leuvenPSA/inc -I/home/lorenzo/Desktop/ARDAC/ARDA/pClasses pFT.h /home/lorenzo/Desktop/ARDAC/ARDA/pClasses/inc/pFTLinkDef.h

pClasses/libdict__FT_rdict.pcm: pClasses/G__dict__FT.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate pClasses/libdict__FT_rdict.pcm

pClasses/libdict__FT.rootmap: pClasses/G__dict__FT.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate pClasses/libdict__FT.rootmap

G__dict__FT: pClasses/CMakeFiles/G__dict__FT
G__dict__FT: pClasses/G__dict__FT.cxx
G__dict__FT: pClasses/libdict__FT_rdict.pcm
G__dict__FT: pClasses/libdict__FT.rootmap
G__dict__FT: pClasses/CMakeFiles/G__dict__FT.dir/build.make

.PHONY : G__dict__FT

# Rule to build all files generated by this target.
pClasses/CMakeFiles/G__dict__FT.dir/build: G__dict__FT

.PHONY : pClasses/CMakeFiles/G__dict__FT.dir/build

pClasses/CMakeFiles/G__dict__FT.dir/clean:
	cd /home/lorenzo/Desktop/ARDAC/build/pClasses && $(CMAKE_COMMAND) -P CMakeFiles/G__dict__FT.dir/cmake_clean.cmake
.PHONY : pClasses/CMakeFiles/G__dict__FT.dir/clean

pClasses/CMakeFiles/G__dict__FT.dir/depend:
	cd /home/lorenzo/Desktop/ARDAC/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lorenzo/Desktop/ARDAC/ARDA /home/lorenzo/Desktop/ARDAC/ARDA/pClasses /home/lorenzo/Desktop/ARDAC/build /home/lorenzo/Desktop/ARDAC/build/pClasses /home/lorenzo/Desktop/ARDAC/build/pClasses/CMakeFiles/G__dict__FT.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : pClasses/CMakeFiles/G__dict__FT.dir/depend
