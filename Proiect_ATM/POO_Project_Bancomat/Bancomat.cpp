#include "Bancomat.h"

using namespace std;
using namespace System;
using namespace msclr;

Bancomat::Bancomat()
{
	storage = gcnew Storage;
}

Bancomat::~Bancomat() {
	delete storage;
}

bool Bancomat::introduceCard(Card^ _card, int pinCode) {
	if (_card->getPinCode() == pinCode) {
		this->card = _card;
		return true;
	}

	return false;
}

void Bancomat::resetPin(int newPin) {
	card->setPinCode(newPin);

	writeCard(this->card);
}

bool Bancomat::canDebit(int amount, String^ base) {
	int exchangedAmount = exchangeMoney(base, "RON", amount);

	if (!card->canDebit(exchangedAmount)) {
		Console::WriteLine("Dvs aveti insuficient sold.");
		return false;
	}

	if (!storage->canWithdraw(amount, base)) return false;

	return true;
}

bool Bancomat::debit(int amount, String^ base) {
	if (!canDebit(amount, base)) return false;

	int exchangedAmount = exchangeMoney(base, "RON", amount);

	storage->withdraw(amount, base);

	card->debit(exchangedAmount);

	writeCard(this->card);

	return true;
}

bool Bancomat::credit(int amount, String^ base) {
	if (!storage->existsBanknoteOf(amount, base)) return false;

	int exchangedAmount = exchangeMoney(base, "RON", amount);
	card->credit(exchangedAmount);

	storage->topUp(amount, base);

	writeCard(this->card);

	return true;
}

bool Bancomat::transfer(Card^ card2, int amount) {
	if (card->debit(amount)) {
		card2->credit(amount);

		writeCard(this->card);
		writeCard(card2);
		return true;
	}



	return false;
}

double Bancomat::getSold() {
	return card->getBalance();
}

void Bancomat::cardOut() {
	card = nullptr;
}

int Bancomat::exchangeMoney(String^ exFrom, String^ exTo, int amount) {

	// if is the same base
	if (exFrom == exTo) return amount;

	// getting request and response json files
	fstream jsonRequest("NodeJS/JSON/exchangeRequest.json", std::ofstream::out | std::ofstream::trunc); // deleting all data from file
	fstream jsonResponse("NodeJS/JSON/exchangeResponse.json", std::ofstream::out | std::ofstream::trunc); // deleting all data from file
	jsonResponse.close();

	Json::Value request;
	Json::Value response;
	Json::Reader reader;

	// creating request object
	request["from"]["base"] = interop::marshal_as<string>(exFrom);
	request["from"]["amount"] = amount;
	request["to"] = interop::marshal_as<string>(exTo);

	// writing request object in json request file
	jsonRequest << request;
	jsonRequest.close();

	// executing node js file that exchange money from API
	system("node.exe NodeJS/index.js");

	jsonResponse.open("NodeJS/JSON/exchangeResponse.json");

	// writing result in response
	reader.parse(jsonResponse, response);

	return response.asInt();
}

bool Bancomat::hasCard() {
	if (card) return true;

	return false;
}

Card^ Bancomat::getCard(String^ cardNumber) {

	if (!existsCardNumber(cardNumber)) return nullptr;

	ifstream input("bank_data.json");
	Json::Value root = new Json::Value();
	input >> root;

	Json::Value card;

	string card_nr = interop::marshal_as<string>(cardNumber);

	for (Json::Value::ArrayIndex i = 0; i < root.size(); i++) {
		string current_card_number = (root)[i]["card_number"].asString();

		if (current_card_number == card_nr) {
			card = (root)[i];
			break;
		}
	}

	string card_number = card["card_number"].asString();
	string PIN = card["PIN"].asString();
	string phone_number = card["phone_number"].asString();
	double money_count = card["money_count"].asDouble();
	string full_name = card["full_name"].asString();

	String^ card_number_1 = gcnew String(card_number.c_str());
	String^ phone_number_1 = gcnew String(phone_number.c_str());
	String^ full_name_1 = gcnew String(full_name.c_str());
	int PIN_1 = stoi(PIN);

	return gcnew Card(card_number_1, full_name_1, PIN_1, phone_number_1, money_count);
}

void Bancomat::writeCard(Card^ _card) {
	fstream cardsDB("bank_data.json");

	Json::Value root = new Json::Value();
	cardsDB >> root;

	string card_nr = interop::marshal_as<string>(_card->getCardNumber());

	for (Json::Value::ArrayIndex i = 0; i < root.size(); i++) {
		string current_card_number = (root)[i]["card_number"].asString();

		if (current_card_number == card_nr) {
			(root)[i]["PIN"] = to_string(_card->getPinCode());
			(root)[i]["money_count"] = _card->getBalance();

			break;
		}
	}

	// clearing data from file
	cardsDB.close();
	cardsDB.open("bank_data.json", std::ofstream::out | std::ofstream::trunc);

	cardsDB << root;
}

bool Bancomat::existsCardNumber(String^ card_Number)
{
	ifstream input("bank_data.json");
	Json::Value root;
	input >> root;

	string card_nr = interop::marshal_as<string>(card_Number);

	for (Json::Value::ArrayIndex i = 0; i < root.size(); i++) {
		string current_card_number = (root)[i]["card_number"].asString();
		if (current_card_number == card_nr) {
			return true;
		}
	}

	return false;
}