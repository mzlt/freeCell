

#include "home.hpp"
#include "pile.hpp"

//--------------------------------------------------------------
Home::Home(ofVec2f pos, ofVec2f s, bool top) : Pile(pos, s, top) {
//    position.set(pos.x, pos.y);
    setPosition(pos);
    setSize(s);
    setOnTop(top);
    currentRank = 0;
    homeImage.load("home.png");
}


//--------------------------------------------------------------
void Home::draw() {
    ofPushStyle();
    ofSetColor(0); // freeCell
    ofNoFill();
    ofDrawRectangle(getPosition().x, getPosition().y, getSize().x, getSize().y);
    ofFill();
    ofSetColor(0,50); // freeCell
    homeImage.draw(getPosition().x, getPosition().y, getSize().x, getSize().y);
    ofPopStyle();
}

//--------------------------------------------------------------
void Home::setSuit(int s) {
    suit = s;
}

//--------------------------------------------------------------
int Home::getSuit() {
    return suit;
}

//--------------------------------------------------------------
void Home::setCRank(int cr) {
    currentRank = cr;
}

//--------------------------------------------------------------
int Home::getCRank() {
    return currentRank;
}
