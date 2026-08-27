//
// Created by teo on 24. 8. 2026.
//

#include "../header/Grid.h"

#include <algorithm>
#include <iostream>

Grid::Grid(unsigned int rows, unsigned int cols) {
    if (rows <= 0 || cols <= 0) {
        std::cerr << "Grid is 0 or smaller and that is not supported" << std::endl;
        exit(-10);
    }
    this->rows = rows;
    this->cols = cols;
}

void Grid::Init() {
    this->rows = 2 * rows + 1;
    this->cols = 2 * cols + 1;
    for (int i = 0; i < this->cols; i++) {
        for (int j = 0; j < this->rows; j++) {
            cells.push_back(std::make_unique<Cell>(glm::vec2(i, j), glm::vec3(i, 0.0f, j)));
        }
    }
}

glm::vec3 Grid::generateMaze() {
    std::mt19937 r_device(std::random_device{}());
    std::uniform_int_distribution<uint> maxY(0, (rows / 2 - 1));
    std::uniform_int_distribution<uint> maxX(0, (cols / 2 - 1));
    int posX = 2 * maxX(r_device) + 1;
    int posY = 2 * maxY(r_device) + 1;
    glm::vec2 position(posX, posY);
    std::clog << "POSX: " << posX << " POSY: " << posY << std::endl;
    glm::vec3 CameraPos(position.x, 0, position.y);
    std::vector<Cell*> frontier, adjacent;
    mark(position.x, position.y, frontier);

    while (!frontier.empty()) {
        std::uniform_int_distribution<int> length(0, frontier.size() - 1);
        int index = length(r_device);
        Cell* current = frontier.at(index);
        position = current->position;

        frontier[index] = frontier.back();
        frontier.pop_back();

        adjacent = neighbours(position.x, position.y);
        std::uniform_int_distribution<int> neigbour(0, adjacent.size() - 1);
        index = neigbour(r_device);
        Cell* n = adjacent.at(index);
        int nx = n->position.x;
        int ny = n->position.y;
        carve(position.x, position.y, nx, ny);
        mark(position.x, position.y, frontier);
    }
    return CameraPos;
}

void Grid::mark(int posX, int posY, std::vector<Cell*>& frontier) {
    getCell(posX, posY)->visited = true;
    addFrontier(posX - 2, posY, frontier);
    addFrontier(posX + 2, posY, frontier);
    addFrontier(posX, posY - 2, frontier);
    addFrontier(posX, posY + 2, frontier);
}

void Grid::addFrontier(int posX, int posY, std::vector<Cell*>& frontier) {
    auto tmp = getCell(posX, posY);
    if (tmp != nullptr && (tmp->frontier == false) && (tmp->visited == false)) {
        tmp->frontier = true;
        frontier.push_back(tmp);
    }
}

std::vector<Cell*> Grid::neighbours(int posX, int posY) const {
    std::vector<Cell*> neighbours;
    if (posX >= 2 && getCell(posX - 2, posY)->visited == true) {
        neighbours.push_back(getCell(posX - 2, posY));
    }
    if (posX + 2 < cols && getCell(posX + 2, posY)->visited == true) {
        neighbours.push_back(getCell(posX + 2, posY));
    }
    if (posY >= 2 && getCell(posX, posY - 2)->visited == true) {
        neighbours.push_back(getCell(posX, posY - 2));
    }
    if (posY + 2 < rows && getCell(posX, posY + 2)->visited == true) {
        neighbours.push_back(getCell(posX, posY + 2));
    }

    return neighbours;
}

void Grid::carve(int x, int y, int nx, int ny) {
    int midX = (x + nx) / 2;
    int midY = (y + ny) / 2;
    getCell(x, y)->isWall = false;
    getCell(nx, ny)->isWall = false;
    getCell(midX, midY)->isWall = false;
}

glm::vec3 Grid::generateWalls() {
    glm::vec3 startPos = generateMaze();

    for (auto& cell : cells) {
        if (cell->isWall) {
            addWall(cell->worldPos);
        }
    }
    createExit();
    return startPos;
}

void Grid::printMaze() const {
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            Cell* cell = getCell(x, y);
            std::cout << (cell->isWall ? "#" : " ");
        }
        std::cout << "\n";
    }
}

std::shared_ptr<Wall> Grid::addWall(const glm::vec3& pos) {
    for (const auto& wall : walls) {
        if (wall->position == pos) {
            return nullptr;
        }
    }
    std::shared_ptr<Wall> wall = std::make_shared<Wall>(
        pos, BoundingBox(pos.x - 0.5f, -0.5f, pos.z - 0.5f, pos.x + 0.5f, 0.5f, pos.z + 0.5f)
    );
    walls.push_back(wall);
    getCell(pos.x, pos.y)->wall = wall;
    return wall;
}

void Grid::createExit() {
    std::mt19937 r_device(std::random_device{}());
    std::uniform_int_distribution<int> maxX(0, cols - 1);
    std::uniform_int_distribution<int> maxY(0, rows - 1);
    std::uniform_int_distribution<int> toss(0, 1);

    int posX = maxX(r_device);
    int posY;
    if (posX == 0 || posX == rows - 1) {
        posY = maxY(r_device);
    } else {
        posY = toss(r_device) == 0 ? 0 : cols - 1;
    }

    removeWall(posX, posY);
}

void Grid::removeWall(int posX, int posY) const {
    glm::vec3 target = glm::vec3(posX, 0, posY);
    auto wallToRemove =
        std::find_if(walls.begin(), walls.end(), [&](const std::shared_ptr<Wall>& wall) {
            return wall->position == target;
        });

    if (wallToRemove != walls.end()) {
        (*wallToRemove)->visible = false;
    }
}

//**Getters and Setters**//

Cell* Grid::getCell(glm::vec2 position) const {
    for (auto& cell : cells) {
        if (position.x == cell->position.x && position.y == cell->position.y) {
            return cell.get();
        }
    }
    return nullptr;
}

Cell* Grid::getCell(int posX, int posY) const {
    for (auto& cell : cells) {
        if (posX == cell->position.x && posY == cell->position.y) {
            return cell.get();
        }
    }
    return nullptr;
}
