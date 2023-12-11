//Proyecto Tom y Jerry (Usando estructuras de datos y programacion orientada a objetos)
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

//Prototipos
class Position;
class Obstacle;
class Grid;
class FileParser;

//prototipos de funciones
vector<pair<int, int>> findPath(const Grid& grid, const Position& start, const Position& goal);
void writePathToFile(const string& filename, const vector<pair<int, int>>& path);
void printFileContent(const string& filename);

//prototipos de structs
struct Node {
    int x, y, dist;
    Node* parent;

    Node(int x, int y, int dist, Node* parent) : x(x), y(y), dist(dist), parent(parent) {}
};

class Position {
public:
    int x, y;
    Position(int x, int y) : x(x-1), y(y-1) {}  // Ajuste para índices basados en cero
};

class Obstacle {
public:
    Position topLeft, bottomRight;
    Obstacle(Position tl, Position br) : topLeft(tl), bottomRight(br) {}

    // Verificar si este obstáculo se superpone con otro
    bool overlaps(const Obstacle& other) const {
        return !(topLeft.x > other.bottomRight.x ||
        bottomRight.x < other.topLeft.x ||
        topLeft.y > other.bottomRight.y ||
        bottomRight.y < other.topLeft.y);
    }
};

class Grid {
private:

public:
    int M, N;
    vector<vector<char>> matrix;
    Grid(int M, int N) : M(M), N(N), matrix(M, vector<char>(N, 'o')) {}

    void placeTom(const Position& tom) {
        matrix[tom.x][tom.y] = 'T';
    }

    void placeJerry(const Position& jerry) {
        matrix[jerry.x][jerry.y] = 'J';
    }

    bool addObstacle(const Obstacle& obs) {
        if (!isPositionValid(obs.topLeft) || !isPositionValid(obs.bottomRight)) {
            return false;  // Invalid obstacle position
        }

        for (int i = obs.topLeft.x; i <= obs.bottomRight.x; ++i) {
            for (int j = obs.topLeft.y; j <= obs.bottomRight.y; ++j) {
                matrix[i][j] = 'x';
            }
        }
        return true;
    }

    bool isPositionValid(const Position& pos) const {
        return pos.x >= 0 && pos.x < M && pos.y >= 0 && pos.y < N;
    }

    string toString() const {
        string output;
        for (const auto& row : matrix) {
            for (char cell : row) {
                output += cell;
            }
            output += '\n';
        }
        return output;
    }

    void markPath(const vector<pair<int, int>>& path) {
        for (const auto& p : path) {
            if(matrix[p.first - 1][p.second - 1] == 'o') // Verificar que no sobreescriba a Tom o Jerry
                matrix[p.first - 1][p.second - 1] = '.';  // Marcamos el camino en la cuadrícula
        }
    }

    bool isObstacle(const Position& pos) const {
        return matrix[pos.x][pos.y] == 'x'; // 'x' representa un obstáculo
    }
};

class FileParser {
public:
    static string parseInputFile(const string& filename, Grid& grid, Position& tom, Position& jerry, vector<Obstacle>& obstacles) {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            return "ERROR: Cannot open input file.";
        }

        int M, N;
        inputFile >> M >> N;

        if (M <= 0 || N <= 0) {
            return "ERROR E0";
        }

        grid = Grid(M, N);

        int tomX, tomY, jerryX, jerryY;
        inputFile >> tomX >> tomY >> jerryX >> jerryY;
        tom = Position(tomX, tomY);
        jerry = Position(jerryX, jerryY);

        if (!grid.isPositionValid(tom) || !grid.isPositionValid(jerry)) {
            return "ERROR E1";
        }

        if (tom.x == jerry.x && tom.y == jerry.y) {
            return "ERROR E2";
        }

        grid.placeTom(tom);
        grid.placeJerry(jerry);

        int x1, y1, x2, y2;
        while (inputFile >> x1 >> y1 >> x2 >> y2) {
            Obstacle obs(Position(x1, y1), Position(x2, y2));
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

    static void writeOutputFile(const string& filename, const string& content) {
        ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            cerr << "ERROR: Cannot open output file." << endl;
            return;
        }
        outputFile << content;
        outputFile.close();
    }
};

int main() {
    Grid grid(0, 0);
    Position tom(0, 0);
    Position jerry(0, 0);
    vector<Obstacle> obstacles;

    string inputFilename = "TOM1.DAT";
    string outputFilename = "TOM1.RES";

    // Verificar si el archivo de entrada existe
    ifstream inputFileTest(inputFilename);
    if (!inputFileTest.good()) {
        // El archivo no existe, crear un archivo de prueba
        ofstream outputFileTest(inputFilename);
        if (!outputFileTest) {
            cerr << "No se pudo crear el archivo de entrada de prueba." << endl;
            return 1;
        }
        // Escribir datos de prueba en el archivo
        outputFileTest << "7 5\n1 2 7 1\n2 1 2 2\n2 4 2 4\n4 2 4 5\n6 1 6 2\n6 4 6 4";
        outputFileTest.close();
        cout << "Se ha creado un archivo de entrada de prueba." << endl;
    }
    inputFileTest.close();

    string error = FileParser::parseInputFile(inputFilename, grid, tom, jerry, obstacles);
    if (!error.empty()) {
        FileParser::writeOutputFile(outputFilename, error);
        cerr << error << endl;
        return 1; // Terminar si hay un error
    } else {
        //meter la cuadricula en el archivo TOM1.RES
        FileParser::writeOutputFile(outputFilename, grid.toString());
        // Copiar el contenido de TOM1.DAT a TOM2.DAT si no hay errores
        ifstream src(inputFilename, ios::binary);
        ofstream dst("TOM2.DAT", ios::binary);
        dst << src.rdbuf();
        src.close();
        dst.close();

        // Procesar TOM2.DAT para calcular la ruta más corta
        inputFilename = "TOM2.DAT";
        auto outputFilename2 = "TOM2.RES";

        // Se asume que parseInputFile ya ha establecido los valores de grid, tom y jerry
        vector<pair<int, int>> path = findPath(grid, tom, jerry);
        writePathToFile(outputFilename2, path);
        grid.markPath(path);

        //Imprimir el contenido del archivo TOM1.RES
        printFileContent(outputFilename);

        // Imprimir el contenido del archivo TOM2.RES
        cout << endl;
        printFileContent(outputFilename2);
    }

    return 0;
}

// Función para encontrar la ruta más corta desde la posición de inicio hasta la posición de destino que consideren los obstáculos, y que TOM solo se puede mover en horizontal o vertical nunca en diagonal
vector<pair<int, int>> findPath(const Grid& grid, const Position& start, const Position& goal) {
    queue<Node*> q;
    vector<vector<bool>> visited(grid.M, vector<bool>(grid.N, false));

    q.push(new Node(start.x, start.y, 0, nullptr));
    visited[start.x][start.y] = true;

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    Node* lastNode = nullptr;

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current->x == goal.x && current->y == goal.y) {
            lastNode = current;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            if (nx >= 0 && nx < grid.M && ny >= 0 && ny < grid.N && !visited[nx][ny] && !grid.isObstacle(Position(nx+1, ny+1))) {
                visited[nx][ny] = true;
                q.push(new Node(nx, ny, current->dist + 1, current));
            }
        }
    }

    vector<pair<int, int>> path;
    while (lastNode) {
        path.emplace_back(lastNode->x + 1, lastNode->y + 1);  // +1 para ajustar índices
        lastNode = lastNode->parent;
    }

    reverse(path.begin(), path.end());
    return path;
}

// Función para escribir la ruta en el archivo "TOM2.RES"
void writePathToFile(const string& filename, const vector<pair<int, int>>& path) {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        cerr << "ERROR: Cannot open output file." << endl;
        return;
    }
    if (path.empty()) {
        outputFile << "INALCANZABLE";
    } else {
        for (const auto& p : path) {
            outputFile << p.first << " " << p.second << "\n";
        }
    }
    outputFile.close();
}

void printFileContent(const string& filename) {
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
