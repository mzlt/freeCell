#include "ofApp.h"
#include "deck.hpp"

//--------------------------------------------------------------
void ofApp::setup() {
    d.newGame(); // set up a new random game
    timer = 0;
    score = 0;
}

//--------------------------------------------------------------
void ofApp::draw(){
    d.draw(); // draw game
    ofPushStyle();
    drawTopBar(); // draw top bar
    if(!d.getEnough()) drawDialog("NOT ENOUGH SPACE TO MOVE", ofGetWidth()/2-150, ofGetHeight()-60, 300, 50); // draw not enough
    if(d.getNoMore()) drawDialog("NO MORE POSSIBLE MOVES", ofGetWidth()/2-150, ofGetHeight()-60, 300, 50); // draw no more moves
    if(d.getAutocomplete()) drawAutocomplete(); // draw autocomplete dialog window
    if(d.getFinished()) drawFinished(); // draw game complete dialog window
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawTopBar(){
    ofSetColor(150);
    ofDrawRectangle(0, 0, ofGetWidth(), 50); // background
    ofSetColor(0);
    ofDrawLine(0, 50, ofGetWidth(), 50); // border
    un->draw(); // undo
    hi->draw(); // hint
    re->draw(); // reset
    ng->draw(); // new game
    ofDrawBitmapString("SCORE:" + ofToString(d.getScore()), ofGetWidth() - 300, 30); // current score
    ofDrawBitmapString("MOVES:" + ofToString(d.getMoves()), ofGetWidth() - 200, 30); // current moves
    ofDrawBitmapString(d.getTime(), ofGetWidth() - 100, 30); // current time
}

//--------------------------------------------------------------
void ofApp::drawDialog(string t, float x, float y, int w, int h){
    ofPushStyle();
    ofSetColor(160);
    ofFill();
    ofDrawRectangle(x, y, w, h); // background
    ofSetColor(0);
    ofNoFill();
    ofDrawRectangle(x, y, w, h); // border
    int width = t.length() * 8; // text width
    int align = (w - width) / 2; // center align
    ofDrawBitmapString(t, x + align, y + 30); // draw text
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawAutocomplete(){
    timer++; // prevents accidental clicks
    drawDialog("AUTOCOMPLETE ?", (ofGetWidth()/2) - 200, (ofGetHeight()/2) - 150, 400, 100); // draw basic dialog window
    yes->draw(); // draw yes button
    no->draw(); // draw no button
}

//--------------------------------------------------------------
void ofApp::drawFinished(){
    drawDialog("CONGRATULATIONS!", (ofGetWidth()/2) - 200, (ofGetHeight()/2) - 150, 400, 230); // draw basic dialog window
    float yourX = (ofGetWidth()/2) - 150; // your score x position
    float bestX = (ofGetWidth()/2) + 20; // best score x position
    float scoreY = (ofGetHeight()/2) - 50; // first score y position
    ofFill();
    drawScore(bScore, "YOUR SCORE:" + ofToString(score), "BEST SCORE:" + ofToString(bestScore), yourX, bestX, scoreY);
    drawScore(bMoves, "YOUR MOVES:" + ofToString(d.getMoves()), "BEST MOVES:" + ofToString(bestMoves), yourX, bestX, scoreY + 20);
    drawScore(bTime, "YOUR " + d.getTime(), "BEST " + bestTime, yourX, bestX, scoreY + 40);
    newGame->draw(); // draw start a new game button
}

//--------------------------------------------------------------
void ofApp::drawArrow(const float & x, const float & y){
    ofDrawRectangle(x - 15, y - 2, 10, 4);
    ofDrawTriangle(x - 5, y - 5, x - 5, y + 5, x - 2, y);
}

//--------------------------------------------------------------
void ofApp::drawScore(bool c, const string s1, const string s2, const float x1, const float x2, const float y){
    if(c) drawArrow(x1 - 5, y - 5); // draw arrow next to your score if beaten best score
    ofDrawBitmapString(s1, x1, y); // draw your score
    ofDrawBitmapString(s2, x2, y); // draw best score
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    d.mousePressed(); // deals with the game [cards and piles]
    if(un->getHover()) un->mousePressed(&Deck::undo, d); // undo button
    if(hi->getHover()) hi->mousePressed(&Deck::hint, d); // hint button
    if(re->getHover()) re->mousePressed(&Deck::refresh, d); // restart button
    if(ng->getHover()) ng->mousePressed(&Deck::newGame, d); // new game button
    if(d.getAutocomplete() && timer >= 30) { // if game is solved for half a second
        if(no->getHover()) no->mousePressed(&Deck::skipAutocomplete, d); // don't autocomplete button
        if(yes->getHover()) yes->mousePressed(&Deck::doAutocomplete, d); // autocomplete button
        timer = 0; // reset timer
    }
    if(d.getFinished()) { // if the game is finished
        score = d.getScore() + 1000000 / d.getSeconds();; // calculate final score
        saveScore(); // save score to xml file
        getBScore(); // get best score from xml and compare it to this game score
        if(newGame->getHover()) newGame->mousePressed(&Deck::newGame, d); // new game button
    }
}

//--------------------------------------------------------------
void ofApp::saveScore(){
    XML.loadFile("scoreTable.xml"); // load file
    XML.addTag("entry"); // create new entry
    XML.pushTag("entry", XML.getNumTags("entry") - 1); // push into newly created entry
    XML.addValue("score", score); // save score
    XML.addValue("seconds", d.getSeconds()); // save time in seconds
    XML.addValue("time", d.getTime()); // save time as string
    XML.addValue("moves", d.getMoves()); // save number of moves
    XML.popTag(); // pop out of the entry
    XML.saveFile("scoreTable.xml"); // save updated settings
}

//--------------------------------------------------------------
void ofApp::getBScore(){
    emptyScores(); // prepare variables
    XML.loadFile("scoreTable.xml"); // load file
    for(int i = 0; i<XML.getNumTags("entry"); i++){ // for all entries
        XML.pushTag("entry", i); // push into each entry
        if(XML.getValue("score", 0) > bestScore) bestScore = XML.getValue("score", 0); // find best score
        if(XML.getValue("seconds", 0) < bestSeconds) {
            bestSeconds  = XML.getValue("seconds", 0); // find best time in seconds
            bestTime  = XML.getValue("time", ""); // find best time as string
        }
        if(XML.getValue("moves", 0) < bestMoves) bestMoves = XML.getValue("moves", 0); // find best moves
        XML.popTag();
    }
    if(bestScore <= score) bScore = true; // was this the best score?
    if(bestSeconds >= d.getSeconds()) bTime = true; // was this the best time?
    if(bestMoves >= d.getMoves()) bMoves = true; // were those the best moves?

}

//--------------------------------------------------------------
void ofApp::emptyScores(){
    bScore = false;
    bTime = false;
    bMoves = false;
    bestScore = 0;
    bestSeconds = 100000;
    bestTime = "";
    bestMoves = 100000;
}
