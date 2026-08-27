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
             cells.push_back(std::make_unique<Cell>(glm::vec2(i,j)));
                for (int k = 0; k < 5; k++) {
                    getCell(i,j)->tiles[k] = std::make_shared<Tile>();
                }
             getCell(i,j)->tiles[NONE]->position = glm::vec3(i,0.0f,j);
             getCell(i,j)->tiles[N]->position = glm::vec3(i-1.0f,0.0f,j);
             getCell(i,j)->tiles[S]->position = glm::vec3(i+1.0f,0.0f,j);
             getCell(i,j)->tiles[W]->position = glm::vec3(i,0.0f,j-1.0f);
             getCell(i,j)->tiles[E]->position = glm::vec3(i,0.0f,j+1.0f);
         }
     }


}



glm::vec3 Grid::generateMaze() {

    std::mt19937 r_device(std::random_device{}());
    std::uniform_int_distribution<uint> maxY(0,rows -1);
    std::uniform_int_distribution<uint> maxX(0,cols -1);

    int posX = maxX(r_device);
    int posY = maxY(r_device);
     glm::vec2 position(posX,posY);
     glm::vec3 CameraPos(position.x,0,position.y);
    std::vector<Cell*> frontier, adjecent;
    mark(position.x,position.y,frontier);

    while (!frontier.empty()) {

        std::uniform_int_distribution<int> length(0,frontier.size()-1);
        int index = length(r_device);
        Cell* current = frontier.at(index);
        position = current->position;

        frontier[index] = frontier.back();
        frontier.pop_back();

        adjecent = neighbours(position.x,position.y);
        std::uniform_int_distribution<int> neigbour(0, adjecent.size() -1);
        index = neigbour(r_device);
        Cell* n = adjecent.at(index);
        int nx = n->position.x;
        int ny = n->position.y;
        BIT_DIR dir = directions(position.x, position.y,nx,ny);
        getCell(position.x,position.y)->direction |= dir;
        getCell(nx,ny)->direction |= opositions(dir);
        mark(position.x,position.y,frontier);

    }
     return CameraPos;

}


void Grid::mark(int posX, int posY,std::vector<Cell*> &frontier) {
     getCell(posX,posY)->not_carved = false;
     addFrontier(posX-1,posY,frontier);
     addFrontier(posX+1,posY,frontier);
     addFrontier(posX,posY-1,frontier);
     addFrontier(posX,posY+1,frontier);
 }

void Grid::addFrontier(int posX,int posY ,std::vector<Cell*> &frontier) {
        auto tmp = getCell(posX,posY);
        if (tmp != nullptr && tmp->frontier == false && tmp->not_carved == true) {
            tmp->frontier = true;
            frontier.push_back(tmp);
        }
}


std::vector<Cell*>  Grid::neighbours(int posX,int posY) const {
        std::vector<Cell*> neighbours;
        if (posX > 0 &&  getCell(posX -1, posY)->not_carved == false) {
            neighbours.push_back(getCell(posX-1,posY));
        }
        if (posX +1 < cols && getCell(posX+1,posY)->not_carved == false) {
            neighbours.push_back(getCell(posX+1,posY));
        }
        if (posY > 0 && getCell(posX,posY-1)->not_carved == false) {
            neighbours.push_back(getCell(posX,posY-1));
        }
        if (posY + 1 <  rows && getCell(posX,posY+1)->not_carved == false) {
            neighbours.push_back(getCell(posX,posY+1));
        }

    return neighbours;

}

BIT_DIR Grid::directions(int fx,int fy,int tx,int ty) {

    if (fx < tx) {
        return   E_BIT;
    }
    if (fx > tx) {
        return   W_BIT;
    }
    if (fy < ty) {
        return S_BIT;
    }

    if (fy > ty) {
        return N_BIT;
    }

}


glm::vec3 Grid::generateWalls() {
    glm::vec3 startPos = generateMaze();
    float offset = 0.5f;

    for (auto &cell : cells){


        if (cell->position.x == 0) {
            glm::vec3 pos = glm::vec3(cell->position.x - offset, 0 , cell->position.y);
            cell->tiles[W]->wall = addWall(pos,true);
        }

        if (cell->position.y == 0) {
            glm::vec3 pos = glm::vec3(cell->position.x , 0 , cell->position.y - offset);
            cell->tiles[N]->wall = addWall(pos,false);
        }


        // Right border
        if (cell->position.x == cols - 1) {
            glm::vec3 pos = glm::vec3(cell->position.x + offset,0, cell->position.y);
            cell->tiles[E]->wall = addWall(pos,true);
        }
        // Bottom border
        if (cell->position.y == rows - 1) {

            glm::vec3 pos = glm::vec3(cell->position.x,0.0f,cell->position.y + offset);
            cell->tiles[S]->wall = addWall(pos,false);
        }

        if (cell->position.x < cols - 1 &&!(cell->direction & E_BIT))
        {       glm::vec3 pos = glm::vec3(cell->position.x + offset, 0.0f, cell->position.y);
                cell->tiles[E]->wall = addWall(pos,true);
        }

        if (cell->position.y < rows - 1 &&!(cell->direction & S_BIT)){
                glm::vec3 pos = glm::vec3(cell->position.x,0.0f,cell->position.y +offset);
                cell->tiles[S]->wall = addWall(pos,false);
        }

    }
     createExit();
     return startPos;

}



void Grid::printMaze() const {
     for (int y = 0; y < rows; ++y) {

         // NORTH WALLS
         for (int x = 0; x < cols; ++x) {
             Cell* cell = getCell(x, y);

             std::cout << "#";

             if (cell->direction & N_BIT)
                 std::cout << " ";
             else
                 std::cout << "#";
         }

         std::cout << "#\n";


         // WEST / EAST WALLS
         for (int x = 0; x < cols; ++x) {
             Cell* cell = getCell(x, y);

             if (cell->direction & W_BIT)
                 std::cout << " ";
             else
                 std::cout << "#";

             std::cout << " ";
         }

         // EAST BORDER
         Cell* last = getCell(cols - 1, y);

         if (last->direction & E_BIT)
             std::cout << " ";
         else
             std::cout << "#";

         std::cout << "\n";
     }


     // SOUTH WALLS
     for (int x = 0; x < cols; ++x) {
         Cell* cell = getCell(x, rows - 1);

         std::cout << "#";

         if (cell->direction & S_BIT)
             std::cout << " ";
         else
             std::cout << "#";
     }

     std::cout << "#\n";
 }


std::shared_ptr<Wall> Grid::addWall( const glm::vec3& pos, bool vertical)
{
    for (const auto& wall : walls) {
        if (wall->position == pos) {

            return nullptr;
        }
    }
     if (vertical) {
    std::shared_ptr<Wall> wall = std::make_shared<Wall>(pos,vertical,
        BoundingBox(pos.x - 0.05f, -0.5f,pos.z-0.5f,pos.x+0.05f , 0.5f,pos.z +0.5f));
    walls.push_back(wall);
         return  wall;
     }else {
         std::shared_ptr<Wall> wall = std::make_shared<Wall>(pos,vertical,
             BoundingBox(pos.x - 0.5f, -0.5f,pos.z-0.05f,pos.x+0.5f , 0.5f,pos.z +0.05f));
         walls.push_back(wall);
         return wall;
     }
}



void Grid::createExit() {
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

     removeWall(posX,posY);

 }


void Grid::removeWall(int posX, int posY) const  {
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
    auto wallToRemove = std::find_if(walls.begin(),walls.end(), [&](const std::shared_ptr<Wall>& wall){
        return wall->position == target;
    });

    if (wallToRemove != walls.end()) {
     (*wallToRemove)->visible = false;
     }


 }




//**Getters and Setters**//


Cell* Grid::getCell(glm::vec2 position) const{
     for (auto& cell : cells){
        if (position.x == cell->position.x && position.y == cell->position.y) {
            return cell.get();
        }
     }
     return nullptr;
 }

Cell* Grid::getCell(int posX, int posY) const{
     for (auto& cell : cells){
         if (posX == cell->position.x && posY == cell->position.y) {
             return cell.get();
         }
     }
     return nullptr;
 }
