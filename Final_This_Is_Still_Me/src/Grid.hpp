//
//  Grid.hpp
//  Final_This_Is_Still_Me
//
//  Created by Jerry-Wanglaws on 4/23/19.
//
#pragma once

#ifndef Grid_hpp
#define Grid_hpp

#include <stdio.h>
#include <ofMain.h>

#endif /* Grid_hpp */

class Grid{
    
public:
    
    glm::ivec2 pos; // the position of the grid
    
    string mode; // check what is this grid for
    
    double moduleWidth; // the width of each module
    double moduleHeight; // the height of each module
    
    bool solid; // check if the grid is occupied
    
    Grid(glm::ivec2 _pos, string _mode);
    Grid(glm::ivec2 _pos, string _mode, double _moduleWidth, double _moduleHeight);
    void setup();
    void update(double _moduleWidth, double _moduleHeight);
    void draw();
    void checkMode();
    
};
