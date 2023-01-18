#include <iostream>
#include <string.h>
#include <fstream>
#include <algorithm>
#include <map>

using namespace std;

#define PRODUCT_NAME_L 100

struct Product{
    char name[PRODUCT_NAME_L];
    float price;
    int available; 
    int sold;
};

void print(const Product *prod) {
    cout << "\nProdukta nosaukums: " << prod->name << endl;
    cout << "Produkta cena: " << prod->price << endl;
    cout << "Produkta pieejamais daudzums: " << prod->available << endl;
    cout << "Produkta pārdotais daudzums: " << prod->sold << endl << endl;
}

bool compareBySold(const Product &prodA, const Product &prodB) {
    return prodA.sold > prodB.sold;
}

bool compareByProfit(const Product &prodA, const Product &prodB) {
    return prodA.sold * prodA.price > prodB.sold * prodB.price;
}

bool compareByPrice(const Product &prodA, const Product &prodB) {
    return prodA.price > prodB.price;
}

int findProduct(fstream &file, const char *name) {
    char buffer[PRODUCT_NAME_L];
    int retVal = -1;

    file.seekg (0, ios::end);
    int length = file.tellg();
    file.seekg (0, ios::beg);

    for (int i = 0; i < length; i += sizeof(Product)) {
        file.read(buffer, PRODUCT_NAME_L);
        if (!strcmp(name, buffer)) {
            retVal = file.tellg();
            break;
        } else {
            file.seekg(sizeof(float) + sizeof(int) * 2, ios::cur);
        }
    }
    return retVal;
}

void addProduct(const string file) {
    char productName[PRODUCT_NAME_L];
    cout << "Kuru produktu papildināt: ";
    cin.ignore();
    cin.get(productName, PRODUCT_NAME_L);
    if (cin.fail()) {
        cout << "Netika ievadits produkta nosaukums\n";
        cin.clear();
        return;
    }

    fstream shopFile;
    shopFile.open(file, ios::out | ios::in |ios::binary | ios::ate);

    if (shopFile.is_open()) {
        int pos = findProduct(shopFile, productName);
        if (pos > -1) {
            cout << productName << " - Par cik vienībām papildināt skaitu: ";
            int toAdd;
            int curVol;
            cin >> toAdd;
            shopFile.seekg(pos + sizeof(float), ios::beg);
            shopFile.read(reinterpret_cast<char *>(&curVol), sizeof(int));
            toAdd += curVol;
            shopFile.seekp(pos + sizeof(float), ios::beg);
            shopFile.write(reinterpret_cast<char *>(&toAdd), sizeof(int));
        } else {
            Product prod;
            cout << "Produkts vēl neeksistē. Ievadi visus parametrus.\n";
            strcpy(prod.name, productName);
            cout << productName << " - Cena: ";
            cin >> prod.price;
            cout << productName << " - Pieejamais daudzums: ";
            cin >> prod.available;
            prod.sold = 0;

            shopFile.seekp(0, ios::end);
            shopFile.write(reinterpret_cast<char *>(&prod), sizeof(Product));
        }
    } else {
        cout << "ERROR\n";
    }
    shopFile.close();
}

void printAll(const string file) {
    fstream shopFile;
    shopFile.open(file, ios::in |ios::binary);

    if (shopFile.is_open()) {
        shopFile.seekg (0, ios::end);
        int length = shopFile.tellg();
        shopFile.seekg (0, ios::beg);

        Product prod;

        for (int i = 0; i < length; i += sizeof(Product)) {
            shopFile.read(reinterpret_cast<char *>(&prod), sizeof(Product));
            print(&prod);
        }   
    } else {
        cout << "ERROR\n";
    }
    shopFile.close();
}

void sellProduct(const string file) {
    char productName[PRODUCT_NAME_L];
    cout << "Kuru produktu pārdot: ";
    cin.ignore();
    cin.get(productName, PRODUCT_NAME_L);
    if (cin.fail()) {
        cout << "Netika ievadits produkta nosaukums\n";
        cin.clear();
        return;
    }

    fstream shopFile;
    shopFile.open(file, ios::out | ios::in |ios::binary);

    if (shopFile.is_open()) {
        int pos = findProduct(shopFile, productName);
        if (pos > -1) {
            int curVol;
            int curSold;
            shopFile.seekg(pos + sizeof(float), ios::beg);
            shopFile.read(reinterpret_cast<char *>(&curVol), sizeof(int));
            shopFile.seekg(pos + sizeof(float) + sizeof(int), ios::beg);
            shopFile.read(reinterpret_cast<char *>(&curSold), sizeof(int));
            if (curVol <= 0) {
                cout << productName << " - Vairāk nav\n";
            } else {
                curVol--;
                curSold++;
                shopFile.seekp(pos + sizeof(float), ios::beg);
                shopFile.write(reinterpret_cast<char *>(&curVol), sizeof(int));
                shopFile.seekp(pos + sizeof(float) + sizeof(int), ios::beg);
                shopFile.write(reinterpret_cast<char *>(&curSold), sizeof(int));
            }
        } else {
            cout << productName << " - Produkts neeksistē\n";
        }
    } else {
        cout << "ERROR\n";
    }
    shopFile.close();
}

void printProduct(const string file) {
    char productName[PRODUCT_NAME_L];
    cout << "Kura produkta datus izvadit: ";
    cin.ignore();
    cin.get(productName, PRODUCT_NAME_L);
    if (cin.fail()) {
        cout << "Netika ievadits produkta nosaukums\n";
        cin.clear();
        return;
    }

    fstream shopFile;
    shopFile.open(file, ios::in |ios::binary);

    if (shopFile.is_open()) {
        int pos = findProduct(shopFile, productName);
        Product prod;
        shopFile.seekg(pos - PRODUCT_NAME_L, ios::beg);
        shopFile.read(reinterpret_cast<char *>(&prod), sizeof(Product));

        print(&prod);
    } else {
        cout << "ERROR\n";
    }
    shopFile.close();
}

void top3Sold(const string file) {
    fstream shopFile;
    shopFile.open(file, ios::in |ios::binary);

    if (shopFile.is_open()) {
        shopFile.seekg (0, ios::end);
        int length = shopFile.tellg() / sizeof(Product);
        shopFile.seekg (0, ios::beg);

        Product *prod = new Product[length];

        for (int i = 0; i < length; i++) {
            shopFile.read(reinterpret_cast<char *>(&prod[i]), sizeof(Product));
        }

        sort(prod, prod + length, &compareBySold);

        for (int i = 0; i < 3 && i < length; i++) {
            print(&prod[i]);
        }

        delete[] prod;
    } else {
        cout << "ERROR\n";
    }
    shopFile.close();
}

void least3Sold(const string file) {
    fstream shopFile;
    shopFile.open(file, ios::in |ios::binary);


    if (shopFile.is_open()) {
        shopFile.seekg (0, ios::end);
        int length = shopFile.tellg() / sizeof(Product);
        shopFile.seekg (0, ios::beg);

        Product *prod = new Product[length];

        for (int i = 0; i < length; i++) {
            shopFile.read(reinterpret_cast<char *>(&prod[i]), sizeof(Product));
        }

        sort(prod, prod + length, &compareBySold);

        for (int i = 0; i < 3 && i < length; i++) {
            print(&prod[length - i - 1]);
        }
        
        delete[] prod;
    } else {
        cout << "ERROR\n";
    }
    shopFile.close();
}

void top3Profitable(const string file) {
    fstream shopFile;
    shopFile.open(file, ios::in |ios::binary);

    if (shopFile.is_open()) {
        shopFile.seekg (0, ios::end);
        int length = shopFile.tellg() / sizeof(Product);
        shopFile.seekg (0, ios::beg);

        Product *prod = new Product[length];

        for (int i = 0; i < length; i++) {
            shopFile.read(reinterpret_cast<char *>(&prod[i]), sizeof(Product));
        }

        sort(prod, prod + length, &compareByProfit);

        for (int i = 0; i < 3 && i < length; i++) {
            print(&prod[i]);
        }

        delete[] prod;
    } else {
        cout << "ERROR\n";
    }
    shopFile.close();
}

void least3Profitable(const string file) {
    fstream shopFile;
    shopFile.open(file, ios::in |ios::binary);

    if (shopFile.is_open()) {
        shopFile.seekg (0, ios::end);
        int length = shopFile.tellg() / sizeof(Product);
        shopFile.seekg (0, ios::beg);

        Product *prod = new Product[length];

        for (int i = 0; i < length; i++) {
            shopFile.read(reinterpret_cast<char *>(&prod[i]), sizeof(Product));
        }

        sort(prod, prod + length, &compareByProfit);

        for (int i = 0; i < 3 && i < length; i++) {
            print(&prod[length - i - 1]);
        }

        delete[] prod;
    } else {
        cout << "ERROR\n";
    }
    shopFile.close();
}

void top3Expensive(const string file) {
    fstream shopFile;
    shopFile.open(file, ios::in |ios::binary);

    if (shopFile.is_open()) {
        shopFile.seekg (0, ios::end);
        int length = shopFile.tellg() / sizeof(Product);
        shopFile.seekg (0, ios::beg);

        Product *prod = new Product[length];

        for (int i = 0; i < length; i++) {
            shopFile.read(reinterpret_cast<char *>(&prod[i]), sizeof(Product));
        }

        sort(prod, prod + length, &compareByPrice);

        for (int i = 0; i < 3 && i < length; i++) {
            print(&prod[i]);
        }

        delete[] prod;
    } else {
        cout << "ERROR\n";
    }
    shopFile.close();
}

void least3Expensive(const string file) {
    fstream shopFile;
    shopFile.open(file, ios::in |ios::binary);

    if (shopFile.is_open()) {
        shopFile.seekg (0, ios::end);
        int length = shopFile.tellg() / sizeof(Product);
        shopFile.seekg (0, ios::beg);

        Product *prod = new Product[length];

        for (int i = 0; i < length; i++) {
            shopFile.read(reinterpret_cast<char *>(&prod[i]), sizeof(Product));
        }

        sort(prod, prod + length, &compareByPrice);

        for (int i = 0; i < 3 && i < length; i++) {
            print(&prod[length - i - 1]);
        }

        delete[] prod;
    } else {
        cout << "ERROR\n";
    }
    shopFile.close();
}

//Papildinājums

bool compareByAvailable(Product a, Product b) {
    return a.available > b.available;
}

void top3Available(const string file) {
    fstream shopFile;
    shopFile.open(file, ios::in |ios::binary);

    if (shopFile.is_open()) {
        shopFile.seekg (0, ios::end);
        int length = shopFile.tellg() / sizeof(Product);
        shopFile.seekg (0, ios::beg);

        Product *prod = new Product[length];

        for (int i = 0; i < length; i++) {
            shopFile.read(reinterpret_cast<char *>(&prod[i]), sizeof(Product));
        }

        sort(prod, prod + length, &compareByAvailable);

        for (int i = 0; i < 3 && i < length; i++) {
            print(&prod[i]);
        }

        delete[] prod;
    } else {
        cout << "ERROR" << endl;
    }
    shopFile.close();
}

bool compareByAvailableLeast(Product a, Product b) {
    return a.available < b.available;
}

void top3LeastAvailable(const string file) {
    fstream shopFile;
    shopFile.open(file, ios::in |ios::binary);

    if (shopFile.is_open()) {
        shopFile.seekg (0, ios::end);
        int length = shopFile.tellg() / sizeof(Product);
        shopFile.seekg (0, ios::beg);

        Product *prod = new Product[length];

        for (int i = 0; i < length; i++) {
            shopFile.read(reinterpret_cast<char *>(&prod[i]), sizeof(Product));
        }

        sort(prod, prod + length, &compareByAvailableLeast);

        for (int i = 0; i < 3 && i < length; i++) {
            print(&prod[i]);
        }

        delete[] prod;
    } else {
        cout << "ERROR" << endl;
    }
    shopFile.close();
}

void assortedProductsMenu(const string file) {
    fstream shopFile;
    shopFile.open(file, ios::in |ios::binary);
    float budget;

    cout << "Ievadiet budžetu: ";
    cin >> budget;
    float remainingBudget = budget;

    if (shopFile.is_open()) {
        shopFile.seekg (0, ios::end);
        int length = shopFile.tellg() / sizeof(Product);
        shopFile.seekg (0, ios::beg);

        Product *prod = new Product[length];

        for (int i = 0; i < length; i++) {
            shopFile.read(reinterpret_cast<char *>(&prod[i]), sizeof(Product));
        }

        sort(prod, prod + length, &compareByPrice);

        map<string, pair<int, float>> productsBought;

        cout << "Nopirktie produkti:" << endl;
        for (int i = 0; i < length; i++) {
            if (remainingBudget >= prod[i].price && prod[i].available > 0) {
                remainingBudget -= prod[i].price;
                prod[i].available--;
                prod[i].sold++;
                if(productsBought.count(prod[i].name) == 0) {
                    productsBought[prod[i].name] = make_pair(1, prod[i].price);
                }
                else {
                    productsBought[prod[i].name].first++;
                    productsBought[prod[i].name].second += prod[i].price;
                }
                if(remainingBudget<=0) break;
                i--;
            }
        }
        shopFile.clear();
        shopFile.seekp(0, ios::beg);
        for (int i = 0; i < length; i++) {
            shopFile.write(reinterpret_cast<char *>(&prod[i]), sizeof(Product));
        }

        for(auto const& [key, value] : productsBought) {
            cout << "\nProdukta noasaukums:"<< key << endl;
            cout << "Produkta daudzums: " << value.first << endl;
            cout << "Kopējā summa: " << value.second << endl << endl;
        }

        cout << "Atlikušais budžets: " << remainingBudget << endl << endl;

        delete[] prod;
    } else {
        cout << "ERROR" << endl;
    }
    shopFile.close();
}
