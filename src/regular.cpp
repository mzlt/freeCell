
#ifndef home_hpp
#define home_hpp

#include "ofMain.h"
#include "pile.hpp"

class Home : public Pile {
public:
    Home(ofVec2f pos, ofVec2f s, bool top);
    void draw();
    void setSuit(int s);
    int getSuit();
    void setCRank(int cr);
    int getCRank();
private:
    int currentRank;
    int suit = -1;
    ofImage homeImage;

};

#endif /* home_hpp */
