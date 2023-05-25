#pragma once
#include <stdexcept>
#include <string>
#include <json\json.h>
#include <fstream>
#include <msclr/marshal_cppstd.h>


using namespace System;

ref class Card{
private:

	String^ cardNumber;
	String^ holder;
	String^ phoneNumber;
	
	unsigned int pinCode;

	double balance;

	void checkExceptions();

public:
	Card(String^ _cardNumber, String^ _holder, int pinCode, String^ phoneNumber, double money);
	//Card(String^ _cardNumber, String^ _holder, int _CVV, int _pinCode, int _expirationMonth, int _expirationYear);

	void credit(int amount);
	bool debit(int amount);
	bool canDebit(int amount);
	double getBalance() { return this->balance; }
	String^ getCardNumber() { return this->cardNumber; }
	String^ getPhoneNumber() { return this->phoneNumber; }
	int getPinCode() { return this->pinCode; }
	void setPinCode(int newPin);
	
};