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
    //E4 los vertices que representan un obstaculo no cumplen una relacion valida entre si
    for (int i = 0; i < obstacles.size(); i++) {
        if (obstacles[i].topLeft.x > obstacles[i].bottomRight.x || obstacles[i].topLeft.y > obstacles[i].bottomRight.y) {
            return "ERROR E4";
        }
    }

    //E5 dos o mas obstaculos se solapan
    for (int i = 0; i < obstacles.size(); i++) {
        for (int j = i + 1; j < obstacles.size(); j++) {
            if (obstacles[i].overlaps(obstacles[j])) {
                return "ERROR E5";
            }
        }
    }

    //E6 Tom o Jerry estan en una casilla ocupada por un obstaculo
    for (int i = 0; i < obstacles.size(); i++) {
        if (tom.x >= obstacles[i].topLeft.x && tom.x <= obstacles[i].bottomRight.x && tom.y >= obstacles[i].topLeft.y && tom.y <= obstacles[i].bottomRight.y) {
            return "ERROR E6";
        }
        if (jerry.x >= obstacles[i].topLeft.x && jerry.x <= obstacles[i].bottomRight.x && jerry.y >= obstacles[i].topLeft.y && jerry.y <= obstacles[i].bottomRight.y) {
            return "ERROR E6";
        }
    }

    return "";  // Sin error
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
