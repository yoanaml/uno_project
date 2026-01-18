// uno_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Yoana Mladenova fn4MI0600631

#include <iostream>
#include <algorithm> 
#include <random>    
#include <ctime> 
#include <fstream>
using namespace std;


const int MAX_CARDS = 108;
const int MAX_CARD_LENGTH = 10;
const int COLORS_COUNT = 4;
const int NUMBERS_COUNT = 10;
const int INITIAL_DECK_SIZE = 108;
const int WILD_CARDS_COUNT = 4;
const int MIN_PLAYERS = 2;
const int MAX_PLAYERS = 4;
const int COLOR_INDEX = 0;
const int TYPE_INDEX = 1;

void saveGame(int currentPlayer, int direction);
bool loadGame(int& currentPlayer, int& direction);


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


	while (currentColor != 'R' && currentColor != 'G' &&
		currentColor != 'B' && currentColor != 'Y') {
		cout << "Invalid color! Choose (R/G/B/Y): ";
		cin >> currentColor;
	}
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

	while (currentColor != 'R' && currentColor != 'G' &&
		currentColor != 'B' && currentColor != 'Y') {
		cout << "Invalid color! Choose (R/G/B/Y): ";
		cin >> currentColor;
	}
}

void printPlayerCards(int player)

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

void playCard(int player, int cardIndex) {

	strcp(currentCard, players[player][cardIndex]);
	strcp(discardPile[discardSize], currentCard);
	discardSize++;

	for (int i = cardIndex; i < cardsCount[player] - 1; i++) {
		strcp(players[player][i], players[player][i + 1]);
	}

	cardsCount[player]--;
}

bool drawCardAndPlayOption(int player) {
	drawCard(player);

	int drawnIndex = cardsCount[player] - 1;


	if (isValidMove(players[player][drawnIndex], currentCard, currentColor)) {
		char choice;
		cout << "You can play this card. Play it now? (y/n): ";
		cin >> choice;

		if (choice == 'y' || choice == 'Y') {

			strcp(currentCard, players[player][drawnIndex]);
			strcp(discardPile[discardSize], currentCard);
			discardSize++;


			cardsCount[player]--;

			return true;
		}
	}

	return false;
}

int getNextPlayer(int currentPlayer, int direction, int playersCount, int skipCount = 0) {
	int next = currentPlayer;
	for (int i = 0; i <= skipCount; i++) {
		next = (next + direction + playersCount) % playersCount;
	}
	return next;
}


void checkUNO(int player)
{
	if (cardsCount[player] != 1) {
		return;
	}

	char unoCall[4];
	cout << "Player " << player + 1 << ", type 'UNO' now: ";
	cin >> unoCall;

	if (streq(unoCall, "UNO") || streq(unoCall, "uno")) {
		cout << "UNO called successfully!" << endl;
		return;
	}

	cout << "You forgot to call UNO! Drawing 1 card as penalty." << endl;
	drawCard(player);
}

bool hasValidMove(int player)
{
	for (int i = 0; i < cardsCount[player]; i++) {
		if (isValidMove(players[player][i], currentCard, currentColor)) {
			return true;
		}
	}
	return false;
}

int applySpecialCardEffect(int playedBy, int& direction, int playersCount, char& currentColor)
{
	int skipCount = 0;

	if (isPlus2(currentCard)) {
		actionPlusTwo(playedBy, direction, playersCount);
		skipCount = 1;
	}
	else if (isReverse(currentCard)) {
		actionReverse(direction);
		if (playersCount == 2) {
			skipCount = 1;
		}
	}
	else if (isSkip(currentCard)) {
		skipCount = 1;
	}
	else if (isWildPlusFour(currentCard)) {
		actionWildPlusFour(playedBy, direction, playersCount, currentColor);
		skipCount = 1;
	}
	else if (isWild(currentCard)) {
		actionWild(currentColor);
	}

	return skipCount;
}

bool handlePlayerChoice(int player, bool& cardPlayed, int currentPlayer, int direction, bool& wantsToQuit)
{
	int chosenCard;
	cout << "Choose index of card to play (or -1 to save and quit): ";
	cin >> chosenCard;

	if (chosenCard == -1) {
		saveGame(currentPlayer, direction);
		cout << "Game saved! You can continue later from the main menu." << endl;
		wantsToQuit = true;
		return true;
	}

	if (chosenCard >= 0 && chosenCard < cardsCount[player]) {
		if (isValidMove(players[player][chosenCard], currentCard, currentColor)) {
			playCard(player, chosenCard);
			cardPlayed = true;
			return true;
		}
		else {
			cout << "Invalid move! Try again.\n";
			return false;
		}
	}
	else {
		cout << "Invalid index! Try again.\n";
		return false;
	}
}

void processCardEffects(int playedBy, int& direction, int& skipCount)
{

	if (!isWild(currentCard) && !isWildPlusFour(currentCard))
	{
		currentColor = '\0';
	}

	skipCount = applySpecialCardEffect(playedBy, direction, playersCount, currentColor);
}

bool checkForWinner(int player)
{
	if (cardsCount[player] == 0) {
		cout << "Player " << player + 1 << " wins!" << endl;
		cout << "Thanks for playing!";
		return true;
	}
	return false;
}

void saveGame(int currentPlayer, int direction)
{
	ofstream file("uno_save.txt");

	if (!file.is_open()) {
		cout << "Error: Could not save game!" << endl;
		return;
	}

	file << playersCount << endl;
	file << currentPlayer << endl;
	file << direction << endl;
	file << currentColor << endl;
	file << currentCard << endl;

	file << deckSize << endl;
	for (int i = 0; i < deckSize; i++) {
		file << deck[i] << endl;
	}

	file << discardSize << endl;
	for (int i = 0; i < discardSize; i++) {
		file << discardPile[i] << endl;
	}

	for (int p = 0; p < playersCount; p++) {
		file << cardsCount[p] << endl;
		for (int c = 0; c < cardsCount[p]; c++) {
			file << players[p][c] << endl;
		}
	}

	file.close();
	cout << "\n Game saved successfully! " << endl;
}
bool loadGame(int& currentPlayer, int& direction)
{
	ifstream file("uno_save.txt");

	if (!file.is_open()) {
		return false;
	}

	file >> playersCount;
	file >> currentPlayer;
	file >> direction;
	file >> currentColor;
	file.ignore();
	file.getline(currentCard, MAX_CARD_LENGTH);

	file >> deckSize;
	file.ignore();
	for (int i = 0; i < deckSize; i++) {
		file.getline(deck[i], MAX_CARD_LENGTH);
	}

	file >> discardSize;
	file.ignore();
	for (int i = 0; i < discardSize; i++) {
		file.getline(discardPile[i], MAX_CARD_LENGTH);
	}

	for (int p = 0; p < playersCount; p++) {
		file >> cardsCount[p];
		file.ignore();
		for (int c = 0; c < cardsCount[p]; c++) {
			file.getline(players[p][c], MAX_CARD_LENGTH);
		}
	}

	file.close();
	cout << "\n  Game loaded successfully! " << endl;
	return true;
}


void playTurn(int& currentPlayer, int& direction, bool& gameOver)
{
	int playedBy = currentPlayer;
	int skipCount = 0;

	if (isDeckEmpty()) {
		refillDeckFromDiscard();
	}

	printCurrentCard(currentCard);
	printPlayerCards(currentPlayer);

	bool cardPlayed = false;
	bool wantsToQuit = false;

	if (!hasValidMove(currentPlayer)) {
		cout << "No valid moves. Drawing a card..." << endl;

		char saveChoice;
		cout << "Do you want to save before drawing? (y/n): ";
		cin >> saveChoice;

		if (saveChoice == 'y' || saveChoice == 'Y') {
			saveGame(currentPlayer, direction);
			cout << "Game saved! Continue playing? (y/n): ";
			char continueChoice;
			cin >> continueChoice;

			if (continueChoice == 'n' || continueChoice == 'N') {
				gameOver = true;
				return;
			}
		}

		cardPlayed = drawCardAndPlayOption(currentPlayer);
	}
	else {
		bool valid = false;
		while (!valid && !wantsToQuit) {
			valid = handlePlayerChoice(currentPlayer, cardPlayed, playedBy, direction, wantsToQuit);
		}

		if (wantsToQuit) {
			gameOver = true;
			return;
		}
	}

	if (!cardPlayed) {
		currentPlayer = getNextPlayer(playedBy, direction, playersCount, skipCount);
		return;
	}

	processCardEffects(playedBy, direction, skipCount);
	checkUNO(playedBy);

	if (checkForWinner(playedBy)) {
		gameOver = true;
		return;
	}

	currentPlayer = getNextPlayer(playedBy, direction, playersCount, skipCount);
}

void runGame()
{
	int currentPlayer = 0;
	int direction = 1;
	bool gameOver = false;

	while (!gameOver) {
		playTurn(currentPlayer, direction, gameOver);
	}
}


void displayMainMenu()
{
	cout << "\n================================" << endl;
	cout << "          UNO GAME" << endl;
	cout << "================================" << endl;
	cout << "1. New Game" << endl;
	cout << "2. Continue Saved Game" << endl;
	cout << "3. Exit" << endl;
	cout << "================================" << endl;
	cout << "Choose option: ";
}


int main()
{
	while (true) {
		displayMainMenu();

		int choice;
		cin >> choice;

		if (choice == 1) {
			cout << "\n--- Starting New Game ---\n" << endl;
			cout << "Let's start the new UNO adventure! It will be fun, I promise\n";
			setupGame();
			runGame();
		}
		else if (choice == 2) {
			int currentPlayer = 0;
			int direction = 1;

			if (loadGame(currentPlayer, direction)) {
				cout << "\n--- Resuming Saved Game ---\n" << endl;
				cout << "\nGood luck!Have fun!\n";
				cout << "It's Player " << currentPlayer + 1 << "'s turn\n" << endl;

				bool gameOver = false;
				while (!gameOver) {
					playTurn(currentPlayer, direction, gameOver);
				}
			}
			else {
				cout << "\nOops! No saved game found. Let's start a new adventure!\n" << endl;
			}
		}
		else if (choice == 3) {
			cout << "\nThanks for playing UNO! Hope to see you again soon! Goodbye!\n" << endl;
			break;
		}
		else {
			cout << "\nInvalid choice! Don't worry, just pick 1, 2, or 3 and let's play!\n" << endl;
		}
	}

	return 0;
}


