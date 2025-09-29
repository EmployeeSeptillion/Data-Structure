# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -Iheaders

# Directories
SRCDIR = sources
HEADERDIR = headers
BUILDDIR = build

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

# Target executable
TARGET = resume_matcher

# Default target
all: $(BUILDDIR) $(TARGET)

# Create build directory
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "Build complete! Executable: $(TARGET)"

# Compile source files to object files with header dependencies
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(wildcard $(HEADERDIR)/*.hpp)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILDDIR) $(TARGET)
	@echo "Clean complete!"

# Rebuild everything
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Install dependencies (if needed)
install-deps:
	@echo "No external dependencies required for this C++ project."

# Show help
help:
	@echo "Available targets:"
	@echo "  all       - Build the project (default)"
	@echo "  clean     - Remove build files"
	@echo "  rebuild   - Clean and build"
	@echo "  run       - Build and run the program"
	@echo "  help      - Show this help message"

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: clean all

.PHONY: all clean rebuild run install-deps help debug