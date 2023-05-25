#include "pch.h"
#include "Bancomat.h"

using namespace System;
using namespace msclr;

void introduceCard(Bancomat^);
void useCard(Bancomat^);
String^ chooseCurrency();
void withdrawMoney(Bancomat^, String^);
void otherTransaction(Bancomat^);
void creditMoney(Bancomat^);
void resetPin(Bancomat^);
void transfer(Bancomat^);
void init(Bancomat^);

System::String^ ConvertToDotNetString(std::string simpleString)
{
	std::wstring wideString(simpleString.begin(), simpleString.end());

	System::String^ dotNetString = gcnew System::String(wideString.c_str());

	return dotNetString;
}

void init(Bancomat^ ATM) {
    while (true) {
        Console::WriteLine("Alegeti actiunea dorita: \n 1) Introducere card \n");

        int action = Int32::Parse(Console::ReadLine());

        switch (action) {
        case 1:
            system("cls");
            introduceCard(ATM);
            break;

        default:
            system("cls");
            Console::WriteLine("Va rog frumos introduceti numai cifrele de mai jos.");
        }
    }
}

void transfer(Bancomat^ ATM) {
    while (true) {
        Console::WriteLine("Scrieti numarul cardului: \n\n 0) Anulare\n");
        String^ nrCard = Console::ReadLine();

        if (nrCard == "0") return;

        if (!ATM->existsCardNumber(nrCard)) {
            system("cls");
            Console::WriteLine("Nu exista card cu asa numar");
            continue;
        }

        Card^ card = ATM->getCard(nrCard);

        system("cls");
        Console::WriteLine("Transfer pe cardul " + nrCard);
        Console::WriteLine("Scrieti suma: \n\n 0) Anulare\n");
        int amount = Int32::Parse(Console::ReadLine());

        if (amount == 0) return;

        if (ATM->transfer(card, amount)) {
            Console::WriteLine("Tranzactie indeplinita cu succes.");
        }

        Console::WriteLine("");

        otherTransaction(ATM);
        system("cls");
        break;
    }
    
}

void resetPin(Bancomat^ ATM) {
    Console::WriteLine("Introduceti pinul nou. \n");
    Console::WriteLine(" 0) Anulare \n");

    int action = Int32::Parse(Console::ReadLine());
    
    if (action == 0) return;

    ATM->resetPin(action);

}

void otherTransaction(Bancomat^ ATM) {
    Console::WriteLine("Doriti sa efectuati alta tranzactie? \n 1) Da \n 2) Nu \n");

    int action = Int32::Parse(Console::ReadLine());

    switch (action) {
    case 1:
        
        break;

    case 2:
        ATM->cardOut();
        return;
        break;

    default:
        system("cls");
        Console::WriteLine("Va rog frumos introduceti numai cifrele de mai jos.");
        otherTransaction(ATM);
    }

    system("cls");
}

void introduceCard(Bancomat^ ATM) {
    while (true) {
        Console::WriteLine("Introduceti numarul cardului. \n\n 0) Anulare \n");

        String^ action = Console::ReadLine();

        if (Int32::Parse(action) == 0) {
            system("cls");
            return;
        }

        Card^ card = ATM->getCard(action);

        if (!card) {
            system("cls");
            Console::WriteLine("Nu exista card cu asa numar. \n");
            continue;
        }

        system("cls");

        while (true) {
            Console::WriteLine("Introduceti pin codul. \n 0) Anulare \n");

            int pinCode = Int32::Parse(Console::ReadLine());

            if (pinCode == 0) {
                system("cls");
                return;
            }

            if (ATM->introduceCard(card, pinCode)) {

                system("cls");
                useCard(ATM);
                system("cls");
                return;
            }
            else {
                system("cls");
                Console::WriteLine("Pin cod incorect. \n");
            }
        }
    }
}

void useCard(Bancomat^ ATM) {

    while(ATM->hasCard()){
        Console::WriteLine("Alegeti actiunea dorita: \n 1) Verificare sold \n 2) Extragere din cont \n 3) Depunere in cont \n 4) Reset pin\n 5) Transfer pe alt cont\n 0) Scoatere card\n");
        
        int action = Int32::Parse(Console::ReadLine());

        system("cls");

        if (action == 1) {
            Console::WriteLine("Pe cardul dvs aveti: " + ATM->getSold().ToString() + " RON \n");

            otherTransaction(ATM);
        }
        else if (action == 2) {
            String^ currency = chooseCurrency();
            system("cls");

            if (currency) {
                withdrawMoney(ATM, currency);

                system("cls");
            }
        }
        else if (action == 3) {
            creditMoney(ATM);
        }
        else if (action == 4) {
            resetPin(ATM);
            system("cls");
        }
        else if (action == 5) {
            transfer(ATM);
        }
        else if (action == 0) {
            ATM->cardOut();
        }
        else {
            Console::WriteLine("Va rog frumos tastati numai una din cifrele de mai sus");
        }
    }
}

String^ chooseCurrency() {
    Console::WriteLine("Alegeti valuta dorita: \n 1) RON \n 2) EUR \n 3) USD \n 4) GBP \n\n 0) Inapoi");

    int action = Int32::Parse(Console::ReadLine());

    switch (action) {
    case 1:
        return "RON";
        break;

    case 2:
        return "EUR";
        break;

    case 3:
        return "USD";
        break;

    case 4:
        return "GBP";
        break;

    case 0:
        return nullptr;
        break;

    default:
        system("cls");
        Console::WriteLine("Va rog frumos tastati una din cifrele de mai jos");
        return chooseCurrency();
    }

    return "";
}

void withdrawMoney(Bancomat^ ATM, String^ base) {
    Console::WriteLine("Alegeti suma dorita: \n 1) 10 \n 2) 20 \n 3) 50 \n 4) 100 \n 5) 200 \n 6) 500 \n 7) 1000 \n\n 9) Alta suma \n\n 0) Inapoi");

    int action = Int32::Parse(Console::ReadLine());

    int withdraw = 0;

    switch (action) {
    case 1:
        withdraw = 10;
        break;

    case 2:
        withdraw = 20;
        break;

    case 3:
        withdraw = 50;
        break;

    case 4:
        withdraw = 100;
        break;

    case 5:
        withdraw = 200;
        break;

    case 6:
        withdraw = 500;
        break;

    case 7:
        withdraw = 1000;
        break;

    case 9:
        system("cls");
        Console::WriteLine("Introduceti suma dorita de la 5 pana la 5000.");
        {int amount = Int32::Parse(Console::ReadLine());

        withdraw = amount; }
        
        break;

    case 0:
        system("cls");
        return;
        break;

    default:
        system("cls");
        Console::WriteLine("Va rog frumos tastati numai una din cifrele de mai jos");
        withdrawMoney(ATM, base);
        return;
    }

    system("cls");

    if (withdraw < 5 || withdraw > 5000) {
        Console::WriteLine("Valoarea depaseste diapazonul 5 - 5000");
        withdrawMoney(ATM, base);
        return;
    }
    
    if (ATM->debit(withdraw, base)) {
        Console::WriteLine("Tranzactie finisatata cu succes.");
    }
    else {
        Console::WriteLine("Tranzactie nu a fost indeplinita.");
    }

    Console::WriteLine("");

    otherTransaction(ATM);
}

void creditMoney(Bancomat^ ATM) {
    Json::Value introducedCash;

    while (true) {
        Console::WriteLine("Ati introdus: ");
        Console::WriteLine("  RON: " + introducedCash["RON"].asInt());
        Console::WriteLine("  EUR: " + introducedCash["EUR"].asInt());
        Console::WriteLine("  USD: " + introducedCash["USD"].asInt());
        Console::WriteLine("  GBP: " + introducedCash["GBP"].asInt());
        Console::WriteLine("");

        String^ base = chooseCurrency();

        system("cls");

        if (!base) break;

        std::string _base = interop::marshal_as<std::string>(base);

        cliext::vector<String^> banknotes = ATM->getBanknotesOf(base);

        while (true) {
            Console::WriteLine("Ati introdus: " + introducedCash[_base].asInt() + " " + base + "\n");
            Console::WriteLine("Alegeti una din bancnote: \n");

            for (int i = 0; i < banknotes.size(); i++) {
                Console::WriteLine(" " + (i+1) + ") " + banknotes[i]);
            }

            Console::WriteLine("\n 0) Inapoi \n");

            int action = Int32::Parse(Console::ReadLine());

            int amount = 0;

            switch (action) {
            case 1:
                amount = 5;
                break;
            
            case 2:
                amount = 10;
                break;

            case 3:
                amount = 20;
                break;

            case 4: 
                amount = 50;
                break;

            case 5:
                amount = 100;
                break;

            case 6:
                amount = 200;
                break;

            case 7:
                amount = 500;
                break;

            case 0:
                system("cls");
                break;

            default:
                system("cls");
                Console::WriteLine("Va rog frumos tastati numai una din cifrele de mai jos");
                continue;
            }

            if (amount == 0) break;

            if (!ATM->existsBanknoteOf(amount, base)) {
                system("cls");
                Console::WriteLine("Va rog frumos tastati numai una din cifrele de mai jos");
                continue;
            }

            ATM->credit(amount, base);

            introducedCash[_base] = introducedCash[_base].asInt() + amount;

            system("cls");
        }

    }
}

int main(array<System::String ^> ^args)
{
    Bancomat^ ATM = gcnew Bancomat;

    init(ATM);
}
