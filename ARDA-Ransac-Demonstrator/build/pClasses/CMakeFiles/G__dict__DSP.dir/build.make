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
CMAKE_SOURCE_DIR = /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build

# Utility rule file for G__dict__DSP.

# Include the progress variables for this target.
include pClasses/CMakeFiles/G__dict__DSP.dir/progress.make

pClasses/CMakeFiles/G__dict__DSP: pClasses/G__dict__DSP.cxx
pClasses/CMakeFiles/G__dict__DSP: pClasses/libdict__DSP_rdict.pcm
pClasses/CMakeFiles/G__dict__DSP: pClasses/libdict__DSP.rootmap


pClasses/G__dict__DSP.cxx: /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/pClasses/inc/pDSPLinkDef.h
pClasses/G__dict__DSP.cxx: /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/pClasses/inc/pDSP.h
pClasses/G__dict__DSP.cxx: /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/pClasses/inc/pDSP.h
pClasses/G__dict__DSP.cxx: /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/pClasses/inc/pDSPLinkDef.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating G__dict__DSP.cxx, libdict__DSP_rdict.pcm, libdict__DSP.rootmap"
	cd /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/pClasses && /usr/bin/cmake -E env LD_LIBRARY_PATH=/home/lorenzo/root/lib:/home/lorenzo/root/lib:/home/lorenzo/root/lib.:/usr/local/geant4/lib/.:/usr/local/geant4/lib/ /home/lorenzo/root/bin/rootcling -v2 -f G__dict__DSP.cxx -s /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/pClasses/libdict__DSP.so -rml libdict__DSP.so -rmf /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/pClasses/libdict__DSP.rootmap -I/home/lorenzo/root/include -I/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/inc -I/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/libMFM -I/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/pClasses/inc -I/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/leuvenPSA/inc -I/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/pClasses pDSP.h /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/pClasses/inc/pDSPLinkDef.h

pClasses/libdict__DSP_rdict.pcm: pClasses/G__dict__DSP.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate pClasses/libdict__DSP_rdict.pcm

pClasses/libdict__DSP.rootmap: pClasses/G__dict__DSP.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate pClasses/libdict__DSP.rootmap

G__dict__DSP: pClasses/CMakeFiles/G__dict__DSP
G__dict__DSP: pClasses/G__dict__DSP.cxx
G__dict__DSP: pClasses/libdict__DSP_rdict.pcm
G__dict__DSP: pClasses/libdict__DSP.rootmap
G__dict__DSP: pClasses/CMakeFiles/G__dict__DSP.dir/build.make

.PHONY : G__dict__DSP

# Rule to build all files generated by this target.
pClasses/CMakeFiles/G__dict__DSP.dir/build: G__dict__DSP

.PHONY : pClasses/CMakeFiles/G__dict__DSP.dir/build

pClasses/CMakeFiles/G__dict__DSP.dir/clean:
	cd /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/pClasses && $(CMAKE_COMMAND) -P CMakeFiles/G__dict__DSP.dir/cmake_clean.cmake
.PHONY : pClasses/CMakeFiles/G__dict__DSP.dir/clean

pClasses/CMakeFiles/G__dict__DSP.dir/depend:
	cd /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/pClasses /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/pClasses /home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/build/pClasses/CMakeFiles/G__dict__DSP.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : pClasses/CMakeFiles/G__dict__DSP.dir/depend

