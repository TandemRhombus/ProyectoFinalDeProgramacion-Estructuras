#include "GridViewer.h"
#include <iostream>
#include <vector>


void BasicGridPrinter::print(const Grid &grid) const {
    // Aquí reutilizamos la lógica de tu función original 'printGrid'
    const char horizontalWall = '-';
    const char verticalWall = '|';
    const char corner = '+';

// Dibujar la parte superior del grid
    cout << corner;
    for (int j = 0; j < grid.getN(); ++j) {
        cout << horizontalWall << horizontalWall << horizontalWall << corner;
    }
    cout << endl;

// Dibujar las filas del grid
    for (int i = 0; i < grid.getM(); ++i) {
        cout << verticalWall;
        for (int j = 0; j < grid.getN(); ++j) {
            cout << ' ' << grid.getCell(i, j) << ' ' << verticalWall;
        }
        cout << endl;

        cout << corner;
        for (int j = 0; j < grid.getN(); ++j) {
            cout << horizontalWall << horizontalWall << horizontalWall << corner;
        }
        cout << endl;
    }
}
