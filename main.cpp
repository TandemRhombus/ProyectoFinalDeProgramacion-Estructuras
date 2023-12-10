//Proyecto Tom y Jerry (Usando estructuras de datos y programacion orientada a objetos)
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <utility>

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

    // Verificar si el archivo de entrada existe
    string inputFilename = "TOM1.DAT";
    ifstream inputFileTest(inputFilename);
    if (!inputFileTest.good()) {
        // El archivo no existe, crear un archivo de prueba
        ofstream outputFileTest(inputFilename);
        if (!outputFileTest) {
            cerr << "No se pudo crear el archivo de entrada de prueba." << endl;
            return 1;
        }
        // Escribir datos de prueba en el archivo
        outputFileTest << "5 5\n1 1 5 3\n2 1 3 3\n1 4 4 4";
        outputFileTest.close();
        cout << "Se ha creado un archivo de entrada de prueba." << endl;
    }
    inputFileTest.close();

    string error = FileParser::parseInputFile("TOM1.DAT", grid, tom, jerry, obstacles);
    if (!error.empty()) {
        FileParser::writeOutputFile("TOM1.RES", error);
        cerr << error << endl;
    } else {
        FileParser::writeOutputFile("TOM1.RES", grid.toString());
        cout << grid.toString() << endl;
    }
    cout << endl;
    vector<pair<int, int>> path = findPath(grid, tom, jerry);

    // Imprimir y escribir la ruta en el archivo "TOM2.RES"
    writePathToFile("TOM2.RES", path);
    for (const auto& p : path) {
        cout << p.first << " " << p.second << "\n";
        grid.matrix[p.first - 1][p.second - 1] = '.';  // Marcamos el camino en la cuadrícula
    }

    // Llamar a la función para imprimir el contenido del archivo "TOM2.RES"
    printFileContent("TOM2.RES");
    return 0;
}

vector<pair<int, int>> findPath(const Grid& grid, const Position& start, const Position& goal) {
    vector<vector<bool>> visited(grid.M, vector<bool>(grid.N, false));
    queue<pair<Position, vector<pair<int, int>>>> q;

    // Direcciones de movimiento (arriba, abajo, izquierda, derecha)
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    // Inicializa la cola con la posición inicial de Tom
    q.push({start, {{start.x + 1, start.y + 1}}});  // +1 para ajustar a la representación 1-indexada

    while (!q.empty()) {
        auto [pos, path] = q.front();
        q.pop();

        // Si llegamos a Jerry, devolvemos el camino
        if (pos.x == goal.x && pos.y == goal.y) {
            return path;
        }

        for (const auto& dir : directions) {
            Position next(pos.x + dir.first, pos.y + dir.second);
            if (grid.isPositionValid(next) && !visited[next.x][next.y] && grid.matrix[next.x][next.y] != 'x') {
                visited[next.x][next.y] = true;
                vector<pair<int, int>> new_path = path;
                new_path.push_back({next.x + 1, next.y + 1});  // +1 para ajustar a la representación 1-indexada
                q.push({next, new_path});
            }
        }
    }

    // Si no hay camino, devolver una lista vacía
    return {};
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
