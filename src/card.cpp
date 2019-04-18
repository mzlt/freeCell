
#include "card.hpp"

//--------------------------------------------------------------
Card::Card(int val) : value(val) {
    // begin from https://rosettacode.org/wiki/Deal_cards_for_FreeCell#OOP_version
    rank = value / 4; // calc rank value
    suit = value % 4; // calc suit value
    stringstream s;
    s << ranks[rank] << suits[suit]; // create file name
    // end from https://rosettacode.org/wiki/Deal_cards_for_FreeCell#OOP_version
    face.load("ca/"+s.str()+".png"); // load image
    if(suit == 1 || suit == 2) color = 0; // asign color black
    else color = 1; // asign color red
    size.x = ofGetWidth()/10; // calc size x
    float scale = face.getWidth()/size.x; // calc scale
    size.y = face.getHeight()/scale; // calc size y and scale it
    // VARIABLE
    active = false; // card is passive
    hint = false; // hint is off
    onTopOfActive = false; // card is not on top of another active card
    active = false; // card is passive
    interactive = false; // card can't be clicked
    top = false; // card is not on top of the column
}


//--------------------------------------------------------------
void Card::draw() {
    drawFace(); // draw a card
    drawOverlay(active || onTopOfActive, ofColor(255, 0, 0, 20)); // mark as active or on top of it
    drawOverlay(hint, ofColor(0, 0, 255, 20)); // mark as hint or on top of it
}

//--------------------------------------------------------------
void Card::drawFace(){
    ofPushStyle();
    ofSetColor(0);
    ofNoFill();
    ofDrawRectangle(position.x, position.y, size.x, size.y); // draw a nice border around the card
    ofPopStyle();
    face.draw(position.x,position.y,size.x,size.y); // draw the card
}

//--------------------------------------------------------------
void Card::drawOverlay(bool condition, ofColor col){
    ofPushStyle();
    if (condition) {
        ofSetColor(col);
        ofDrawRectangle(position.x, position.y, size.x, size.y); // draw coloured overlay
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void Card::setHint(bool h){
    hint = h;
}

//--------------------------------------------------------------
bool Card::getHint(){
    return hint;
}

//--------------------------------------------------------------
void Card::setRow(int r){
    row = r;
}

//--------------------------------------------------------------
int Card::getRow(){
    return row;
}

//--------------------------------------------------------------
void Card::setOnTop(bool t){
    onTopOfActive = t;
}

//--------------------------------------------------------------
bool Card::getOnTop(){
    return onTopOfActive;
}

//--------------------------------------------------------------
void Card::setPosition(ofVec2f p){
    position = p;
}

//--------------------------------------------------------------
ofVec2f Card::getPosition(){
    return position;
}

//--------------------------------------------------------------
void Card::setActive(bool a){
    active = a;
}

//--------------------------------------------------------------
bool Card::getActive(){
    return active;
}

//--------------------------------------------------------------
void Card::setInteractive(bool i){
    interactive = i;
}

//--------------------------------------------------------------
bool Card::getInteractive(){
    return interactive;
}

//--------------------------------------------------------------
void Card::setTop(bool t){
    top = t;
}

//--------------------------------------------------------------
bool Card::getTop(){
    return top;
}

//--------------------------------------------------------------
void Card::pushPrevFcs(const int & idx){
    prevFcs.push_back(idx);
}

//--------------------------------------------------------------
void Card::popPrevFcs(){
    prevFcs.pop_back();
}

//--------------------------------------------------------------
int Card::getPrevFcsSize(){
    return prevFcs.size();
}

//--------------------------------------------------------------
int Card::getPrevFcs(const int & size){
    return prevFcs[size];
}

//--------------------------------------------------------------
const ofVec2f Card::getSize(){
    return size;
}

//--------------------------------------------------------------
const bool Card::getColour(){
    return color;
}

//--------------------------------------------------------------
const int Card::getRank(){
    return rank;
}

//--------------------------------------------------------------
const int Card::getSuit(){
    return suit;
}
