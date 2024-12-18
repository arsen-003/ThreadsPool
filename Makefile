CXX := g++
CXXFLAGS := -std=c++11 -pthread -fPIC
TARGET := demo-application
LIB := libparallel_scheduler.so  # Ensure the library is named with 'lib' prefix

all: $(LIB) $(TARGET)

# Create shared library
$(LIB): parallel_scheduler.cpp
	$(CXX) $(CXXFLAGS) -shared -o $(LIB) parallel_scheduler.cpp

# Create demo application
$(TARGET): demo.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) demo.cpp -L. -lparallel_scheduler

clean:
	rm -f $(TARGET) $(LIB)
