

#ifndef pile_hpp
#define pile_hpp

#include "ofMain.h"
//#include "regular.hpp"

class Pile {
public:
    // common to all piles
    Pile(ofVec2f pos, ofVec2f s, bool top) {};
    void setPosition(ofVec2f p) { position = p; };
    const ofVec2f getPosition() { return position; };
    void setSize(ofVec2f s) { size = s; };
    const ofVec2f getSize() { return size; };
    void setOnTop(bool c) { onTop = c; };
    bool getOnTop() { return onTop; };
    // pile specific
    virtual  void draw() {};
    // only in Home
    virtual void setSuit(int s) {};
    virtual int getSuit() {};
    virtual void setCRank(int cr) {};
    virtual int getCRank() {};
private:
    // common to all piles
    ofVec2f position;
    ofVec2f size;
    bool onTop;
};

#endif /* pile_hpp */
