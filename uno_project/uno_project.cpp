// uno_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Yoana Mladenova fn4MI0600631

#include <iostream>
#include <algorithm> 
#include <random>    
#include <ctime> 
using namespace std;


const int MAX_CARDS = 108;
const int MAX_CARD_LENGTH = 10;
const int COLORS_COUNT = 4;
const int NUMBERS_COUNT = 10;
const int INITIAL_DECK_SIZE = 108;
const int WILD_CARDS_COUNT = 4;
const int MAX_CARD_LENGTH = 10;

char deck[MAX_CARDS][MAX_CARD_LENGTH];
int deckSize = INITIAL_DECK_SIZE;

char discardPile[MAX_CARDS][MAX_CARD_LENGTH];
int discardSize = 0;

int streq(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i])
            return 0;
        i++;
    }
    return a[i] == b[i];
}

void strcp(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void strconcat(char* dest, const char* src) {
    int i = 0;
    while (dest[i] != '\0') {
        i++;
    }
    int j = 0;
    while (src[j] != '\0') {
        dest[i] = src[j];
        i++;
        j++;
    }

    dest[i] = '\0';
}

void createDeck() {
    int k = 0;
    char colors[COLORS_COUNT] = { 'R','G','B','Y' };

   
    for (int c = 0; c < COLORS_COUNT; c++) {

        deck[k][0] = colors[c];
        deck[k][1] = '0';
        deck[k][2] = '\0';
        k++;

        for (int n = 1; n < NUMBERS_COUNT; n++) {
            deck[k][0] = colors[c];
            deck[k][1] = '0' + n;
            deck[k][2] = '\0';
            k++;

            deck[k][0] = colors[c];
            deck[k][1] = '0' + n;
            deck[k][2] = '\0';
            k++;
        }
    }

    
    for (int c = 0; c < COLORS_COUNT; c++) {
        char temp[MAX_CARD_LENGTH];

        temp[0] = colors[c];
        temp[1] = '\0';
        strconcat(temp, "+2");
        strcp(deck[k++], temp);
        strcp(deck[k++], temp);

        temp[0] = colors[c];
        temp[1] = '\0';
        strconcat(temp, "Skip");
        strcp(deck[k++], temp);
        strcp(deck[k++], temp);

        temp[0] = colors[c];
        temp[1] = '\0';
        strconcat(temp, "Reverse");
        strcp(deck[k++], temp);
        strcp(deck[k++], temp);
    }

    
    for (int i = 0; i < WILD_CARDS_COUNT; i++) {
        strcp(deck[k++], "Wild");
        strcp(deck[k++], "Wild+4");
    }
}

void shuffleDeck()
{
    random_device rd;   
    mt19937 g(rd());    

    
    for (int i = deckSize - 1; i > 0; i--) {
        uniform_int_distribution<int> dist(0, i);
        int j = dist(g);

        
        char temp[MAX_CARD_LENGTH];
        strcp(temp, deck[i]);
        strcp(deck[i], deck[j]);
        strcp(deck[j], temp);
    }
}


int isPlus2(const char* card) {
    return card[1] == '+' && card[2] == '2';
}
int isReverse(const char* card) {
    return card[1] == 'R'; // Reverse
}
int isWild(const char* card)
{
    return streq(card, "Wild");
}
int isWildPlusFour(const char* card)
{
    return streq(card, "Wild+4");
}
int isSkip(const char* card)
{
    return card[1] == 'S';
}


char currentCard[MAX_CARD_LENGTH];

void drawStartingCard()
{
    while (true) {
        strcp(currentCard, deck[deckSize - 1]);
        deckSize--;

        
        if (isWild(currentCard) || isWildPlusFour(currentCard) ||
            isPlus2(currentCard) || isReverse(currentCard) || isSkip(currentCard)) {
            strcp(deck[deckSize], currentCard);
            deckSize++;
            shuffleDeck();
        }
        else {
            
            strcp(discardPile[discardSize], currentCard);
            discardSize++;
            break;
        }
    }
}

const int COLOR_INDEX = 0;
const int TYPE_INDEX = 1;

bool isValidMove(const char* card, const char* topCard, char currentColor)
{
   
    if (card[COLOR_INDEX] == 'W')
        return true;

    
    if (currentColor != '\0') {
        return card[COLOR_INDEX] == currentColor;
    }

   
    if (card[COLOR_INDEX] == topCard[COLOR_INDEX])
        return true;

    
    if (card[TYPE_INDEX] >= '0' && card[TYPE_INDEX] <= '9' &&
        topCard[TYPE_INDEX] >= '0' && topCard[TYPE_INDEX] <= '9' &&
        card[TYPE_INDEX] == topCard[TYPE_INDEX])
        return true;

   
    if (card[TYPE_INDEX] == '+' && topCard[TYPE_INDEX] == '+')
        return true;

    
    if (card[TYPE_INDEX] == 'R' && topCard[TYPE_INDEX] == 'R')
        return true;

    
    if (card[TYPE_INDEX] == 'S' && topCard[TYPE_INDEX] == 'S')
        return true;

    return false;
}

const int MAX_PLAYERS = 4;
const int COUNT_OF_CARDS_PER_PLAYER = 7;
char players[MAX_PLAYERS][MAX_CARDS][MAX_CARD_LENGTH]; 
int cardsCount[MAX_PLAYERS];                           
int playersCount;

void dealCards(int playersCount)
{
    
    for (int p = 0; p < playersCount; p++) {
        cardsCount[p] = 0;
    }

    for (int i = 0; i < COUNT_OF_CARDS_PER_PLAYER; i++) {
        for (int p = 0; p < playersCount; p++) {
            
            strcp(players[p][cardsCount[p]], deck[deckSize - 1]);
            cardsCount[p]++;

            deckSize--; 
        }
    }
}

int nextPlayerIndex(int currentPlayer, int direction, int playersCount)
{
    return (currentPlayer + direction + playersCount) % playersCount;
}

void actionReverse(int& direction)
{
    direction *= -1;
}
void actionSkip(int currentPlayer, int direction, int playersCount, int& nextPlayer)
{
    nextPlayer = nextPlayerIndex(currentPlayer, direction, playersCount);
}
void actionPlusTwo(int currentPlayer, int direction, int playersCount)
{
    int next = nextPlayerIndex(currentPlayer, direction, playersCount);

   
    for (int i = 0; i < 2; i++) {
        strcp(players[next][cardsCount[next]], deck[deckSize - 1]);
        cardsCount[next]++;
        deckSize--;
    }


}

char currentColor = '\0';

void actionWild(char& currentColor)
{
    cout << "Choose color (R/G/B/Y): ";
    cin >> currentColor;
}
void actionWildPlusFour(int currentPlayer, int direction, int playersCount, char& currentColor)
{
    int next = (currentPlayer + direction + playersCount) % playersCount;

   
    for (int i = 0; i < 4; i++) {
        strcp(players[next][cardsCount[next]], deck[deckSize - 1]);
        cardsCount[next]++;
        deckSize--;
    }

    cout << "Choose color (R/G/B/Y): ";
    cin >> currentColor;
}

void printPlayerCards(int player) //играча на позиция  0,1,2,3

{
    cout << "\nPlayer " << player + 1 << " - Your cards:\n";
    for (int i = 0; i < cardsCount[player]; i++) {
        cout << "[" << i << "] " << players[player][i] << " ";
    }
    cout << endl;
}
void printCurrentCard(char* currentCard)
{
    cout << "\n---UNO---";
    cout << "\nCurrent card: " << currentCard << endl;

    if (currentColor != '\0') {
        cout << "Current color (from Wild): " << currentColor << endl;
    }
}

const int MIN_PLAYERS = 2;
const int MAX_PLAYERS = 4;

void readPlayersCount()
{
    do {
        cout << "Enter number of players ("
            << MIN_PLAYERS << "-" << MAX_PLAYERS << "): ";
        cin >> playersCount;

        if (playersCount < MIN_PLAYERS || playersCount > MAX_PLAYERS) {
            cout << "Invalid number of players. Try again.\n";
        }

    } while (playersCount < MIN_PLAYERS || playersCount > MAX_PLAYERS);
}
void setupGame()
{
    readPlayersCount();     
    createDeck();           
    shuffleDeck();          
    dealCards(playersCount);
    drawStartingCard();     
}


bool isDeckEmpty() {
    return deckSize == 0;
}

void refillDeckFromDiscard() {
    cout << "Deck empty! Shuffling discard pile into deck..." << endl;

    
    deckSize = discardSize - 1;
    for (int i = 0; i < deckSize; i++) {
        strcp(deck[i], discardPile[i]);
    }

   
    discardSize = 1;
    strcp(discardPile[0], currentCard);

    shuffleDeck();
}

void drawCard(int player) {
    if (deckSize == 0) {
        refillDeckFromDiscard();
    }

    strcp(players[player][cardsCount[player]], deck[deckSize - 1]);
    cardsCount[player]++;
    deckSize--;

    cout << "Drawn card: " << players[player][cardsCount[player] - 1] << endl;
}


int main()
{
   
}


