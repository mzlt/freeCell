
#include "button.hpp"


//--------------------------------------------------------------
Button::Button(string lab, float x, float y) : label(lab), xPos(x), yPos(y) {}

//--------------------------------------------------------------
void Button::draw() {
    ofPushStyle();
    if(getHover()){ // lighter color when hovered over
        ofFill();
        ofSetColor(200);
        ofDrawRectangle(xPos, yPos, xSize, ySize);
    }
    ofSetColor(0);
    ofNoFill();
    ofDrawRectangle(xPos, yPos, xSize, ySize);
    int labelWidth = label.length() * 8;
    int labelX = (xSize - labelWidth) / 2; // center align the text within the button
    ofDrawBitmapString(label, xPos + labelX, yPos + 20);
    ofPopStyle();

}

//--------------------------------------------------------------
void Button::mousePressed(void (Deck::*action)(), Deck& obj) {
    (obj.*action)(); // takes a pointer to the function from the deck
}

//--------------------------------------------------------------
bool Button::getHover() { // returns true if the mose is over the button
    return ofGetMouseX() > xPos &&
    ofGetMouseY() > yPos &&
    ofGetMouseX() < xPos + xSize &&
    ofGetMouseY() < yPos + ySize;
}
