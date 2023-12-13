#ifndef PROYECTOFINALDEPROGRAMACION_ESTRUCTURAS_RED_H
#define PROYECTOFINALDEPROGRAMACION_ESTRUCTURAS_RED_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>
#include <map>
#include "Posicion.h"
#include "Obstaculo.h"

using namespace std;

class Grid { // Clase para representar la matriz de celdas (M x N)
private:
    int M, N;
    vector<vector<char>> matrix;

public:
    Grid(int M, int N) : M(M), N(N), matrix(M, vector<char>(N, 'o')) {} // Inicializar matriz de celdas con 'o's (celdas vacías)

    // Métodos get
    int getM() const { // Función para obtener el número de filas de la matriz de celdas
        return M;
    }

    int getN() const { // Función para obtener el número de columnas de la matriz de celdas
        return N;
    }

    char getCell(int x, int y) const { // Función para obtener el valor de una celda en la matriz de celdas
        return matrix[x][y];
    }

    // Métodos set
    void setCell(int x, int y, char value) { // Función para modificar el valor de una celda en la matriz de celdas
        matrix[x][y] = value;
    }

    void placeTom(const Position& tom) { // Función para colocar a Tom en la matriz de celdas
        matrix[tom.getX()][tom.getY()] = 'T';
    }

    void placeJerry(const Position& jerry) { // Función para colocar a Jerry en la matriz de celdas
        matrix[jerry.getX()][jerry.getY()] = 'J';
    }

    bool addObstacle(const Obstacle& obs) { // Función para agregar un obstáculo a la matriz de celdas (si es válido)
        if (!isPositionValid(obs.topLeft) || !isPositionValid(obs.bottomRight)) {
            return false;
        }

        for (int i = obs.topLeft.getX(); i <= obs.bottomRight.getX(); ++i) {
            for (int j = obs.topLeft.getY(); j <= obs.bottomRight.getY(); ++j) {
                matrix[i][j] = 'x';
            }
        }
        return true;
    }

    bool isPositionValid(const Position& pos) const { // Función para verificar si una posición es válida (está dentro de la matriz de celdas)
        int x = pos.getX();
        int y = pos.getY();
        return x >= 0 && x < M && y >= 0 && y < N;
    }

    string toString() const { // Función para convertir la matriz de celdas a string (para escribirlo en el archivo de salida)
        string output;
        for (const auto& row : matrix) {
            for (char cell : row) {
                output += cell;
            }
            output += '\n';
        }
        return output;
    }

    void markPath(const vector<pair<int, int>>& path) { // Función para marcar el camino de Tom a Jerry en la matriz de celdas
        for (const auto& p : path) {
            int x = p.first - 1;
            int y = p.second - 1;
            if (matrix[x][y] == 'o') {
                matrix[x][y] = '.';
            }
        }
    }

    bool isObstacle(const Position& pos) const { // Función para verificar si una posición es un obstáculo
        int x = pos.getX();
        int y = pos.getY();
        return matrix[x][y] == 'x';
    }

    // Destructor
    ~Grid() {
        // Liberar recursos, si es necesario
    }

    // Función amiga para acceder a miembros privados
    friend vector<pair<int, int>> findPath(const Grid& grid, const Position& start, const Position& goal);
};

#endif //PROYECTOFINALDEPROGRAMACION_ESTRUCTURAS_RED_H
