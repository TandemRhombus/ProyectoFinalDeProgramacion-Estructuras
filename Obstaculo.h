#ifndef PROYECTOFINALDEPROGRAMACION_ESTRUCTURAS_OBSTACULO_H
#define PROYECTOFINALDEPROGRAMACION_ESTRUCTURAS_OBSTACULO_H

#include "Posicion.h"

class Obstacle { // Clase para representar un obstáculo (esquina superior izquierda y esquina inferior derecha)
public:
    Position topLeft, bottomRight; // Posición de la esquina superior izquierda y posición de la esquina inferior derecha
    Obstacle(Position tl, Position br) : topLeft(tl), bottomRight(br) {} // Inicializar obstáculo con la posición de la esquina superior izquierda y la posición de la esquina inferior derecha

    bool overlaps(const Obstacle& other) const; // Función para verificar si un obstáculo se superpone con otro
};

#endif
