# Compilador y banderas de compilación
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Nombre del archivo ejecutable de salida
TARGET = GestionDeTorneo

# Lista de todos los archivos fuente (todos tus "ladrillos" .cpp)
SRCS = main.cpp \
       equipos/Equipo.cpp \
       equipos/OperacionesEquipo.cpp \
       jugadores/Jugador.cpp \
       jugadores/OperacionesJugador.cpp \
       partidos/Gol.cpp \
       partidos/Partido.cpp \
       partidos/OperacionesPartido.cpp \
       torneo/Torneo.cpp \
       utilidades/Formatos.cpp \
       utilidades/Validaciones.cpp \
       interfaz/Interfaz.cpp

# Transforma automáticamente la lista de .cpp a .o (archivos objeto)
OBJS = $(SRCS:.cpp=.o)

# Regla principal: compila todo
all: $(TARGET)

# Regla para unir todos los .o y crear el ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Regla genérica para compilar cualquier .cpp a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar el proyecto de archivos compilados
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe