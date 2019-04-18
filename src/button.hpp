

#ifndef button_hpp
#define button_hpp
#include "ofMain.h"
#include "deck.hpp"

//------------------------------------------------------------------------------

class Button {
public:
    Button(string lab, float x, float y);
    void draw();
    void mousePressed(void (Deck::*action)(), Deck& obj);
    bool getHover();


private:
    string label;
    float xPos;
    float yPos;
    int xSize = 90;
    int ySize = 30;
};

#endif /* button_hpp */
