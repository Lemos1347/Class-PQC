# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

# Include paths - assuming liboqs is installed in the standard location
INCLUDES = -I/usr/local/include

# Library paths
LDFLAGS = -L/usr/local/lib

# Libraries to link against
LIBS = -loqs

# Target executable names
TARGETS = alice_kem bob_kem alice2_kem

# Source files
SOURCES_ALICE = alice_kem.cpp
SOURCES_BOB = bob_kem.cpp
SOURCES_ALICE2 = alice2_kem.cpp

# Object files
OBJECTS_ALICE = $(SOURCES_ALICE:.cpp=.o)
OBJECTS_BOB = $(SOURCES_BOB:.cpp=.o)
OBJECTS_ALICE2 = $(SOURCES_ALICE2:.cpp=.o)

# Default target
all: $(TARGETS)

# Linking Alice's executable
alice_kem: $(OBJECTS_ALICE)
	$(CXX) $(OBJECTS_ALICE) $(LDFLAGS) $(LIBS) -o alice_kem

# Linking Bob's executable
bob_kem: $(OBJECTS_BOB)
	$(CXX) $(OBJECTS_BOB) $(LDFLAGS) $(LIBS) -o bob_kem

# Linking Alice2's executable
alice2_kem: $(OBJECTS_ALICE2)
	$(CXX) $(OBJECTS_ALICE2) $(LDFLAGS) $(LIBS) -o alice2_kem

# Compiling source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJECTS_ALICE) $(OBJECTS_BOB) $(OBJECTS_ALICE2) $(TARGETS)

# Phony targets
.PHONY: all clean

