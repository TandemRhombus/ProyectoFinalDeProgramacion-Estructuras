#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>
#include <map>
#include "Archivos.h"
#include "red.h"
#include "GridViewer.h"
#include "Posicion.h"

using namespace std;

struct Node { // Estructura para representar un nodo (posición) en el BFS
    int x, y, dist;
    Node* parent;

    Node(int x, int y, int dist, Node* parent) : x(x), y(y), dist(dist), parent(parent) {}
};

vector<pair<int, int>> findPath(const Grid& grid, const Position& start, const Position& goal) { // Función para encontrar el camino de Tom a Jerry (BFS)
    queue<Node*> q; // Cola para BFS
    vector<vector<bool>> visited(grid.getM(), vector<bool>(grid.getN(), false)); // Matriz de visitados para BFS

    q.push(new Node(start.getX(), start.getY(), 0, nullptr)); // Inicializar BFS con la posición inicial de Tom
    visited[start.getX()][start.getY()] = true; // Marcar posición inicial como visitada

    int dx[] = {1, -1, 0, 0}; // Arreglos para recorrer las 4 direcciones
    int dy[] = {0, 0, 1, -1}; // (derecha, izquierda, arriba, abajo)

    Node* lastNode = nullptr; // Último nodo visitado (posición de Jerry)

    while (!q.empty()) { // BFS
        Node* current = q.front(); // Obtener nodo actual de la cola y eliminarlo de la misma (pop) para no volver a visitarlo más adelante
        q.pop();

        if (current->x == goal.getX() && current->y == goal.getY()) { // Si la posición actual es la de Jerry, terminar BFS y guardar el nodo actual
            lastNode = current;
            break;
        }

        for (int i = 0; i < 4; i++) { // Recorrer las 4 direcciones (derecha, izquierda, arriba, abajo)
            int nx = current->x + dx[i]; // Obtener coordenadas de la posición adyacente en la dirección actual
            int ny = current->y + dy[i]; // (nx, ny)

            if (nx >= 0 && nx < grid.getM() && ny >= 0 && ny < grid.getN() && !visited[nx][ny] && !grid.isObstacle(Position(nx + 1, ny + 1))) { // Si la posición adyacente es válida y no ha sido visitada, agregarla a la cola y marcarla como visitada (si no es un obstáculo)
                visited[nx][ny] = true; // Marcar posición adyacente como visitada (para no volver a visitarla más adelante)
                q.push(new Node(nx, ny, current->dist + 1, current)); // Agregar posición adyacente a la cola (para visitarla más adelante)
            }
        }
    }

    vector<pair<int, int>> path; // Vector para guardar el camino de Tom a Jerry (posiciones visitadas) (se guarda en orden inverso)
    while (lastNode) { // Recorrer el camino de Jerry a Tom (desde la posición de Jerry hasta la posición de Tom) y guardarlo en el vector
        path.emplace_back(lastNode->x + 1, lastNode->y + 1); // +1 para ajustar índices
        lastNode = lastNode->parent; // Obtener nodo padre del nodo actual (posición adyacente anterior) y repetir el proceso hasta llegar a la posición inicial de Tom
    }

    reverse(path.begin(), path.end()); // Invertir el vector para obtener el camino de Tom a Jerry (en orden correcto) y devolverlo
    return path; // Devolver camino de Tom a Jerry
}

void writePathToFile(const string& filename, const vector<pair<int, int>>& path) { // Función para escribir el camino de Tom a Jerry en el archivo de salida (TOM2.RES) (en orden inverso)
    ofstream outputFile(filename);
    if (!outputFile.is_open()) { // Validación de archivo abierto
        cerr << "ERROR: Cannot open output file." << endl;
        return;
    }
    if (path.empty()) { // Si no hay camino, escribir "INALCANZABLE"
        outputFile << "INALCANZABLE";
    } else { // Si hay camino, escribir cada posición del camino en una línea diferente
        for (const auto& p : path) { // Recorrer el camino de Tom a Jerry (en orden inverso) y escribir cada posición en una línea diferente
            outputFile << p.first << " " << p.second << "\n"; // Escribir posición en una línea diferente (x y)
        }
    }
    outputFile.close();
}

void printFileContent(const string& filename) { // Función para imprimir el contenido de un archivo en la consola
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: No se pudo abrir el archivo para lectura." << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

void findAllPaths(const Grid& grid, const Position& current, const Position& goal, vector<vector<bool>>& visited, int length, map<int, int>& pathCount) {
    if (current.getX() == goal.getX() && current.getY() == goal.getY()) {
        pathCount[length]++;
        return;
    }

    static const int dx[] = {1, -1, 0, 0};
    static const int dy[] = {0, 0, 1, -1};

    visited[current.getX()][current.getY()] = true;

    for (int i = 0; i < 4; ++i) {
        int nx = current.getX() + dx[i];
        int ny = current.getY() + dy[i];
        Position nextPos(nx + 1, ny + 1);

        if (grid.isPositionValid(nextPos) && !grid.isObstacle(nextPos) && !visited[nx][ny]) {
            findAllPaths(grid, nextPos, goal, visited, length + 1, pathCount);
        }
    }

    visited[current.getX()][current.getY()] = false;
}

// Esta función escribe el contenido de ayuda en el archivo "ayuda.txt"
void writeHelpToFile() {
    ofstream helpFile("ayuda.txt");

    if (!helpFile.is_open()) {
        cerr << "ERROR: No se pudo abrir el archivo de ayuda." << endl;
        return;
    }

    helpFile << "=== Ayuda del Programa ===" << endl;
    helpFile << "Bienvenido al programa de resolución del laberinto de Tom y Jerry." << endl;
    helpFile << "Este programa genera por default a TOM1.DAT si gustas modificar el archivo puedes hacerlo desde el archivo TOM1.DAT" << endl;
    helpFile << "Apartir de TOM1.DAT gira el funcionamiento del programa." << endl;
    helpFile << "" << endl;
    helpFile << "=== Formato del archivo TOM1.DAT ===" << endl;
    helpFile << "La primera linea del archivo contiene dos enteros separados por un espacio, M y N, que representan el tamaño de la matriz de celdas." << endl;
    helpFile << "La segunda linea del archivo contiene cuatro enteros separados por un espacio, x1, y1, x2, y2, que representan la posición de Tom y Jerry." << endl;
    helpFile << "Las siguientes lineas y dependiendo del tamaño de la matriz se colocaran las coordenadas para los obstaculos." << endl;
    helpFile << "" << endl;
    helpFile << "Dependiendo de lo anterior se va a generar un tablero el cual sera almacenado en TOM1.RES el cual tambien puede marcarte errores que van del E0 al E6" << endl;
    helpFile << "Algunos ejemplos de errores son:" << endl;
    helpFile << "E0: M y N no son enteros positivos." << endl;
    helpFile << "E1: Tom y Jerry se encuentran en la misma posición." << endl;
    helpFile << "" << endl;
    helpFile << "Si TOM1.RES no tiene errores se generara un TOM2.RES el cual contendra el camino mas corto de Tom a Jerry." << endl;
    helpFile << "" << endl;
    helpFile << "=== Formato del archivo TOM2.RES ===" << endl;
    helpFile << "El archivo contiene una serie de lineas, cada una con dos enteros separados por un espacio, x y, que representan una posición en el camino de Tom a Jerry." << endl;
    helpFile << "Si no hay camino, el archivo contiene la palabra INALCANZABLE." << endl;
    helpFile << "" << endl;
    helpFile << "De igual manera si TOM1.RES no tiene errores se generara un TOM3.RES el cual contendra la longitud de los caminos de Tom a Jerry y el numero de caminos de esa longitud." << endl;
    helpFile << "" << endl;
    helpFile << "=== Formato del archivo TOM3.RES ===" << endl;
    helpFile << "El archivo contiene una serie de lineas, cada una con dos enteros separados por un espacio, longitud y caminos, que representan la longitud de los caminos de Tom a Jerry y el numero de caminos de esa longitud." << endl;
    helpFile << "" << endl;
    helpFile << "Los algoritmos utilizados para resolver el problema son:" << endl;
    helpFile << "BFS: Para encontrar el camino mas corto de Tom a Jerry." << endl;
    helpFile << "DFS: Para encontrar todos los caminos de Tom a Jerry." << endl;
    helpFile << "" << endl;
    helpFile << "Las estructuras utilizadas para resolver el problema son:" << endl;
    helpFile << "Grid: Para representar la matriz de celdas." << endl;
    helpFile << "Position: Para representar una posición (x, y)." << endl;
    helpFile << "Obstacle: Para representar un obstáculo." << endl;
    helpFile << "Node: Para representar un nodo (posición) en el BFS." << endl;
    helpFile << "" << endl;
    helpFile << "Las estructuras de datos utilizadas para resolver el problema son:" << endl;
    helpFile << "vector: Para representar la matriz de celdas." << endl;
    helpFile << "queue: Para representar la cola del BFS." << endl;
    helpFile << "map: Para representar la tabla de frecuencias de los caminos de Tom a Jerry." << endl;
    helpFile << "" << endl;
    helpFile << "Esperamos que disfrutes el programa." << endl;

    helpFile.close();
}

int main() {
    writeHelpToFile();
    Grid grid(0, 0); // Matriz de celdas (M x N)
    Position tom(0, 0); // Posición de Tom (x, y)
    Position jerry(0, 0); // Posición de Jerry (x, y) (x != y)
    vector<Obstacle> obstacles; // Vector de obstáculos

    string inputFilename = "TOM1.DAT";
    string outputFilename = "TOM1.RES";

    ifstream inputFileTest(inputFilename);
    if (!inputFileTest.good()) {
        ofstream outputFileTest(inputFilename);
        if (!outputFileTest) {
            cerr << "No se pudo crear el archivo de entrada de prueba." << endl;
            return 1;
        }
        outputFileTest << "4 4\n1 1 4 4\n3 2 3 3\n2 3 2 3";
        outputFileTest.close();
        cout << "Se ha creado un archivo de entrada de prueba." << endl;
    }
    inputFileTest.close();

    string error = FileParser::parseInputFile(inputFilename, grid, tom, jerry, obstacles); // Leer archivo de entrada (TOM1.DAT) y validar datos de entrada (M, N, Tom, Jerry, obstáculos)
    cout << "Fase 1" << endl;
    if (!error.empty()) { // Validación de error en la lectura del archivo de entrada
        FileParser::writeOutputFile(outputFilename, error); // Escribir error en el archivo de salida (TOM1.RES) y terminar el programa
        cerr << error << endl;
        return 1;
    } else {
        FileParser::writeOutputFile(outputFilename, grid.toString()); // Escribir matriz de celdas en el archivo de salida (TOM1.RES) y continuar con el programa
        ifstream src(inputFilename, ios::binary); // Copiar archivo de entrada (TOM1.DAT) a TOM2.DAT
        ofstream dst("TOM2.DAT", ios::binary);
        dst << src.rdbuf();
        src.close();
        dst.close();

        inputFilename = "TOM2.DAT";
        auto outputFilename2 = "TOM2.RES";

        vector<pair<int, int>> path = findPath(grid, tom, jerry);
        writePathToFile(outputFilename2, path);
        grid.markPath(path);

        printFileContent(outputFilename);

        cout << endl;
        cout << "Fase 2" << endl;
        cout << "Camino de Tom a Jerry mas corto:" << endl;
        // Crear un puntero a GridPrinter y asignarle una instancia de BasicGridPrinter
        GridPrinter* printer = new BasicGridPrinter();

        // Usar el printer para imprimir el grid
        printer->print(grid);

        // Liberar la memoria
        delete printer;
        cout << endl;
        printFileContent(outputFilename2);
        cout << endl;
    }
    cout << "Fase 3" << endl;
    // Copiando TOM1.DAT a TOM3.DAT
    ifstream src("TOM1.DAT", ios::binary);
    ofstream dst("TOM3.DAT", ios::binary);
    dst << src.rdbuf();
    src.close();
    dst.close();

    // Procesamiento para TOM3.DAT
    map<int, int> pathCount;
    vector<vector<bool>> visited(grid.getM(), vector<bool>(grid.getN(), false));
    findAllPaths(grid, tom, jerry, visited, 0, pathCount);

    // Escribiendo los resultados a TOM3.RES
    ofstream outputFile("TOM3.RES");
    if (!outputFile.is_open()) {
        cerr << "ERROR: Cannot open output file for TOM3." << endl;
        return 1;
    }

    for (const auto& p : pathCount) {
        outputFile << p.first << " " << p.second << "\n";
    }
    outputFile.close();

    // Mostrando los resultados por pantalla
    cout << endl;
    for (const auto& p : pathCount) {
        cout << "Longitud: " << p.first << ", Caminos: " << p.second << endl;
    }

    return 0;
}
