

#include "deck.hpp"

#define NUMBER_OF_CARDS 52
#define NUMBER_OF_COLUMNS 8
#define SPACING 26
#define TOP 50

//--------------------------------------------------------------
Deck::Deck() {}

//--------------------------------------------------------------
void Deck::newGame() {
    deckID = ofRandom(32000); // assign deck id
    refresh(); // set up the new game
}

//--------------------------------------------------------------
void Deck::refresh() {
    arrangeCards(deckID); // deal new deck
    makePretty(); // set up positions of each card
    setupInteractivity(); // set up interactive and top
    setupPiles(); // set up Free, Home and Regular cells
    act = false; // state is not active
    cAtHome = 0; // there is no cards at home
    un = false; // undo is not happening
    hin = false; // hint is not happening
    autocomplete = false; // autocomplete is not happening
    dontAutocomplete = true; // autocomplete is not happening
    finished = false; // game is not finished
    enough = true; // enough space anoucement is off
    noMore = false; // no more moves anoucement is off
    moves = 0; // no moves have been taken
    score = 0; // score is 0
    sec = 0; // game lasted 0 seconds
    ofResetElapsedTimeCounter(); // reset time
}

//--------------------------------------------------------------
void Deck::arrangeCards(int GI) { // partially from https://rosettacode.org/wiki/Deal_cards_for_FreeCell#OOP_version
    if(cards.size() != 0) cards.clear(); // empty vector in case of reseting or starting new the game
    for (int i = 0; i < NUMBER_OF_CARDS; i++) { // for each card
        shared_ptr<Card> c (new Card((NUMBER_OF_CARDS - 1) - i)); // create card
        cards.push_back(move(c)); // push it into the vector
    }
    for (int i = 0; i < (cards.size() - 1); i++) { // for each card
        int j = (cards.size() - 1) - RNG(GI) % (cards.size() - i); // choose card to swap with
        swap(cards[i], cards[j]); // swap cards
    }
}

//--------------------------------------------------------------
int Deck::RNG(int seed) { // from https://rosettacode.org/wiki/Deal_cards_for_FreeCell#OOP_version
    return (seed = (seed * 214013+2531011) & (1U << 31) - 1) >> 16; // generate random number based on the seed
}

//--------------------------------------------------------------
void Deck::makePretty() {
    int y = 0; // initialise y position of the card
    int row = -1; // initialise row position
    float cardSpace = cards[0]->getSize().x * 1.1; // card with horizontal spacing
    float margins = (ofGetWidth() - NUMBER_OF_COLUMNS * cardSpace) / 2; // calc deck's distance from the left
    for(int i = 0; i < cards.size(); i++) { // for each card
        if(i % NUMBER_OF_COLUMNS == 0) { // for each row
            if(i != 0) y += SPACING; // increase y position
            row += 1; // increase row index
        }
        float xPos = margins + (i % NUMBER_OF_COLUMNS * cardSpace); // cals card's x position
        float yPos = y + ofGetHeight()/4; // cals card's y position
        cards[i]->setPosition(ofVec2f(xPos,TOP + yPos));  // set card's position
        cards[i]->setRow(row); // set row index
    }
}

//--------------------------------------------------------------
void Deck::setupInteractivity() {
    for(int i = 0; i < cards.size(); i++) {
        if(i <= cards.size() - 1 && i > cards.size() - (NUMBER_OF_COLUMNS + 1)) { // cards on top of each column
            cards[i]->setInteractive(1); // are clickable
            cards[i]->setTop(1); // and are on top of each column
            checkForInteractive(i);
        } else { // have cards on top
            cards[i]->setInteractive(0); // are not clickable
            cards[i]->setTop(0); // and have cards on top
        }
    }
}

//--------------------------------------------------------------
void Deck::checkForInteractive(const int & idx) {
    for(int i = 0; i < cards.size(); i++) {
        if(cards[i]->getPosition().y == cards[idx]->getPosition().y - SPACING && // if the card is directly above
           cards[i]->getPosition().x == cards[idx]->getPosition().x && // and in the same column
           cards[i]->getRank() == cards[idx]->getRank() + 1 && // and has a rank one higher
           cards[i]->getColour() != cards[idx]->getColour()) { // and different colour
            cards[i]->setInteractive(1); // make it interactive
            checkForInteractive(i); // and check the card above
        }
    }
}

//--------------------------------------------------------------
void Deck::setupPiles() {
    if(regs.size() != 0) regs.clear(); // empty vector in case of reseting or starting new the game
    if(homes.size() != 0) homes.clear(); // empty vector in case of reseting or starting new the game
    if(fcells.size() != 0) fcells.clear(); // empty vector in case of reseting or starting new the game
    float width = ofGetWidth()/9; // top pile width with horizontal spacing
    float spaceH = width - cards[0]->getSize().x; // gap between two piles
    float gap = ofGetWidth() - (spaceH + NUMBER_OF_COLUMNS/2 * width); // starting point of the second pile
    float yPos = TOP + spaceH; // y position of piles
    for(int i = 0; i < 4; i++) {
        float xPos = spaceH + i * width; // x position of each pile
        shared_ptr<Pile> h (new Home(ofVec2f(xPos, yPos), cards[i]->getSize(), 0)); // create home piles
        shared_ptr<Pile> f (new Regular(ofVec2f(gap + xPos, yPos), cards[i]->getSize(), 0)); // create fc piles
        homes.push_back(move(h)); // create vector of homes
        fcells.push_back(move(f)); // create vector of fcs
    }
    for(int i = 0; i < NUMBER_OF_COLUMNS; i++) { // for the first row
        shared_ptr<Pile> r (new Regular((cards[i]->getPosition()),cards[i]->getSize(), 1)); // create regs
        regs.push_back(move(r)); // create vector of regs
    }
}

//--------------------------------------------------------------
void Deck::draw() {
    for(int i = 0; i < homes.size(); i++) homes[i]->draw();
    for(int i = 0; i < fcells.size(); i++) fcells[i]->draw();
    for(int i = 0; i < regs.size(); i++) regs[i]->draw();
    for(int i = 0; i < cards.size(); i++) cards[i]->draw();
    if(!finished) measureTime();
    if(hin) drawHint(); // highlights a location where the card could be moved to
}

//--------------------------------------------------------------
void Deck::measureTime() {
    int s = (ofGetElapsedTimeMillis()/1000) % 60; // calc seconds
    int m = (ofGetElapsedTimeMillis()/60000); // calc minutes
    string minutes = "";
    string seconds = "";
    if (s < 10) seconds = "0" + ofToString(s); // add 0 for more elegant look
    else seconds = ofToString(s);
    if (m < 10) minutes = "0" + ofToString(m); // add 0 for more elegant look
    else minutes = ofToString(m);
    time = "TIME:" + minutes + ":" + seconds; // time in string format
    sec = ofGetElapsedTimeMillis()/1000; // time in int (seconds) format for easier comparison with best score
}

//--------------------------------------------------------------
bool Deck::getEnough() {
    return enough;
}

//--------------------------------------------------------------
bool Deck::getNoMore() {
    return noMore;
}

//--------------------------------------------------------------
bool Deck::getAutocomplete() {
    return autocomplete;
}

//--------------------------------------------------------------
void Deck::drawHint() {
    // draws a blue arrow
    ofPushStyle();
    ofSetColor(0, 0, 200);
    ofDrawTriangle(hintPos.x, hintPos.y, hintPos.x + 10, hintPos.y + 10, hintPos.x + 10, hintPos.y - 10);
    ofDrawRectangle(hintPos.x + 5, hintPos.y - 5, 50, 10);
    ofPopStyle();
}

//--------------------------------------------------------------
void Deck::mousePressed() {
    deactivateStates(); // deactivate possible annoucements
    if(!autocomplete) {
        if(!act){ // if state passive
            if(canActivate()) act = true; // activate pressed card
        } else { // id state active
            pint active = findActive(); // find active card and the amount of cards on top of it
            // check which category card is moving to and and if the click was ok perform further actions
            if(check(cards, active, 0) || check(regs, active, 1) || check(fcells, active, 2) || check(homes, active, 3)) {
                moves++; // count the moves
                if(dontAutocomplete) checkAutocomplete();
                checkFinished();
            }
            else deactivate(active.first); // unfortunate click
            act = false; // finish and deactivate state
        }
    }
}

//--------------------------------------------------------------
void Deck::deactivateStates() {
    if(hin) { // deactivate hint
        for (int i = 0; i < cards.size(); i++) if (cards[i]->getHint()) cards[i]->setHint(0);
        hin = false;
    }
    if(noMore) noMore = false; // deactivate no more possible moves
    if(!enough) enough = true; // deactivate not enough space
}

//--------------------------------------------------------------
bool Deck::canActivate() {
    int idx = find(cards, 0); // find card's index
    if (idx != -1) { // if index found
        if(cards[idx]->getInteractive() && cards[idx]->getRow() != -11) { // only allow for interactive cards that are not at home
            activateCard(idx);
            return true; // success
        } else {
            return false; // fail
        }
    }
}

//--------------------------------------------------------------
template <class Piles>
int Deck::find(const pil<Piles> &vec, const int target) {
    int idx = -1; // initialise index
    for(int i = 0; i < vec.size(); i++) { // find base pile
        if (clicked(vec, i) && i > idx)  { // if position was clicked
            switch(target) { // depending on the situation
                case 0: idx = i; break; // find a card to activate
                case 1: if (cards[i]->getTop() && cards[i]->getRow() >= 0) idx = i; break; // find a top card to move to
                case 2: if (!vec[i]->getOnTop()) idx = i; break; // find a pile to move to (can't have any cards already there)
            }
        }
    }
    return idx; // return its index
}

//--------------------------------------------------------------
template <class Piles>
bool Deck::clicked(const pil<Piles> &vec, const int &i) {
    return ofGetMouseX() >= vec[i]->getPosition().x &&
    ofGetMouseX() <= vec[i]->getPosition().x + cards[0]->getSize().x &&
    ofGetMouseY() >= vec[i]->getPosition().y &&
    ofGetMouseY() <= vec[i]->getPosition().y + cards[0]->getSize().y;
}

//--------------------------------------------------------------
void Deck::activateCard(const int & idx) {
    cards[idx]->setActive(1); // activate the card
    for(int i = 0; i<cards.size(); i++)
        if(cards[i]->getPosition().x == cards[idx]->getPosition().x && i > idx)
            cards[i]->setOnTop(true); // activate all cards on top of it
}

//--------------------------------------------------------------
pair<int,int> Deck::findActive() {
    pint idx; // create an int, int pair
    for(int i = 0; i < cards.size(); i++){
        if(cards[i]->getActive()) idx.first = i; // find active card index
        if(cards[i]->getOnTop()) idx.second ++; // count cards on top
    }
    return idx;
}

//--------------------------------------------------------------
template <class Piles>
bool Deck::check(const pil<Piles> &vec, const pint & ac, const int target) {
    pint newPos(-1, target); // set up new position pair
    newPos.first = (target == 0) ? find(vec, 1) : find(vec, 2); // moves to card or to pile
    if (newPos.first != -1) { // if new position found and moving to another card
        bool condition;
        switch(target) {
            case 0: condition = anotherCard(newPos.first, ac); break; // new pos is a card chceck if possible to move
            case 1: condition = enoughSpace(ac.second,1); break; // new pos is a regular check if there is enough space to move it
            case 2: condition = cards[ac.first]->getTop(); break; // new pos is a free cell only allowed for top cards
            case 3: condition = checkHomes(ac, newPos.first); break; // new pos is a home check if rank and suit are ok
        }
        if(condition) { // if the move meets condition specific to it's destination
            moveCard(ac, newPos); // move the card
            return true; // return ok
        } else return false; // the move didn't meet it's condition
    } else {
        return false; // the click wasn't made on any location
    }
}

//--------------------------------------------------------------
bool Deck::anotherCard(const int &np, const pint & ac) {
    return ac.first != np && // check if didn't click on the active card
    cards[ac.first]->getColour() != cards[np]->getColour() && // chceck colour
    cards[ac.first]->getRank() == cards[np]->getRank() - 1 && // check rank
    enoughSpace(ac.second,0); // chceck if there is enough space to move
}

//--------------------------------------------------------------
bool Deck::enoughSpace(const int& onTop, bool reg) {
    int emptyRegs = 0;
    int emptyFcells = 0;
    int move = 0;
    for (int i = 0; i< regs.size(); i++) if(!regs[i]->getOnTop()) emptyRegs++; // calc empty regular piles
    for (int i = 0; i< fcells.size(); i++) if(!fcells[i]->getOnTop()) emptyFcells++; // calc empty freeCells piles
    if (emptyRegs == 0) move = emptyFcells; // if no empty regulars we can move by amount of freecells
    else { // if some empty regulars
        if(reg) emptyRegs--; // moving to the regular means we have one less to multiply by
        if (emptyFcells != 0) move = emptyFcells * (emptyRegs + 1); // if there are empty freecells multiply by number of regs
        else move = emptyRegs; // if no empty free cells we can move by number of regulars
    }
    if(move >= onTop) return 1; // if we can move move or equal to cards we try to move we can move
    else {
        enough = false;
        return 0; // not enough space
    }
}

//--------------------------------------------------------------
bool Deck::checkHomes(const pint & ac, const int & newPos) {
    if(homes[newPos]->getCRank() == cards[ac.first]->getRank()) { // check rank
        if(homes[newPos]->getSuit() == -1) { // if home has no suit
            homes[newPos]->setSuit(cards[ac.first]->getSuit()); // assign cards suit
            homes[newPos]->setCRank(cards[ac.first]->getRank() + 1); // set new home rank
            score += 10;
            return true; // success
        } else {
            if (homes[newPos]->getSuit() == cards[ac.first]->getSuit()) {
                homes[newPos]->setCRank(cards[ac.first]->getRank() + 1); // set new home rank
                score += 10;
                return true; // check if the suit is correct - success
            } else return false; // fail
        }
    } else return false; // fail
}

//--------------------------------------------------------------
void Deck::moveCard(const pint & ac, const pint & np) {
    occupyHistory(ac, np); // manage occupied states of affected cards and save information to history
    int newActive = updateLocation(ac, np); // update card's position and index values
    newActiveHistory(newActive, np.second);
    if(un) checkForInteractive(newActive); // in case the card got deactivated while undo was active and cards are aligned
    cards[newActive]->setActive(0); // deactivate card
    if (ac.second != 0) moveCardsOnTop(ac.second, newActive); // move cards on top
}

//--------------------------------------------------------------
void Deck::occupyHistory(const pint & ac, const pint & np) {
    occupyNp(np); // set the state of the pile card is coming to as occupied
    switch(cards[ac.first]->getRow()) { // free the place card is leaving and save information to history
        case -10: fcHistory(np, ac.first); break; // set history when the active card was in free cell
        case -11: break; // used to be home
        case 0: regHistory(np, ac); break; // used to be last one in the column
        default: cardHistory(np, ac); break; // used to be on top of other card
    }
}

//--------------------------------------------------------------
void Deck::occupyNp(const pint & np) {
    switch(np.second) {
        case 0:
            cards[np.first]->setTop(0);
            if(un) cards[np.first]->setInteractive(0);
            break; // new pos is a card - the card we will place stuff on won't be on top
        case 1: regs[np.first]->setOnTop(1); break; // new pos is a regular - mark the pile as full
        case 2: fcells[np.first]->setOnTop(1); break; // new pos is a free cell - mark the pile as full
        case 3: break; // new pos is a home
    }
}

//--------------------------------------------------------------
void Deck::fcHistory(const pint & np, const int & active) {
    int fcellindex = freePile(fcells, active); // find index of the abadoned free cell - free this pile
    if(!un){ // history
        cards[active]->pushPrevFcs(active); // save index if card wasn't yet in fc
        newHistoryEntry(fcellindex, 2, 0); // save the location and index of that location for the destination if undo
    }
}

//--------------------------------------------------------------
template <class Piles>
int Deck::freePile(const pil<Piles> &vec, const int &active) {
    int idx = -1; // set empty index
    for(int i = 0; i < vec.size(); i++){
        if(vec[i]->getPosition().x == cards[active]->getPosition().x) { // find a pile under active card
            vec[i]->setOnTop(0); // set it free
            idx = i; // save its index
        }
    }
    return idx; // return index
}

//--------------------------------------------------------------
void Deck::newHistoryEntry(const int& np, const int type, const int adj) {
    array<int, 4> entry; // create new entry
    entry[0] = -1; // set index of the card to move as empty (this happens later)
    entry[1] = np; // save index of the cards location
    entry[2] = type; // save a type of location
    entry[3] = adj; // sace how much the index of the card to move should be adjusted by
    history.push_back(entry); // push the entry into the history
}

//--------------------------------------------------------------
void Deck::regHistory(const pint & np, const pint & ac) {
    int regindex = freePile(regs, ac.first); // find reg index
    if(!un) { // history
        switch(np.second) {
            case 0: historyBiggerRow(regs, regindex, np, ac.second,  -1); break; // last in the column to another card
            default: newHistoryEntry(regindex, 1, 0); break; // last in the column to FC, HOME or REG
        }
    }
}

//--------------------------------------------------------------
template <class Piles>
void Deck::historyBiggerRow(const pil<Piles> &vec, const int &idx, const pint &np, const int &onTop, const int &row) {
    int shiftBy = 0; // how many cards should the index of active card be adjusted by when undo
    if(vec[idx]->getPosition().x > cards[np.first]->getPosition().x) { // cards going left
        // the differece btw the card under it's future location and the row of a card underneath it's current location
        shiftBy = cards[np.first]->getRow() - row - 1;
        // if the differece same or bigger than total amount of cards to move
        if(shiftBy >= onTop + 1) shiftBy = onTop; // shift by the number of cards on top of the card to move
    } else { // cards going right
        // the differece btw the card under it's future location and the row of a card's current location
        shiftBy = cards[np.first]->getRow() - row;
        // if the differece same or bigger than amount of cards on top of the card to move
        if(shiftBy >= onTop) shiftBy = onTop; // shift by the number of cards on top of the card to move
    }
    int type; // declare type
    if (typeid(vec) == typeid(regs)) type = 1; // if the card moves from reg pile type is regular
    else type = 0; // card moves from another card so type is card
    newHistoryEntry(idx, type, shiftBy); // save the location and index of that location for the destination if undo
}

//--------------------------------------------------------------
void Deck::cardHistory(const pint & np, const pint & ac) {
    int cardindex = freeCard(ac.first);
    if (!un) {
        switch(np.second) {
            case 0: historyCardToCard(cardindex, np, ac.second); break; // from other card to card
            case 1: historySmallerRow(regs, cardindex, np, ac.second, -1); break; // from other card to regular
            default: newHistoryEntry(cardindex, 0, 0); break; // from other card to free cell or home
        }
    }
}

//--------------------------------------------------------------
int Deck::freeCard(const int &active) {
    int idx; // declare index
    for(int i = 0; i<cards.size(); i++){
        if(cards[i]->getPosition().x == cards[active]->getPosition().x && // if same column
           cards[i]->getRow() == cards[active]->getRow() - 1) { // find the card underneath the active card
            cards[i]->setInteractive(1); // make it clickable
            cards[i]->setTop(1); // and mark as the card on top of the pile
            checkForInteractive(i); // chceck if there are any accidentally aligned cards underneath
            idx = i; // save index
        }
    }
    return idx; // return index
}

//--------------------------------------------------------------
void Deck::historyCardToCard(const int &idx, const pint & np, const int &onTop) {
    if(cards[idx]->getRow() < cards[np.first]->getRow()) { // if card moves to the bigger row than currently
        historyBiggerRow(cards, idx, np, onTop, cards[idx]->getRow()); // set history
    } else if(cards[idx]->getRow() > cards[np.first]->getRow()) { // Card moves to the smaller row than currently
        historySmallerRow(cards, idx, np, onTop, cards[np.first]->getRow()); // set history
    } else { // the same row
        newHistoryEntry(idx, 0, 0); // set history
    }
}

//--------------------------------------------------------------
template <class Piles>
void Deck::historySmallerRow(const pil<Piles> &vec, const int &idx, const pint &np, const int &onTop, const int &row) {
    // adjust the index of the card the active card will be coming back to by a differece btw cards row and and the new row
    int howBigger = cards[idx]->getRow() - row;
    if (cards[idx]->getPosition().x > vec[np.first]->getPosition().x) { // if the card is going right
        // if the differecne is bigger then the total amount of cards to move adjust by the total amount of cards to move
        if(howBigger > onTop + 1) howBigger = onTop + 1;
    } else { // if the cards are going left
        howBigger--; // subtract one from that difference
        // if the subtacted difference is bigget than the amount of cards on top of the card to move
        if(howBigger > onTop) howBigger = onTop + 1; // adjust by the total amount of cards to mmove
    }
    newHistoryEntry(idx + howBigger, 0, 0); // save index to history (always moves to another card)
}

//--------------------------------------------------------------
int Deck::updateLocation(const pint & ac, const pint & np) {
    int newRow = updatePosition(ac,np); // update cards position, return it's new row
    int updatedIndex; // will hold the index of the active card after updating indicies
    if(np.second == 0 || np.second == 1) updatedIndex = assesSituation(newRow, ac.first); // cards will be placed within the main deck
    else if(np.second == 2){ // cards will be placed  fc
        updatedIndex = cards.size()-1 - cAtHome;
        if(un) { // comes back to fc during undo
            updatedIndex = cards[ac.first]->getPrevFcs(cards[ac.first]->getPrevFcsSize()-1);
            cards[ac.first]->popPrevFcs();
        }
    } else { // cards will be placed at home
        cAtHome++;
        updatedIndex = cards.size()-1;
    }
    reaarange(newRow, ac.first, updatedIndex); // update the order of indicies
    return updatedIndex; // return the new index of the previously active card
}

//--------------------------------------------------------------
int Deck::updatePosition(const pint & ac, const pint & np) {
    int newRow;
    switch(np.second) {
        case 0: // new pos is a card
            newRow = cards[np.first]->getRow() + 1; // the row card will be in
            // position of card underneath + spacing
            cards[ac.first]->setPosition(ofVec2f(cards[np.first]->getPosition().x, cards[np.first]->getPosition().y + SPACING));
            break;
        case 1: // new pos is a regular
            newRow = 0; // if a card is goint to the pile it's new row will be 0
            cards[ac.first]->setPosition(regs[np.first]->getPosition()); // pile's position
            break;
        case 2: // new pos is a free cell
            newRow = -10; // if a card is goint to the pile it's new row will be 0
            cards[ac.first]->setPosition(fcells[np.first]->getPosition()); // pile's position
            break;
        case 3: // new pos is a home
            newRow = -11; // if a card is goint to the pile it's new row will be 0
            cards[ac.first]->setPosition(homes[np.first]->getPosition()); // pile's position
            break;
    }
    return newRow; // return the value of the card's new row
}

//--------------------------------------------------------------
int Deck::assesSituation(const int &nr,const int & activeC) {
    int updatedIndex;
    tuple<bool,float, int> situation(false, ofGetWidth(), -1);
    for(int i=0; i< cards.size(); i++) {
        if(cards[i]->getRow() == nr) { // if the card will land in the row with other cards
            get<0>(situation) = true; // card will be in the same row as it previously was
            if ((cards[i]->getPosition().x > cards[activeC]->getPosition().x) && // if there is card to the right
                (cards[i]->getPosition().x < get<1>(situation))) { // find the card closest to the active card
                get<1>(situation) = cards[i]->getPosition().x; // save its position
                get<2>(situation) = i; // and its index
            }
        }
    }
    if(!get<0>(situation)) updatedIndex = findUpdatedIndex(activeC, nr - 1); // the card won't be moving to the same row
    else { // there were some cards
        if (get<2>(situation) == -1) updatedIndex = findUpdatedIndex(activeC, nr); // but only on the left
        else { // there were some cards on the right
            if (get<2>(situation) > activeC) updatedIndex = get<2>(situation) - 1; // if card moves to the right - place before card to the right
            else updatedIndex = get<2>(situation); // replace card on the right
        }
    }
    return updatedIndex;
}

//--------------------------------------------------------------
int Deck::findUpdatedIndex(const int& active, const int& row) {
    int idx;
    int hi = findHighestIndex(row); // find highest index of the given row
    if(cards[active]->getRow() > row || // if active row is bigger than previous row
       cards[active]->getRow() == -11 || // but it's not in the freecell
       cards[active]->getRow() == -10) idx = hi + 1; // and not in the home place after the hi
    else idx = hi; // place at hi
    return idx;
}

//--------------------------------------------------------------
int Deck::findHighestIndex(const int& row) {
    int hi = -1; // initialise new index
    for(int i = 0; i < cards.size(); i++) if (cards[i]->getRow() == row) if (hi < i) hi = i; // find the highest index
    return hi; // and return it
}

//--------------------------------------------------------------
void Deck::reaarange(const int & row, const int & indexToMove, const int & newIndex) {
    shared_ptr<Card> tmp(cards[indexToMove]); // create temporary card
    cards.erase(cards.begin() + indexToMove); // delete from the vector
    tmp->setRow(row); // update card's row information
    cards.insert(cards.begin()+newIndex, tmp); // insert at new index
}

//--------------------------------------------------------------
void Deck::newActiveHistory(const int & newA, const int & target) {
    if(!un){
        switch(target) {
                // new pos is a free cell - save index of the last card that is not yet home
            case 2: history[history.size()-1][0] = cards.size() - 1 - cAtHome; break;
                // new pos is a home - save last index to history
            case 3: history[history.size()-1][0] = cards.size() - 1; break;
                // new pos is a card or a regular - save the index of the new active minus eventual adjustement
            default: history[history.size()-1][0] = newA - history[history.size()-1][3]; break;
        }
    }
}

//--------------------------------------------------------------
void Deck::moveCardsOnTop(const int & onTop, int & newA) {
    for(int i = 0; i < onTop; i++) { // for each of them
        pint active(52,0);
        pint newP(newA,0);
        for(int j = 0; j < cards.size(); j++) { // look for the cards on top
            // find one with the smallest index and make it activeCard
            if(cards[j]->getOnTop() && j < active.first) active.first = j;
        }
        newA = updateLocation(active, newP); // find the new index of the active card
        cards[newA]->setOnTop(false); // deactivate card (no longer active as on top)
    }
}

//--------------------------------------------------------------
void Deck::checkAutocomplete() {
    int check = 0;
    for(int i = 0; i < regs.size(); i++) {
        int idx = -1;
        for(int j = 0; j < cards.size(); j++) {
            if(cards[j]->getPosition().x == regs[i]->getPosition().x &&
               cards[j]->getPosition().y == regs[i]->getPosition().y) {
                idx = j; // find cards on top of regs
            }
        }
        if (idx == -1 || autocompleteColumn(idx)) check++; // if the reg empty or all cards in the column are ok
    }
    // if the amount of good columns is same as amount of columns it's reafy for autocomplete
    if (check == regs.size()) autocomplete = true;
}

//--------------------------------------------------------------
bool Deck::autocompleteColumn(int idx) {
    int onTop = 0;
    for(int j = 0; j < cards.size(); j++) {
        if(cards[j]->getPosition().x == cards[idx]->getPosition().x && j > idx) onTop++; // count cards on top of it
    }
    for(int j = 0; j < onTop; j++) { // for all cards on top of the reg
        if(idx != -1) idx = autocompleteCard(idx); // check if the cards are sorted from the highest rank to the smaller one
        else break; // if not stop the loop
    }
    if (idx!=-1) return true; // column is ok
    else return false; // column is not ok
}

//--------------------------------------------------------------
int Deck::autocompleteCard(const int & idx) {
    int index = -1;
    for(int j = 0; j < cards.size(); j++) {
        if(cards[j]->getPosition().x == cards[idx]->getPosition().x && // find the card on top
           cards[j]->getPosition().y == cards[idx]->getPosition().y + SPACING &&
           cards[j]->getRank() <= cards[idx]->getRank()) { // chack if it has a smaller rank
            index = j;
        }
    }
    return index;
}

//--------------------------------------------------------------
void Deck::checkFinished() {
    int idx = -1; // check if all the cards are home
    for(int i = 0; i<cards.size(); i++) {
        if(cards[i]->getRow() != -11) {
            idx = i;
            break;
        }
    }
    if (idx == -1) finished = true; // if so set finished
}

//--------------------------------------------------------------
void Deck::deactivate(const int &ac) {
    cards[ac]->setActive(0); // deactivate active card
    for(int i = 0; i < cards.size(); i++) if(cards[i]->getOnTop()) {
        cards[i]->setOnTop(false); // deactivate cards on top of it
    }
}

//--------------------------------------------------------------
void Deck::undo() {
    if(act) deactivateAllCards(); // cancel card's activation
    if(history.size() > 0){ // if there is anything to undo
        score -= 5; // udno penalty
        un = true; // undo is in action
        activateCard(history[history.size()-1][0]); // activate previously moved card
        pint active = findActive(); // assign it's index and count cards on top of it
        // mark the location active used to be on top of before the move we want to undo as new pos, and assign it a type of location
        pint np(history[history.size()-1][1], history[history.size()-1][2]);
        // if the card is coming back from home - undo home state
        if (cards[active.first]->getRow() == -11) undoHome(active.first); // if undoing from home
        moveCard(active, np); // move the cards
        history.pop_back(); // delete this entry from history
        un = false; // undo is now done
    }
}

//--------------------------------------------------------------
void Deck::deactivateAllCards() {
    act = false; // deactivate state in case undo was pressed while there was an active card
    for(int i = 0; i<cards.size(); i++) if(cards[i]->getActive()) cards[i]->setActive(0); // deactivate any cards just in case
    for(int i = 0; i<cards.size(); i++) if(cards[i]->getActive()) cards[i]->setActive(0); // deactivate any cards just in case
}

//--------------------------------------------------------------
void Deck::undoHome(const int & idx) {
    score -= 10; // undo score for home
    cAtHome--; // decrease the amount of cards at home
    for(int i = 0; i< homes.size(); i++) {
        if(homes[i]->getPosition().x == cards[idx]->getPosition().x){
            if(cards[idx]->getRank() == 0) homes[i]->setSuit(-1); // if the card was ace unasign the suit fron the home cell
            homes[i]->setCRank(cards[idx]->getRank()); // make it expect a lower rank
        }
    }
}

//--------------------------------------------------------------

void Deck::hint() {
    if(act) deactivateAllCards(); // cancel card's activation
    // check for hints in that order:
    bool ok = (checkHint(0,3) || // card to home
               checkHint(1,3) || // fc to home
               checkHint(1,0) || // fc to card
               checkHint(0,0) || // card to card
               checkHint(0,1) || // card to reg
               checkHint(1,1) || // fc to reg
               checkHint(0,2)); // card to fc
    if(!ok) noMore = true; // if none of those are true there is no more possible moves
    hin = ok; // if any is true show the hint
}

//--------------------------------------------------------------
bool Deck::checkHint(const bool fc, const int target) {
    cout<<"CHECK"<<endl;
    bool match = false;
    int howManyTimes = (fc) ? fcells.size() : regs.size(); // check for up to as many times as either columns or freecells
    for(int i = 0; i < howManyTimes; i++) {
        // find a card to find a possible move for
        int idx = (fc) ? hintFindIdx(1, i, target) : hintFindIdx(0, i, target);
        if(idx != -1) { // if found
            bool condition;
            switch(target) { // find possible match
                case 0: condition = hintFindTarget(cards, idx, 0); break;
                case 1: condition = hintFindTarget(regs, idx, 1); break;
                case 2: condition = hintFindTarget(fcells, idx, 2); break;
                case 3: condition = hintFindTarget(homes, idx, 3); break;
            }
            if(condition) { // if the match was found
                match = true; // there is a possible move
                break; // stop the loop
            }
        }
    }
    return match; // return possible move or lack of thereof
}

//--------------------------------------------------------------
int Deck::hintFindIdx(const bool & fc, const int & sourceIdx, const int & target) {
    int idx = -1;
    if (fc) { // going from fc
        for(int i = 0; i < cards.size(); i++) { // if card is on top of fc
            if(cards[i]->getPosition() == fcells[sourceIdx]->getPosition()) idx = i;
        }
    } else { // going from cards
        if(regs[sourceIdx]->getOnTop()) { // only for columns that have cards in it
            if(target == 0 || target == 1) { // going to other card or reg
                idx = 52;
                for(int i = 0; i < cards.size(); i++) { // find lowest interactive index
                    if(cards[i]->getInteractive() &&
                       cards[i]->getPosition().x == regs[sourceIdx]->getPosition().x &&
                       i < idx) idx = i;
                }
            } else { // going home or fc
                for(int i = 0; i < cards.size(); i++) { // only find top cards
                    if(cards[i]->getTop() && cards[i]->getPosition().x == regs[sourceIdx]->getPosition().x) idx = i;
                }
            }
        }
    }
    return idx;
}

//--------------------------------------------------------------
template <class Piles>
bool Deck::hintFindTarget(const pil<Piles> & vec, const int & idx, const int & target) {
    pint cidx(idx,0); // this is needed for another card and enough space functions
    if (target == 0 || target == 1) { // if target is a card or a reg
        for(int j = 0; j < cards.size(); j++) {
            if(cards[j]->getPosition().x == cards[idx]->getPosition().x && j > idx) cidx.second++; // count cards on top
        }
    }
    bool ok = false;
    for(int j = 0; j < vec.size(); j++) { // for the length of target vector
        bool condition;
        switch (target) {
                // moveing to the top card that is not home or freeCell only if suits and colours match and enougch space
            case 0: condition = cards[j]->getTop() &&
                cards[j]->getRow() != -10 && cards[j]->getRow() != -11 && anotherCard(j, cidx); enough = true; break;
                // moveing to the reg if it's empty and enough space
            case 1: condition = !regs[j]->getOnTop() && enoughSpace(cidx.second, 1); enough = true; break;
                // moveing to the fcell if it's empty
            case 2: condition = !fcells[j]->getOnTop(); break;
                // moveing to the home cell if rhe rank is ok and suit is either empty or ok
            case 3: condition = homes[j]->getCRank() == cards[idx]->getRank() &&
                (homes[j]->getSuit() == -1 || homes[j]->getSuit() == cards[idx]->getSuit()); break;
        }
        if(condition) {
            setHint(idx, j, target); // set hintcard and target
            ok = true; // match is found for the column
            break;
        }
    }
    return ok; // return match
}

//--------------------------------------------------------------
void Deck::setHint(const int & idx, const int & targetIdx, const int & target) {
    cards[idx]->setHint(1); // set hint card
    hintPos = (cards[0]->getSize()/2); // center the hint arrow on the target
    switch (target) { // choose target
        case 3: { // home
            // if home is empty set tarhet to home
            if(!homes[targetIdx]->getOnTop()) hintPos += homes[targetIdx]->getPosition();
            else {
                int i = -1;
                for(int j = 0; j < cards.size(); j++) // find the topmost card on top of the home
                    if(cards[j]->getPosition() == homes[targetIdx]->getPosition() && i > j) i = j;
                hintPos += cards[i]->getPosition(); // set hint target to that card
            }
            break;
        }
        case 2: { hintPos += fcells[targetIdx]->getPosition(); break; } // set fcell as target
        default: { // cards and regs
            // if cards get card pos as taget if reg get regs pos as target
            hintPos += (target == 0) ? cards[targetIdx]->getPosition() : regs[targetIdx]->getPosition();
            // set all cards on top of the hint as hint cards as well
            for(int j = 0; j < cards.size(); j++) if(cards[j]->getPosition().x == cards[idx]->getPosition().x && j > idx) cards[j]->setHint(1);
            break;
        }
    }
}

//--------------------------------------------------------------
void Deck::skipAutocomplete() {
    dontAutocomplete = false;
    autocomplete = false;
}

//--------------------------------------------------------------
void Deck::doAutocomplete() {
    int cardsInGame;
    for(int i = 0; i < cards.size(); i++) if(cards[i]->getRow() != -11) cardsInGame++; // count cards still in the game
    for(int i = 0; i < cardsInGame; i++) {
        int idx = autoFindIdx(i); // find smallest card
            if(idx!= 52) {
                pint ac(idx, 0);
                for(int j = 0; j < homes.size(); j++) {
                if(checkHomes(ac,j)) { // find a home to move to and if found
                    pint home(j, 3);
                    moveCard(ac, home); // move to that home
                    moves++; // count moves
                    break; // stop checking for other freecells we already got what we needed
                }
            }
        }
    }
    dontAutocomplete = false; // stop checking for autocomplete after every move
    autocomplete = false; // disable autocomplete
    finished = true; // enable finished tab
}

//--------------------------------------------------------------
int Deck::autoFindIdx(const int &i) {
    int idx = 52;
    int rank = 13;
    for(int j = 0; j < cards.size(); j++) {
        // card is not at home find the smallest card in the deck
        if(cards[j]->getRow() != -11 && rank > cards[j]->getRank()) {
            if(cards[j]->getTop() || cards[j]->getRow() == -10) { // if the card is in free cell or on top of the column
                idx = j;
                rank = cards[j]->getRank();
            }
        }
    }
    return idx;
}

//--------------------------------------------------------------
bool Deck::getFinished() {
    return finished;
}

//--------------------------------------------------------------
int Deck::getMoves() {
    return moves;
}

//--------------------------------------------------------------
int Deck::getScore() {
    return score;
}

//--------------------------------------------------------------
string Deck::getTime() {
    return time;
}

//--------------------------------------------------------------
int Deck::getSeconds() {
    return sec;
}
