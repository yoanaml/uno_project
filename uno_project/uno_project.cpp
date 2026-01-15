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


int main()
{
   
}


