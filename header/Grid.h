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
    W = 4,
    E = 8,

};


struct Wall {

    glm::vec3 position;
    bool vertical;
    BoundingBox box;
    float thickness = 0.1;
    bool visible = true;


};

struct Tile {
    int posX;
    int posY;
    bool not_carved = true;
    bool frontier = false;
    int direction = NONE;
    Wall *wall = nullptr ;

};




class Grid {

public:
    Grid(unsigned int rows ,unsigned int cols);
    Tile* getTile(unsigned int posX, unsigned int posY) const;
    const int getRows() const{return rows;};
    const int getCols() const {return cols;};
    std::vector<std::unique_ptr<Tile>>& getTiles()  { return  tiles;};
    glm::vec3 generateWalls(std::vector<Wall> &walls);
    void printMaze() const;
    Directions opositions(Directions dir) {
        switch (dir) {
            case W:
                return E;
            case E:
                return W;
            case N:
                return S;
            case S:
                return N;
        }
        return  NONE;
    }


private:
    glm::vec3 generateMaze();
    void mark(int posX, int posY,std::vector<Tile*> &frontier);
    void addFrontier(int posX,int posY ,std::vector<Tile*> &frontier);
    static Directions directions(int fx,int fy,int tx,int ty);
    std::vector<Tile*>  neighbours(int posX,int posY) const;
    void addWall(std::vector<Wall>& walls, const glm::vec3& pos, bool vertical);
    unsigned int  rows;
    unsigned int cols;
    unsigned int cellRows;
    unsigned int cellCols;
    std::vector<std::unique_ptr<Tile>> tiles;
    void getExit(std::vector<Wall>& walls);
    void removeWall(std::vector<Wall>& walls,int posX, int posY) const;
    void generateGrid(std::vector<Wall>& walls);

};



#endif //MAZE_GRID_H
