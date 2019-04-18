

#ifndef regular_hpp
#define regular_hpp

#include "ofMain.h"
#include "pile.hpp"

class Regular : public Pile {
public:
    Regular(ofVec2f pos, ofVec2f s, bool top);
    void draw();
};

#endif /* regular_hpp */
