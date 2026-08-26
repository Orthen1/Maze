//
// Created by teo on 24. 8. 2026.
//

#include "../header/Grid.h"

#include <algorithm>
#include <iostream>






 Grid::Grid(unsigned int rows,unsigned int cols){
     if (rows <= 0 || cols <= 0) {
         std::cerr << "Grid is 0 or smaller and that is not supported" << std::endl;
         exit(-10);
     }
     this->rows = rows ;
     this->cols = cols ;


     for (int i = 0; i < rows; i++) {
         for (int j = 0; j < cols; j++) {
             tiles.push_back(std::make_unique<Tile>(i,j));
         }
     }


}

Tile* Grid::getTile(unsigned int posX, unsigned int posY) const{

     for (auto &tile : tiles){
         if (tile->posX == posX && tile->posY == posY) {
             return tile.get();
         }
     }
     return nullptr;
}

glm::vec3 Grid::generateMaze() {

    std::mt19937 r_device(std::random_device{}());
    std::uniform_int_distribution<uint> maxY(0,rows -1);
    std::uniform_int_distribution<uint> maxX(0,cols -1);

    int posX = maxX(r_device);
    int posY = maxY(r_device);
     glm::vec3 CameraPos(posX,0,posY);
    std::vector<Tile*> frontier, adjecent;
    mark(posX,posY,frontier);

    while (!frontier.empty()) {

        std::uniform_int_distribution<int> length(0,frontier.size()-1);
        int index = length(r_device);
        Tile* current = frontier.at(index);

        posX = current->posX;
        posY = current->posY;

        frontier[index] = frontier.back();
        frontier.pop_back();

        adjecent = neighbours(posX,posY);
        std::uniform_int_distribution<int> neigbour(0, adjecent.size() -1);
        index = neigbour(r_device);
        Tile* n = adjecent.at(index);
        int nx = n->posX;
        int ny = n->posY;
        Directions dir = directions(posX, posY,nx,ny);
        getTile(posX,posY)->direction |= dir;
        getTile(nx,ny)->direction |= opositions(dir);
        mark(posX,posY,frontier);

    }
     return CameraPos;

}

void Grid::addFrontier(int posX,int posY ,std::vector<Tile*> &frontier) {
        auto tmp = getTile(posX,posY);
        if (tmp != nullptr && tmp->frontier == false && tmp->not_carved == true) {
            tmp->frontier = true;
            frontier.push_back(tmp);
        }
}


void Grid::mark(int posX, int posY,std::vector<Tile*> &frontier) {
    getTile(posX,posY)->not_carved = false;
    addFrontier(posX-1,posY,frontier);
    addFrontier(posX+1,posY,frontier);
    addFrontier(posX,posY-1,frontier);
    addFrontier(posX,posY+1,frontier);
}


std::vector<Tile*>  Grid::neighbours(int posX,int posY) const {
        std::vector<Tile*> neighbours;
        if (posX > 0 &&  getTile(posX -1, posY)->not_carved == false) {
            neighbours.push_back(getTile(posX-1,posY));
        }
        if (posX +1 < cols && getTile(posX+1,posY)->not_carved == false) {
            neighbours.push_back(getTile(posX+1,posY));
        }
        if (posY > 0 && getTile(posX,posY-1)->not_carved == false) {
            neighbours.push_back(getTile(posX,posY-1));
        }
        if (posY + 1 <  rows && getTile(posX,posY+1)->not_carved == false) {
            neighbours.push_back(getTile(posX,posY+1));
        }

    return neighbours;

}

Directions Grid::directions(int fx,int fy,int tx,int ty) {

    Directions dir;
    if (fx < tx) {
        dir =  E;
    }
    if (fx > tx) {
        dir =  W;
    }
    if (fy < ty) {
        dir = S;
    }

    if (fy > ty) {
        dir = N;
    }

    return dir;
}


glm::vec3 Grid::generateWalls( std::vector<Wall> &walls) {
    glm::vec3 startPos = generateMaze();
    float offset = 0.5f;

    for (auto &tile : tiles){


        if (tile->posX == 0) {
            glm::vec3 pos = glm::vec3(static_cast<float>(tile->posX) - offset, 0 , static_cast<float>(tile->posY));
            addWall(walls,pos,true);
        }

        if (tile->posY == 0) {
            glm::vec3 pos = glm::vec3(static_cast<float>(tile->posX), 0 , static_cast<float>(tile->posY) - offset);
            addWall(walls,pos,false);
        }


        // Right border
        if (tile->posX == this->getCols() - 1) {
            addWall(walls, glm::vec3(static_cast<float>(tile->posX) + offset , 0, static_cast<float>(tile->posY) ),true);
        }
        // Bottom border
        if (tile->posY == this->getRows() - 1) {
            addWall(walls, glm::vec3(static_cast<float>(tile->posX), 0, static_cast<float>(tile->posY) + offset),false);
        }

        if (tile->posX < this->getCols() - 1 &&!(tile->direction & E))
        {
                addWall(walls, glm::vec3(static_cast<float>(tile->posX) + offset , 0, static_cast<float>(tile->posY) ),true);
        }

        if (tile->posY < this->getRows() - 1 &&!(tile->direction & S)){
                addWall(walls, glm::vec3(static_cast<float>(tile->posX), 0, static_cast<float>(tile->posY)  + offset),false);
            }

    }
     generateGrid(walls);
     //getExit(walls);
     return startPos;

}



    void Grid::printMaze() const{
            for (int y = 0; y < rows; ++y) {

                // TOP WALLS
                for (int x = 0; x < cols; ++x) {
                    Tile* tile = getTile(x, y);

                    std::cout << "#";

                    if (tile->direction & N)
                        std::cout << " ";
                    else
                        std::cout << "#";
                }
                std::cout << "#\n";


                // LEFT/RIGHT WALLS
                for (int x = 0; x < getCols(); ++x) {
                    Tile* tile = getTile(x, y);

                    if (tile->direction & W)
                        std::cout << " ";
                    else
                        std::cout << "#";

                    std::cout << " ";
                }

                // RIGHT BORDER
                Tile* last = getTile(cols - 1, y);

                if (last->direction & E)
                    std::cout << " ";
                else
                    std::cout << "#";

                std::cout << "\n";
            }


            // BOTTOM WALL
            for (int x = 0; x < cols; ++x) {
                Tile* tile = getTile(x, rows - 1);

                std::cout << "#";

                if (tile->direction & S)
                    std::cout << " ";
                else
                    std::cout << "#";
            }

            std::cout << "#\n";
        }


void Grid::addWall(std::vector<Wall>& walls, const glm::vec3& pos, bool vertical)
{
    for (const auto& wall : walls) {
        if (wall.position == pos) {

            return;
        }
    }
     if (vertical) {

    walls.push_back({pos,vertical,BoundingBox(pos.x - 0.05f, -0.5f,pos.z-0.5f,pos.x+0.05f , 0.5f,pos.z +0.5f)});
     }else {
         walls.push_back({pos,vertical,BoundingBox(pos.x - 0.5f, -0.5f,pos.z-0.05f,pos.x+0.5f , 0.5f,pos.z +0.05f)});
     }
}



void Grid::getExit(std::vector<Wall>& walls) {
     std::mt19937 r_device(std::random_device{}());
     std::uniform_int_distribution<int> maxX(0,cols-1);
     std::uniform_int_distribution<int> maxY(0,rows -1);
     std::uniform_int_distribution<int> toss(0,1);

     int posX = maxX(r_device);
     int posY;
     if (posX == 0 || posX == rows-1) {
         posY = maxY(r_device);
     }else {
         posY = toss(r_device) == 0 ? 0: cols -1;
     }

     removeWall(walls,posX,posY);

 }


void Grid::removeWall(std::vector<Wall>& walls,int posX, int posY) const  {
    float offset= 0.5f;
     glm::vec3 target;
     if (posX == 0) {
         target = glm::vec3(posX - offset, 0, posY);
     } else if (posX == rows - 1) {
         target = glm::vec3(posX + offset, 0, posY);
     } else if (posY == 0) {
         target = glm::vec3(posX, 0, posY - offset);
     } else { // posY == cols - 1
         target = glm::vec3(posX, 0, posY + offset);
     }
    auto wallToRemove = std::find_if(walls.begin(),walls.end(), [&](const Wall& wall){
        return wall.position == target;
    });

    if (wallToRemove != walls.end()) {
     wallToRemove->visible = false;
     }


 }


void Grid::generateGrid(std::vector<Wall> &walls) {
     float offset= 0.5f;
     for (auto &tile: tiles) {

         glm::vec3 target;
         if (tile->posX == 0) {
             target = glm::vec3(tile->posX - offset, 0, tile->posY);
         } else if (tile->posX == rows - 1) {
             target = glm::vec3(tile->posX + offset, 0, tile->posY);
         } else if (tile->posY == 0) {
             target = glm::vec3(tile->posX, 0, tile->posY - offset);
         } else { // posY == cols - 1
             target = glm::vec3(tile->posX, 0, tile->posY + offset);
         }
         for (auto &wall : walls) {
            if (target == wall.position) {
                tile->wall = &wall;
            }

         }
     }


 }
