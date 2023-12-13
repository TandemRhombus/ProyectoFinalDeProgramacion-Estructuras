#ifndef PROYECTOFINALDEPROGRAMACION_ESTRUCTURAS_GRIDVIEWER_H
#define PROYECTOFINALDEPROGRAMACION_ESTRUCTURAS_GRIDVIEWER_H

#include <iostream>
#include <vector>
#include "red.h"

// Clase base abstracta para imprimir el grid
class GridPrinter {
public:
    virtual void print(const Grid& grid) const = 0;
    virtual ~GridPrinter() {}
};

// Clase derivada que implementa la impresión básica del grid
class BasicGridPrinter : public GridPrinter {
public:
    void print(const Grid& grid) const override;
};

#endif
