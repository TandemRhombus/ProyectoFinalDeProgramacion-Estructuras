#ifndef PROYECTOFINALDEPROGRAMACION_ESTRUCTURAS_POSICION_H
#define PROYECTOFINALDEPROGRAMACION_ESTRUCTURAS_POSICION_H



class Position { // Clase para representar una posición (x, y)
public:
    int x, y;
    Position(int x, int y) : x(x - 1), y(y - 1) {} // Ajuste para índices basados en cero

    // Métodos get
    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }
};

#endif
