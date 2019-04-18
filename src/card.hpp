

#ifndef card_hpp
#define card_hpp

#include "ofMain.h"


//------------------------------------------------------------------------------

class Card {
public:
    Card(int val);
    void draw();
    void setHint(bool h);
    bool getHint();
    void setRow(int r);
    int getRow();
    void setOnTop(bool t);
    bool getOnTop();
    void setPosition(ofVec2f p);
    ofVec2f getPosition();
    void setActive(bool a);
    bool getActive();
    void setInteractive(bool i);
    bool getInteractive();
    void setTop(bool t);
    bool getTop();
    void pushPrevFcs(const int & idx);
    void popPrevFcs();
    int getPrevFcs(const int & size);
    int getPrevFcsSize();
    const ofVec2f getSize();
    const bool getColour();
    const int getRank();
    const int getSuit();
private:
    // initialised before setup (used for calc suit and rank)
    int value; // card's unique id
    // setup (used for loading texture)
    const char* suits = "CDHS";
    const char* ranks = "A23456789TJQK";
    // whole program
    ofImage face; // card's texture
    bool color; // card's color
    int rank; // card's rank
    int suit; // card's suit
    ofVec2f position; // card's position
    int row; // card's row
    ofVec2f size; // card's size
    bool active; // is the card active
    bool interactive; // can the card be clicked
    bool top; // is the card on top of the column
    bool onTopOfActive; // is the card on top of the active card
    bool hint; // if hint is on
    vector<int> prevFcs; // used in undo collects information of index no in the free cell
    void drawFace(); // draw card
    void drawOverlay(bool condition, ofColor col); // draw overlay
};

#endif /* card_hpp */
