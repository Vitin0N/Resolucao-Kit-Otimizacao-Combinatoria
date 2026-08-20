# ==========================================
# Configurações
# ==========================================

CXX = g++
CXXFLAGS = -Wall -Iinclude -O3

TARGET = tsp_solver

# ==========================================
# Detecta o sistema operacional
# ==========================================

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    RM = del /Q
else
    RM = rm -f
endif

# ==========================================
# Regras
# ==========================================

all: $(TARGET)

$(TARGET): src/main.cpp src/solucao.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp src/solucao.cpp -o $(TARGET)

clean:
	$(RM) $(TARGET)