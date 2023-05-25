#pragma once
#include "pch.h"
#include "Card.h"
#include "Storage.h"

using namespace System;

ref class Bancomat {
private:
	unsigned int ID;
	double bancomatStorage;
	Storage^ storage;
	Card^ card;


public:
	Bancomat();
	~Bancomat();

	bool introduceCard(Card^ _card, int pinCode);
	void resetPin(int newPin);
	bool debit(int amount, String^ base);
	bool credit(int amount, String^ base);
	bool transfer(Card^ cont, int amount);
	bool canDebit(int amount, String^ base);
	double getSold();
	double getBancomatStorage() { return bancomatStorage; }
	bool checkPin(int pin) { return pin == card->getPinCode(); }
	void cardOut();
	static int exchangeMoney(String^ exFrom, String^ exTo, int amount);
	cliext::vector<String^> getBanknotesOf(String^ base) { return storage->getBanknotesOf(base); }
	bool hasCard();
	bool existsBanknoteOf(int banknote, String^ base) { return storage->existsBanknoteOf(banknote, base); }

	Card^ getCard(String^ cardNumber);
	void writeCard(Card^ _card);
	bool existsCardNumber(String^ card_Number);

};