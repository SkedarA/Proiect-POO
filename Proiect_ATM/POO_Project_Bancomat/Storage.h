#pragma once

#include <json\json.h>
#include <string>
#include <fstream>
#include <msclr/marshal_cppstd.h>
#include <cliext/vector>

using namespace System;

ref class Storage {
private:
	int id;

	String^ storageFileName;

	static int idIter;

	Json::Value getStorageObj();

public:
	Storage();

	bool withdraw(int amount, String^ base);
	bool topUp(int amount, String^ base);

	bool canWithdraw(int amount, String^ base);
	bool existsBanknoteOf(int banknote, String^ base);

	int getTotalAmountOf(String^ base);
	cliext::vector<String^> getBanknotesOf(String^ base);

};

