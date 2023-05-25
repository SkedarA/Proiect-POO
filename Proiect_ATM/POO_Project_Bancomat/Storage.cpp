#include "Storage.h"

using namespace std;
using namespace System;
using namespace msclr;

Storage::Storage() {
	idIter++;
	id = idIter;
	
	String^ _storageFileName = "cashStorages/cashStorage";
	_storageFileName += id.ToString();
	_storageFileName += ".json";

	storageFileName = _storageFileName;

	string storageStrName = interop::marshal_as<string>(_storageFileName);

	// opening cashStorage<id>.json
	fstream storage;
	storage.open(storageStrName);

	// if there is not such a file then create new
	if (storage.fail()) {
		storage.open(storageStrName, fstream::in | fstream::out | fstream::trunc);

		Json::Value storageObj;

		// filling storage with ron bancnotes
		Json::Value ron;

		ron["5"] = 200;
		ron["10"] = 150;
		ron["20"] = 110;
		ron["50"] = 80;
		ron["100"] = 60;
		ron["200"] = 40;
		ron["500"] = 20;

		storageObj["RON"] = ron;

		// filling storage with eur bancnotes
		Json::Value eur;

		eur["5"] = 110;
		eur["10"] = 90;
		eur["20"] = 70;
		eur["50"] = 40;
		eur["100"] = 30;
		eur["200"] = 20;
		eur["500"] = 10;

		storageObj["EUR"] = eur;

		// filling storage with usd bancnotes
		Json::Value usd;

		usd["5"] = 110;
		usd["10"] = 90;
		usd["20"] = 70;
		usd["50"] = 40;
		usd["100"] = 30;

		storageObj["USD"] = usd;

		// filling storage with gbp bancnotes
		Json::Value gbp;

		gbp["5"] = 110;
		gbp["10"] = 90;
		gbp["20"] = 70;
		gbp["50"] = 40;
		
		storageObj["GBP"] = gbp;

		// writing obj with bancnotes in file
		storage << storageObj;
	}

	storage.close();
}

int Storage::getTotalAmountOf(String^ base) {
	int totalAmount = 0;

	Json::Value storageObj = getStorageObj();

	string _base = interop::marshal_as<string>(base);

	cliext::vector<String^>banknotes = getBanknotesOf(base);

	for (int i = 0; i < banknotes.size(); i++) {
		string banknote = interop::marshal_as<string>(banknotes[i]);

		// if exists such banknote then add its amount and value to total amount
		if (storageObj[_base][banknote]) {
			totalAmount += stoi(banknote) * stoi(storageObj[_base][banknote].asString());
		}
	}

	return totalAmount;
}

Json::Value Storage::getStorageObj() {
	String^ _storageFileName = storageFileName;

	fstream storage(interop::marshal_as<string>(_storageFileName));

	if (storage.fail())
		Console::WriteLine("File is not open");

	Json::Value storageObj;
	Json::Reader reader;

	reader.parse(storage, storageObj);

	storage.close();

	return storageObj;
}

bool Storage::withdraw(int amount, String^ base) {
	// if there is not enough amount of cash in storage
	if (!canWithdraw(amount, base)) return false;

	Json::Value storageObj = getStorageObj();

	string _base = interop::marshal_as<string>(base);

	String^ _storageFileName = storageFileName;
	fstream storage;
	storage.open(interop::marshal_as<string>(_storageFileName));

	cliext::vector<String^> banknotes = getBanknotesOf(base);

	for (int i = banknotes.size() - 1; i >= 0;) {
		int banknote = Int32::Parse(banknotes[i]);
		int banknotesAmount = storageObj[_base][to_string(banknote)].asInt();

		if (amount >= banknote && banknotesAmount > 0) {

			storageObj[_base][to_string(banknote)] = banknotesAmount - 1;

			amount -= banknote;
		}
		else {
			i--;
		}
	}

	storage << storageObj;

	storage.close();

	return true;
}

bool Storage::topUp(int banknote, String^ base) {
	if (!existsBanknoteOf(banknote, base)) return false;

	String^ _storageFileName = storageFileName;
	fstream storage;
	storage.open(interop::marshal_as<string>(_storageFileName));

	Json::Value storageObj;
	Json::Reader reader;

	reader.parse(storage, storageObj);

	// clearing data from file
	storage.close();
	storage.open(interop::marshal_as<string>(_storageFileName), std::ofstream::out | std::ofstream::trunc);

	string _base = interop::marshal_as<string>(base);
	string banknoteStr = to_string(banknote);

	storageObj[_base][banknoteStr] = storageObj[_base][banknoteStr].asInt() + 1;

	storage << storageObj;

	storage.close();

	return true;
}

cliext::vector<String^> Storage::getBanknotesOf(String^ base) {
	cliext::vector<String^> banknotes;

	if (base == "RON" || base == "EUR" || base == "USD" || base == "GBP") {
		banknotes.push_back("5");
		banknotes.push_back("10");
		banknotes.push_back("20");
		banknotes.push_back("50");

		if (base == "RON" || base == "EUR" || base == "USD") {
			banknotes.push_back("100");

			if (base == "RON" || base == "EUR") {
				banknotes.push_back("200");
				banknotes.push_back("500");
			}
		}
	}

	return banknotes;
}

bool Storage::existsBanknoteOf(int banknote, String^ base) {
	cliext::vector<String^> banknotes = getBanknotesOf(base);

	for (int i = 0; i < banknotes.size(); i++) {
		if (Int32::Parse(banknotes[i]) == banknote) {
			return true;
		}
	}

	return false;
}

bool Storage::canWithdraw(int amount, String^ base) {
	if (amount % 5 != 0) {
		Console::WriteLine("Cantitatea de bani trebuie sa fie divizibila la 5");
		return false;
	}

	if (getTotalAmountOf(base) < amount) {
		Console::WriteLine("Insuficient cash de \"" + base + "\" in depozitul bancomatului");
		return false;
	}

	return true;
}