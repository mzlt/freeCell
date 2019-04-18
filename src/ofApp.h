#pragma once

#include "ofMain.h"
#include "deck.hpp"
#include "button.hpp"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void draw();
        void drawTopBar();
        void drawDialog(string t, float x, float y, int w, int h);
        void drawAutocomplete();
        void drawFinished();
        void drawArrow(const float & x, const float & y);
        void drawScore(bool c, const string s1, const string s2, const float x1, const float x2, const float y);
		void mousePressed(int x, int y, int button);
        void saveScore();
        void getBScore();
        void emptyScores();
        Deck d; // game
        //buttons
        unique_ptr<Button> un = make_unique<Button>("UNDO", 10, 10);
        unique_ptr<Button> hi = make_unique<Button>("HINT", 110, 10);
        unique_ptr<Button> re = make_unique<Button>("RESET", 210, 10);
        unique_ptr<Button> ng = make_unique<Button>("NEW GAME", 310, 10);
        unique_ptr<Button> yes = make_unique<Button>("YES", (ofGetWidth()/2) - 150, (ofGetHeight()/2) - 100);
        unique_ptr<Button> no = make_unique<Button>("NO", (ofGetWidth()/2) + 50, (ofGetHeight()/2) - 100);
        unique_ptr<Button> newGame = make_unique<Button>("NEW GAME", (ofGetWidth()/2) - 50, (ofGetHeight()/2) + 20);
        int timer; // fix for autocomplete
        // scoring
        ofxXmlSettings XML;
        int score;
        bool bScore;
        bool bTime;
        bool bMoves;
        int bestScore;
        int bestSeconds;
        string bestTime;
        int bestMoves;

};
