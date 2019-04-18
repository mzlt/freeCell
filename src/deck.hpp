

#include "card.hpp"
#include "pile.hpp"
#include "regular.hpp"
#include "freecell.hpp"
#include "home.hpp"
#include "ofMain.h"


#ifndef deck_hpp
#define deck_hpp


class Deck {
public:
    Deck();
    void newGame();
    void refresh();
    void draw();
    bool getEnough();
    bool getNoMore();
    bool getAutocomplete();
    void mousePressed();
    void undo();
    void hint();
    void skipAutocomplete();
    void doAutocomplete();
    bool getFinished();
    int getMoves();
    int getScore();
    string getTime();
    int getSeconds();
private:
    // typedefs
    typedef pair<int,int> pint;
    template<class Piles>
    using pil = vector<shared_ptr<Piles>>;
    // game state variables
    int deckID;
    bool un;
    bool act;
    int cAtHome;
    int moves;
    bool hin;
    ofVec2f hintPos;
    bool autocomplete;
    bool dontAutocomplete;
    bool finished;
    string time = "";
    int sec;
    int score;
    bool enough;
    bool noMore;
    // vectors
    pil<Card> cards; // cards
    pil<Pile> regs; // regular cells
    pil<Pile> fcells; // free cells
    pil<Pile> homes; // home cells
    vector<array<int,4>> history; // undo vector
    // setup
    void arrangeCards(int GI);
    int RNG(int seed);
    void makePretty();
    void setupInteractivity();
    void checkForInteractive(const int &idx);
    void setupPiles();
    void measureTime();
    void drawHint();
    void deactivateStates();
    bool canActivate();
    template <class Piles>
    int find(const pil<Piles> &vec, const int target);
    template <class Piles>
    bool clicked(const pil<Piles> &vec, const int &i);
    void activateCard(const int &idx);
    pint findActive();
    template <class Piles>
    bool check(const pil<Piles> &vec, const pint &ac, const int target);
    bool anotherCard(const int &np, const pint & ac);
    bool enoughSpace(const int &onTop, bool reg);
    bool checkHomes(const pint &ac, const int &newPos);
    void moveCard(const pint &ac, const pair <int,int> &np);
    void occupyHistory(const pint &ac, const pint &np);
    void occupyNp(const pint &np);
    void fcHistory(const pint &np, const int &active);
    template <class Piles>
    int freePile(const pil<Piles> &vec, const int &active);
    void newHistoryEntry(const int &np, const int type, const int adj);
    void regHistory(const pint &np, const pint &ac);
    template <class Piles>
    void historyBiggerRow(const pil<Piles> &vec, const int &idx, const pint &np, const int &onTop, const int &row);
    void cardHistory(const pint &np, const pint &ac);
    int freeCard(const int &active);
    void historyCardToCard(const int &idx, const pint &np, const int &onTop);
    template <class Piles>
    void historySmallerRow(const pil<Piles> &vec, const int &idx, const pint &np, const int &onTop, const int &row);
    int updateLocation(const pint &ac, const pint &np);
    int updatePosition(const pint &ac, const pint &np);
    int assesSituation(const int &nr, const int &activeC);
    int findUpdatedIndex(const int &active, const int &row);
    int findHighestIndex(const int &row);
    void reaarange(const int &row, const int &indexToMove, const int &newIndex);
    void newActiveHistory(const int &newA, const int &target);
    void moveCardsOnTop(const int & onTop, int & newA);
    void checkAutocomplete();
    bool autocompleteColumn(int idx);
    int autocompleteCard(const int & idx);
    void checkFinished();
    void deactivate(const int &ac);
    void deactivateAllCards();
    void undoHome(const int &idx);
    bool checkHint(const bool fc, const int target);
    int hintFindIdx(const bool & fc, const int & sourceIdx, const int & target);
    template <class Piles>
    bool hintFindTarget(const pil<Piles> &vec, const int &idx, const int &target);
    void setHint(const int &idx, const int &targetIdx, const int &target);
    int autoFindIdx(const int &i);
};

#endif /* deck_hpp */
