# Compilador a usar
CXX = g++

# Opciones de compilación (C++17, AddressSanitizer, advertencias y depuración)
CXXFLAGS = -Wall -Wextra -g -fsanitize=address -std=c++17

# Nombre del ejecutable final
TARGET = ExpresionesRegulares

# Busca automáticamente todos los archivos .cc en src
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cc)

# Reemplaza la extensión .cc por .o para generar los archivos objeto
OBJS = $(SRCS:.cc=.o)

# Regla principal
all: $(TARGET)

# Cómo construir el ejecutable final
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Cómo compilar cada archivo fuente (.cc) a código objeto (.o)
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
clean:
	rm -f $(OBJS) $(TARGET)