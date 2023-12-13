#ifndef PROYECTOFINALDEPROGRAMACION_ESTRUCTURAS_ARCHIVOS_H
#define PROYECTOFINALDEPROGRAMACION_ESTRUCTURAS_ARCHIVOS_H

#include <fstream>
#include <vector>
#include "red.h"
#include "Obstaculo.h"

using namespace std;

class FileParser { // Clase para leer el archivo de entrada (TOM1.DAT) y validar los datos de entrada (M, N, Tom, Jerry, obstáculos)
public:
    static string parseInputFile(const string& filename, Grid& grid, Position& tom, Position& jerry, vector<Obstacle>& obstacles);

    static void writeOutputFile(const string& filename, const string& content);
};

#endif
