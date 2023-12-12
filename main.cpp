#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>
#include <map>

using namespace std;

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

class Obstacle { // Clase para representar un obstáculo (esquina superior izquierda y esquina inferior derecha)
public:
    Position topLeft, bottomRight; // Posición de la esquina superior izquierda y posición de la esquina inferior derecha
    Obstacle(Position tl, Position br) : topLeft(tl), bottomRight(br) {} // Inicializar obstáculo con la posición de la esquina superior izquierda y la posición de la esquina inferior derecha

    bool overlaps(const Obstacle& other) const { // Función para verificar si un obstáculo se superpone con otro obstáculo
        return !(topLeft.x > other.bottomRight.x ||
                 bottomRight.x < other.topLeft.x ||
                 topLeft.y > other.bottomRight.y ||
                 bottomRight.y < other.topLeft.y);
    }
};

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

class FileParser { // Clase para leer el archivo de entrada (TOM1.DAT) y validar los datos de entrada (M, N, Tom, Jerry, obstáculos)
public:
    static string parseInputFile(const string& filename, Grid& grid, Position& tom, Position& jerry, vector<Obstacle>& obstacles) {
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

    static void writeOutputFile(const string& filename, const string& content) { // Función para escribir el archivo de salida (TOM1.RES)
        ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            cerr << "ERROR: Cannot open output file." << endl;
            return;
        }
        outputFile << content;
        outputFile.close();
    }
};

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

void printGrid(const Grid& grid) {
    const char horizontalWall = '-';
    const char verticalWall = '|';
    const char corner = '+';

    // Dibujar la parte superior de la cuadrícula
    cout << corner;
    for (int j = 0; j < grid.getN(); ++j) {
        cout << horizontalWall << horizontalWall << horizontalWall << corner;
    }
    cout << endl;

    // Dibujar las filas de la cuadrícula
    for (int i = 0; i < grid.getM(); ++i) {
        // Línea con las celdas y paredes verticales
        cout << verticalWall;
        for (int j = 0; j < grid.getN(); ++j) {
            cout << ' ' << grid.getCell(i, j) << ' ' << verticalWall;
        }
        cout << endl;

        // Línea con las paredes horizontales y esquinas
        cout << corner;
        for (int j = 0; j < grid.getN(); ++j) {
            cout << horizontalWall << horizontalWall << horizontalWall << corner;
        }
        cout << endl;
    }
}

int main() {
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
        printFileContent(outputFilename2);
    }

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


// Después de procesar TOM1.DAT, TOM2.DAT y TOM3.DAT
    printGrid(grid);

    return 0;

    return 0;
}
