# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/young/software/bootcp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/young/software/bootcp/build

# Include any dependencies generated for this target.
include CMakeFiles/bootcp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bootcp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bootcp.dir/flags.make

CMakeFiles/bootcp.dir/src/bootcp.cpp.o: CMakeFiles/bootcp.dir/flags.make
CMakeFiles/bootcp.dir/src/bootcp.cpp.o: ../src/bootcp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/young/software/bootcp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bootcp.dir/src/bootcp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bootcp.dir/src/bootcp.cpp.o -c /home/young/software/bootcp/src/bootcp.cpp

CMakeFiles/bootcp.dir/src/bootcp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bootcp.dir/src/bootcp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/young/software/bootcp/src/bootcp.cpp > CMakeFiles/bootcp.dir/src/bootcp.cpp.i

CMakeFiles/bootcp.dir/src/bootcp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bootcp.dir/src/bootcp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/young/software/bootcp/src/bootcp.cpp -o CMakeFiles/bootcp.dir/src/bootcp.cpp.s

CMakeFiles/bootcp.dir/src/client.cpp.o: CMakeFiles/bootcp.dir/flags.make
CMakeFiles/bootcp.dir/src/client.cpp.o: ../src/client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/young/software/bootcp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/bootcp.dir/src/client.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bootcp.dir/src/client.cpp.o -c /home/young/software/bootcp/src/client.cpp

CMakeFiles/bootcp.dir/src/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bootcp.dir/src/client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/young/software/bootcp/src/client.cpp > CMakeFiles/bootcp.dir/src/client.cpp.i

CMakeFiles/bootcp.dir/src/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bootcp.dir/src/client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/young/software/bootcp/src/client.cpp -o CMakeFiles/bootcp.dir/src/client.cpp.s

CMakeFiles/bootcp.dir/src/server.cpp.o: CMakeFiles/bootcp.dir/flags.make
CMakeFiles/bootcp.dir/src/server.cpp.o: ../src/server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/young/software/bootcp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/bootcp.dir/src/server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bootcp.dir/src/server.cpp.o -c /home/young/software/bootcp/src/server.cpp

CMakeFiles/bootcp.dir/src/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bootcp.dir/src/server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/young/software/bootcp/src/server.cpp > CMakeFiles/bootcp.dir/src/server.cpp.i

CMakeFiles/bootcp.dir/src/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bootcp.dir/src/server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/young/software/bootcp/src/server.cpp -o CMakeFiles/bootcp.dir/src/server.cpp.s

CMakeFiles/bootcp.dir/src/simplemsg.cpp.o: CMakeFiles/bootcp.dir/flags.make
CMakeFiles/bootcp.dir/src/simplemsg.cpp.o: ../src/simplemsg.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/young/software/bootcp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/bootcp.dir/src/simplemsg.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bootcp.dir/src/simplemsg.cpp.o -c /home/young/software/bootcp/src/simplemsg.cpp

CMakeFiles/bootcp.dir/src/simplemsg.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bootcp.dir/src/simplemsg.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/young/software/bootcp/src/simplemsg.cpp > CMakeFiles/bootcp.dir/src/simplemsg.cpp.i

CMakeFiles/bootcp.dir/src/simplemsg.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bootcp.dir/src/simplemsg.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/young/software/bootcp/src/simplemsg.cpp -o CMakeFiles/bootcp.dir/src/simplemsg.cpp.s

# Object files for target bootcp
bootcp_OBJECTS = \
"CMakeFiles/bootcp.dir/src/bootcp.cpp.o" \
"CMakeFiles/bootcp.dir/src/client.cpp.o" \
"CMakeFiles/bootcp.dir/src/server.cpp.o" \
"CMakeFiles/bootcp.dir/src/simplemsg.cpp.o"

# External object files for target bootcp
bootcp_EXTERNAL_OBJECTS =

libbootcp.a: CMakeFiles/bootcp.dir/src/bootcp.cpp.o
libbootcp.a: CMakeFiles/bootcp.dir/src/client.cpp.o
libbootcp.a: CMakeFiles/bootcp.dir/src/server.cpp.o
libbootcp.a: CMakeFiles/bootcp.dir/src/simplemsg.cpp.o
libbootcp.a: CMakeFiles/bootcp.dir/build.make
libbootcp.a: CMakeFiles/bootcp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/young/software/bootcp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library libbootcp.a"
	$(CMAKE_COMMAND) -P CMakeFiles/bootcp.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bootcp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bootcp.dir/build: libbootcp.a

.PHONY : CMakeFiles/bootcp.dir/build

CMakeFiles/bootcp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bootcp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bootcp.dir/clean

CMakeFiles/bootcp.dir/depend:
	cd /home/young/software/bootcp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/young/software/bootcp /home/young/software/bootcp /home/young/software/bootcp/build /home/young/software/bootcp/build /home/young/software/bootcp/build/CMakeFiles/bootcp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bootcp.dir/depend

