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

/// A single wall in the grid
struct Wall {
    glm::vec3 position;
    BoundingBox box;
    bool visible = true;
};
/// A single cell in the maze grid, used during generation and lookup.
struct Cell {
    glm::vec2 position;
    glm::vec3 worldPos;
    bool visited = false;
    bool frontier = false;
    bool isWall = true;
    std::shared_ptr<Wall> wall;
};
/**
 * @brief Procedurally generates and stores a maze as a grid of cells and walls.
 *
 * Uses Prim Algorthm to carve passages through an
 * initial all-wall grid, producing a set of Cell and Wall objects that can
 * be queried by position and rendered.
 *
 */

class Grid {
   public:
    Grid(unsigned int rows, unsigned int cols);
    void Init();
    /// Calls generateMaze and fills the walls variable
    glm::vec3 generateWalls();

    /// Support function for drawing in terminal window
    /// current layout of maze
    void printMaze() const;

    /// Getters and Setters
    void setRows(int rowsIn) { this->rows = rowsIn; };
    void setCols(int colsIn) { this->cols = colsIn; };
    std::vector<std::unique_ptr<Cell>>& getCells() { return cells; };
    std::vector<std::shared_ptr<Wall>>& getWalls() { return walls; };
    Cell* getCell(int posX, int posY) const;
    Cell* getCell(glm::vec2 position) const;
    int getCols() const { return cols; };
    int getRows() const { return rows; };
    glm::vec3 getExit();

   private:
    /// Variables
    unsigned int rows;  ///< Maze dimensions in logical cells (passages only)
    unsigned int cols;
    unsigned int cellRows;  ///< Expanded grid dimensions used for rendering/storage:
                            ///< 2*rows - 1, giving each wall between cells its own slot
    unsigned int cellCols;  ///< 2*cols - 1, same expansion as cellRows

    std::vector<std::unique_ptr<Cell>> cells;  /// Hold all the cells in the maze
    std::vector<std::shared_ptr<Wall>> walls;  /// Holds all the walls in the maze for faster lookup

    /// Runs the Prims Algorithm
    /// and returns the starting positon of player
    glm::vec3 generateMaze();

    /// Prims Algorithm support functions
    void mark(int posX, int posY, std::vector<Cell*>& frontier);

    void addFrontier(int posX, int posY, std::vector<Cell*>& frontier);

    std::vector<Cell*> neighbours(int posX, int posY) const;

    std::shared_ptr<Wall> addWall(const glm::vec3& pos);
    /// Removes the wall between adjacent cells (x, y) and (nx, ny), joining them.
    void carve(int x, int y, int nx, int ny);

    /// Carves an opening in the boundary wall to create the maze exit.
    void createExit();
    void removeWall(int posX, int posY) const;
};

#endif  // MAZE_GRID_H
