//
// Created by teo on 24. 8. 2026.
//

#ifndef MAZE_GRID_H
#define MAZE_GRID_H
#include <glm/vec3.hpp>
#include <memory>
#include <random>
#include <vector>

#include "Camera.h"

struct Wall {
    glm::vec3 position;
    BoundingBox box;
    bool visible = true;
};

struct Cell {
    glm::vec2 position;
    glm::vec3 worldPos;
    bool visited = false;
    bool frontier = false;
    bool isWall = true;
    std::shared_ptr<Wall> wall;
};

class Grid {
   public:
    Grid(unsigned int rows, unsigned int cols);
    std::vector<std::unique_ptr<Cell>>& getCells() { return cells; };
    void Init();
    std::vector<std::shared_ptr<Wall>>& getWalls() { return walls; };
    Cell* getCell(int posX, int posY) const;
    Cell* getCell(glm::vec2 position) const;
    int getCols() const { return cols; };
    int getRows() const { return rows; };
    glm::vec3 getExit();
    glm::vec3 generateWalls();
    void printMaze() const;
    void setRows(int rowsIn) { this->rows = rowsIn; };
    void setCols(int colsIn) { this->cols = colsIn; };

   private:
    glm::vec3 generateMaze();
    void mark(int posX, int posY, std::vector<Cell*>& frontier);
    void addFrontier(int posX, int posY, std::vector<Cell*>& frontier);
    std::vector<Cell*> neighbours(int posX, int posY) const;
    std::shared_ptr<Wall> addWall(const glm::vec3& pos);
    unsigned int rows;
    unsigned int cols;
    unsigned int cellRows;
    unsigned int cellCols;
    std::vector<std::unique_ptr<Cell>> cells;
    std::vector<std::shared_ptr<Wall>> walls;
    void createExit();
    void carve(int x, int y, int nx, int ny);
    void removeWall(int posX, int posY) const;
};

#endif  // MAZE_GRID_H
