# ==========================================
# Configurações
# ==========================================

CXX = g++
CXXFLAGS = -Wall -Iinclude -O3

TARGET = tsp_solver
BENCHMARK = benchmark_status

# ==========================================
# Detecta o sistema operacional
# ==========================================

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    BENCHMARK := $(BENCHMARK).exe
    RM = del /Q
else
    RM = rm -f
endif

# ==========================================
# Regras
# ==========================================

all: $(TARGET) $(BENCHMARK)

# ------------------------------------------
# Solver principal
# ------------------------------------------

$(TARGET): src/main.cpp src/solucao.cpp src/data.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp src/solucao.cpp src/data.cpp -o $(TARGET)

# ------------------------------------------
# Benchmark
# ------------------------------------------

$(BENCHMARK): src/benchmark_status.cpp src/solucao.cpp src/data.cpp
	$(CXX) $(CXXFLAGS) src/benchmark_status.cpp src/solucao.cpp src/data.cpp -o $(BENCHMARK)

# ------------------------------------------
# Limpeza
# ------------------------------------------

clean:
	$(RM) $(TARGET) $(BENCHMARK)