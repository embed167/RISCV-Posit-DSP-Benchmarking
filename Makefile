CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
INCLUDES = -I./include -I./soft-posit-cpp/include
LIBS = -L./soft-posit-cpp/build -lsoftposit

# Default target builds everything
all: check_includes softposit dsp_algo_posit

# Build the SoftPosit library
softposit:
	@echo "Building SoftPosit library..."
	@mkdir -p soft-posit-cpp/build
	@cd soft-posit-cpp/build && cmake .. -DCMAKE_BUILD_TYPE=Release && make
	@echo "Checking for built library..."
	@if [ -f ./soft-posit-cpp/build/libsoftposit.a ] || [ -f ./soft-posit-cpp/build/libsoftposit.so ]; then \
		echo "Library found!"; \
	else \
		echo "Library NOT found at expected location. Searching..."; \
		find ./soft-posit-cpp/build -name "*.a" -o -name "*.so"; \
	fi

# Build DSP algorithm
dsp_algo_posit: softposit
	@echo "Building DSP algorithm with verbose output..."
	$(CXX) -v $(CXXFLAGS) $(INCLUDES) -o dsp_algo_posit src/main.cpp $(LIBS)

# Clean target
clean:
	@echo "Cleaning up..."
	rm -f dsp_algo
	# rm -rf soft-posit-cpp/build

.PHONY: all softposit clean check_includes
