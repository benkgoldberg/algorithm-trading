# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/build"

# Include any dependencies generated for this target.
include CMakeFiles/TwsApiCpp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TwsApiCpp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TwsApiCpp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TwsApiCpp.dir/flags.make

CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.o: CMakeFiles/TwsApiCpp.dir/flags.make
CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.o: /Users/bengoldberg/Documents/Algorithm\ Trading/algorithm-trading/TwsApiCpp/src/EClientSocketBase.cpp
CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.o: CMakeFiles/TwsApiCpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.o -MF CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.o.d -o CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.o -c "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/src/EClientSocketBase.cpp"

CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/src/EClientSocketBase.cpp" > CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.i

CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/src/EClientSocketBase.cpp" -o CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.s

CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.o: CMakeFiles/TwsApiCpp.dir/flags.make
CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.o: /Users/bengoldberg/Documents/Algorithm\ Trading/algorithm-trading/TwsApiCpp/src/EPosixClientSocket.cpp
CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.o: CMakeFiles/TwsApiCpp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.o -MF CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.o.d -o CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.o -c "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/src/EPosixClientSocket.cpp"

CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/src/EPosixClientSocket.cpp" > CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.i

CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/src/EPosixClientSocket.cpp" -o CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.s

# Object files for target TwsApiCpp
TwsApiCpp_OBJECTS = \
"CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.o" \
"CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.o"

# External object files for target TwsApiCpp
TwsApiCpp_EXTERNAL_OBJECTS =

libTwsApiCpp.a: CMakeFiles/TwsApiCpp.dir/src/EClientSocketBase.cpp.o
libTwsApiCpp.a: CMakeFiles/TwsApiCpp.dir/src/EPosixClientSocket.cpp.o
libTwsApiCpp.a: CMakeFiles/TwsApiCpp.dir/build.make
libTwsApiCpp.a: CMakeFiles/TwsApiCpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libTwsApiCpp.a"
	$(CMAKE_COMMAND) -P CMakeFiles/TwsApiCpp.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TwsApiCpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TwsApiCpp.dir/build: libTwsApiCpp.a
.PHONY : CMakeFiles/TwsApiCpp.dir/build

CMakeFiles/TwsApiCpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TwsApiCpp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TwsApiCpp.dir/clean

CMakeFiles/TwsApiCpp.dir/depend:
	cd "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp" "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp" "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/build" "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/build" "/Users/bengoldberg/Documents/Algorithm Trading/algorithm-trading/TwsApiCpp/build/CMakeFiles/TwsApiCpp.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/TwsApiCpp.dir/depend

