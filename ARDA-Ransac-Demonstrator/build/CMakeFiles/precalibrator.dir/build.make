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

# Include any dependencies generated for this target.
include CMakeFiles/precalibrator.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/precalibrator.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/precalibrator.dir/flags.make

CMakeFiles/precalibrator.dir/src/precalibrator.cc.o: CMakeFiles/precalibrator.dir/flags.make
CMakeFiles/precalibrator.dir/src/precalibrator.cc.o: /home/lorenzo/Desktop/ARDAC/ARDA/src/precalibrator.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lorenzo/Desktop/ARDAC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/precalibrator.dir/src/precalibrator.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/precalibrator.dir/src/precalibrator.cc.o -c /home/lorenzo/Desktop/ARDAC/ARDA/src/precalibrator.cc

CMakeFiles/precalibrator.dir/src/precalibrator.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/precalibrator.dir/src/precalibrator.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lorenzo/Desktop/ARDAC/ARDA/src/precalibrator.cc > CMakeFiles/precalibrator.dir/src/precalibrator.cc.i

CMakeFiles/precalibrator.dir/src/precalibrator.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/precalibrator.dir/src/precalibrator.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lorenzo/Desktop/ARDAC/ARDA/src/precalibrator.cc -o CMakeFiles/precalibrator.dir/src/precalibrator.cc.s

# Object files for target precalibrator
precalibrator_OBJECTS = \
"CMakeFiles/precalibrator.dir/src/precalibrator.cc.o"

# External object files for target precalibrator
precalibrator_EXTERNAL_OBJECTS =

precalibrator: CMakeFiles/precalibrator.dir/src/precalibrator.cc.o
precalibrator: CMakeFiles/precalibrator.dir/build.make
precalibrator: libARDAlib.so
precalibrator: /home/lorenzo/root/lib/libCore.so
precalibrator: /home/lorenzo/root/lib/libImt.so
precalibrator: /home/lorenzo/root/lib/libRIO.so
precalibrator: /home/lorenzo/root/lib/libNet.so
precalibrator: /home/lorenzo/root/lib/libHist.so
precalibrator: /home/lorenzo/root/lib/libGraf.so
precalibrator: /home/lorenzo/root/lib/libGraf3d.so
precalibrator: /home/lorenzo/root/lib/libGpad.so
precalibrator: /home/lorenzo/root/lib/libROOTDataFrame.so
precalibrator: /home/lorenzo/root/lib/libTree.so
precalibrator: /home/lorenzo/root/lib/libTreePlayer.so
precalibrator: /home/lorenzo/root/lib/libRint.so
precalibrator: /home/lorenzo/root/lib/libPostscript.so
precalibrator: /home/lorenzo/root/lib/libMatrix.so
precalibrator: /home/lorenzo/root/lib/libPhysics.so
precalibrator: /home/lorenzo/root/lib/libMathCore.so
precalibrator: /home/lorenzo/root/lib/libThread.so
precalibrator: /home/lorenzo/root/lib/libMultiProc.so
precalibrator: /home/lorenzo/root/lib/libROOTVecOps.so
precalibrator: leuvenPSA/libleuvenPSA.so
precalibrator: libMFM/libMFM.so
precalibrator: pClasses/libpClasses.so
precalibrator: /home/lorenzo/root/lib/libCore.so
precalibrator: /home/lorenzo/root/lib/libImt.so
precalibrator: /home/lorenzo/root/lib/libRIO.so
precalibrator: /home/lorenzo/root/lib/libNet.so
precalibrator: /home/lorenzo/root/lib/libHist.so
precalibrator: /home/lorenzo/root/lib/libGraf.so
precalibrator: /home/lorenzo/root/lib/libGraf3d.so
precalibrator: /home/lorenzo/root/lib/libGpad.so
precalibrator: /home/lorenzo/root/lib/libROOTDataFrame.so
precalibrator: /home/lorenzo/root/lib/libTree.so
precalibrator: /home/lorenzo/root/lib/libTreePlayer.so
precalibrator: /home/lorenzo/root/lib/libRint.so
precalibrator: /home/lorenzo/root/lib/libPostscript.so
precalibrator: /home/lorenzo/root/lib/libMatrix.so
precalibrator: /home/lorenzo/root/lib/libPhysics.so
precalibrator: /home/lorenzo/root/lib/libMathCore.so
precalibrator: /home/lorenzo/root/lib/libThread.so
precalibrator: /home/lorenzo/root/lib/libMultiProc.so
precalibrator: /home/lorenzo/root/lib/libROOTVecOps.so
precalibrator: CMakeFiles/precalibrator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lorenzo/Desktop/ARDAC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable precalibrator"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/precalibrator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/precalibrator.dir/build: precalibrator

.PHONY : CMakeFiles/precalibrator.dir/build

CMakeFiles/precalibrator.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/precalibrator.dir/cmake_clean.cmake
.PHONY : CMakeFiles/precalibrator.dir/clean

CMakeFiles/precalibrator.dir/depend:
	cd /home/lorenzo/Desktop/ARDAC/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lorenzo/Desktop/ARDAC/ARDA /home/lorenzo/Desktop/ARDAC/ARDA /home/lorenzo/Desktop/ARDAC/build /home/lorenzo/Desktop/ARDAC/build /home/lorenzo/Desktop/ARDAC/build/CMakeFiles/precalibrator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/precalibrator.dir/depend
