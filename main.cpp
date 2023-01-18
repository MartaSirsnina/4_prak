#include "my_functions.h"
#include <map>

#define FILE_NAME "shop.dat"

enum eMenu {
    End,
    Input,
    PrintAll,
    Sell,
    PrintOne,
    Top3Sold,
    Least3Sold,
    Top3Profitable,
    Least3Profitable,
    Top3Expensive,
    Least3Expensive,
    AssortedProducts,
    Top3MostAvailable,
    Top3LeastAvailable
};

int main() {
    ifstream file;
    file.open(FILE_NAME);
    if (!file.is_open()) {
        ofstream file;
        file.open(FILE_NAME);
        file.close();
    }
    file.close();

    int input;

    while (true) {
        cout << "1 - Datu ievade\n";
        cout << "2 - Visu datu izvade\n";
        cout << "3 - Pārdot produktu\n";
        cout << "4 - Konkrēta produkta izvade\n";
        cout << "5 - TOP3 pārdotākie produkti\n";
        cout << "6-  TOP3 vismazāk pārdotākie produkti\n";
        cout << "7 - TOP3 pelnošākie produkti\n";
        cout << "8 - TOP3 mazāk pelnošākie produkti\n";
        cout << "9 - TOP3 dārgākie produkti\n";
        cout << "10 -TOP3 lētākie produkti\n";
        cout << "11 - Asorti\n";
        cout << "12 - TOP3 visvairāk pieejamākie produkti\n";
        cout << "13 - TOP3 vismazāk pieejamākie produkti\n";
        cout << "0 vai cits simbols - Beigt darbu\n";
        
        cin >> input;

        switch (input) {
            case Input:
                addProduct(FILE_NAME);
                continue;
            case PrintAll:
                printAll(FILE_NAME);
                continue;
            case Sell:
                sellProduct(FILE_NAME);
                continue;
            case PrintOne:
                printProduct(FILE_NAME);
                continue;
            case Top3Sold:
                top3Sold(FILE_NAME);
                continue;
            case Least3Sold:
                least3Sold(FILE_NAME);
                continue;
            case Top3Profitable:
                top3Profitable(FILE_NAME);
                continue;
            case Least3Profitable:
                least3Profitable(FILE_NAME);
                continue;
            case Top3Expensive:
                top3Expensive(FILE_NAME);
                continue;
            case Least3Expensive:
                least3Expensive(FILE_NAME);
                continue;
            case AssortedProducts:
                assortedProductsMenu(FILE_NAME);
                continue;
            case Top3MostAvailable:
                top3Available(FILE_NAME);
                continue;
            case Top3LeastAvailable:
                top3LeastAvailable(FILE_NAME);
                continue;
            case End:
                break;
            default:
                cout << "Nepareiza opcija, mēģini vēlreiz" << endl;
                continue;
        }
        break;
    }
    return 0;
}