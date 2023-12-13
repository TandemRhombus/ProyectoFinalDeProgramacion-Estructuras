#include "Archivos.h"

string FileParser::parseInputFile(const string &filename, Grid &grid, Position &tom, Position &jerry, vector<Obstacle> &obstacles) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        return "ERROR: Cannot open input file.";
    }

    int M, N;
    inputFile >> M >> N;

    if (M <= 0 || N <= 0) { // Validación de M y N positivos
        return "ERROR E0";
    }

    grid = Grid(M, N);

    int tomX, tomY, jerryX, jerryY;
    inputFile >> tomX >> tomY >> jerryX >> jerryY;
    tom = Position(tomX, tomY);
    jerry = Position(jerryX, jerryY);

    if (!grid.isPositionValid(tom) || !grid.isPositionValid(jerry)) { // Validación de posiciones válidas para Tom y Jerry
        return "ERROR E1";
    }

    if (tom.getX() == jerry.getX() && tom.getY() == jerry.getY()) { // Validación de posiciones distintas para Tom y Jerry (no pueden estar en la misma posición)
        return "ERROR E2";
    }

    grid.placeTom(tom);
    grid.placeJerry(jerry);

    int x1, y1, x2, y2;
    while (inputFile >> x1 >> y1 >> x2 >> y2) {
        Obstacle obs(Position(x1, y1), Position(x2, y2)); // Validación de posiciones válidas para los obstáculos
        if (!grid.addObstacle(obs)) {
            return "ERROR E3";
        }
        obstacles.push_back(obs);
    }

    // Resto del código de validación

    return ""; // Sin error
}

void FileParser::writeOutputFile(const string &filename, const string &content) {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "ERROR: Cannot open output file." << endl;
        return;
    }
    outputFile << content;
    outputFile.close();
}
