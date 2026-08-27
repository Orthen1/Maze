//
// Created by teo on 24. 8. 2026.
//

#ifndef MAZE_GRID_H
#define MAZE_GRID_H
#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include <random>

#include "Camera.h"

enum Directions {
    NONE = 0,
    N = 1,
    S = 2,
    W = 3,
    E = 4,

};

enum BIT_DIR {
    N_BIT = 1,
    S_BIT = 2,
    W_BIT = 4,
    E_BIT = 8,
    ERROR = -1
};


struct Wall {

    glm::vec3 position;
    bool vertical;
    BoundingBox box;
    float thickness = 0.1f;
    bool visible = true;


};

struct Tile {
    glm::vec3 position;
    std::shared_ptr<Wall> wall;
        };


struct Cell {

    glm::vec2 position;
    bool not_carved = true;
    bool frontier = false;
    int direction = NONE;
    std::shared_ptr<Tile> tiles[5];
};




class Grid {

public:
    Grid(unsigned int rows ,unsigned int cols);
    Tile* getTile(unsigned int posX, unsigned int posY) const;
    std::vector<std::unique_ptr<Cell>> &getCells(){return  cells;};

    std::vector<std::shared_ptr<Wall>> &getWalls(){return walls;};
    Cell* getCell(int posX, int posY)const;
    Cell* getCell(glm::vec2 position)const;
    int getCols()const {return cols;};
    int getRows()const{return rows;};
    glm::vec3 generateWalls();
    void printMaze() const;
    BIT_DIR opositions(BIT_DIR dir) {
        switch (dir) {
            case W_BIT:
                return E_BIT;
            case E_BIT:
                return W_BIT;
            case N_BIT:
                return S_BIT;
            case S_BIT:
                return N_BIT;
        }
        return ERROR;
    };


private:
    glm::vec3 generateMaze();
    void mark(int posX, int posY,std::vector<Cell*> &frontier);
    void addFrontier(int posX,int posY ,std::vector<Cell*> &frontier);
    static BIT_DIR directions(int fx,int fy,int tx,int ty);
    std::vector<Cell*>  neighbours(int posX,int posY) const;
    std::shared_ptr<Wall>  addWall( const glm::vec3& pos, bool vertical);
    unsigned int  rows;
    unsigned int cols;
    unsigned int cellRows;
    unsigned int cellCols;
    std::vector<std::unique_ptr<Cell>> cells;
    std::vector<std::shared_ptr<Wall>> walls;
    void createExit();
    void removeWall(int posX, int posY) const;

};



#endif //MAZE_GRID_H
