#include "Card.h"

using namespace System;
using namespace std;

void Card::checkExceptions() {
	if (cardNumber->Length != 12)
	{
		throw length_error("Lungimea numarului cardului trebuie sa fie 16. Lungimea dvs este: " + to_string(cardNumber->Length));
	}

	if (holder->Length > 50 || holder->Length < 5) {
		throw length_error("Lungimea numelui trebuie sa fie cuprinsa intre 5 si 50. Lungimea dvs este: " + to_string(holder->Length));
	}

	if (pinCode.ToString()->Length != 4) {
		throw length_error("Lungimea Pin Codului trebuie sa fie 4. Lungimea dvs este: " + to_string(pinCode.ToString()->Length));
	}
}

Card::Card(String^ _cardNumber, String^ _holder, int _pinCode, String^ _phoneNumber, double _money)
	:cardNumber(_cardNumber), holder(_holder), pinCode(_pinCode), balance(_money), phoneNumber(_phoneNumber)
{
	//checkExceptions();
}

void Card::credit(int amount) {
	balance += amount;
}

bool Card::debit(int amount) {

	if (canDebit(amount)) {
		balance -= amount;
		return true;
	}

	Console::WriteLine("Insuficient bani pe cont.");
	return false;
}

bool Card::canDebit(int amount) {
	return amount <= balance;
}

void Card::setPinCode(int newPin) {
	if (newPin.ToString()->Length != 4) {
		Console::WriteLine("Lungime gresita a pincodului");
	}
	else {
		pinCode = newPin;
	}
}