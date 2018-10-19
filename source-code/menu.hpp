#include "opcja1.hpp"
#include "opcja2.hpp"

using namespace std;

void MainMenu(short int &opcja)
{
    do
    {
        system("CLS"); //wywolanie polecenia systemowego czyszczacego konsole (potrzeba usuniecia znaku wpisanego przez uzytkownika)
        cout    << "Witaj w GraSieciowa"    << endl
                                            << endl
                << "Opcje:"                 << endl
                << "1 - stworz gre"         << endl
                << "2 - wyszukaj gre"       << endl
                << "3 - wyjdz"              << endl
                                            << endl;
        opcja = getchar();
    }while(opcja<'1' || opcja>'3'); // obsluga bledu uzytkownika
}

void opcjeMenu(short int &opcja)
{
    switch(opcja)
    {
    case '1':
        opcja1();
        break;

    case '2':
        opcja2();
        break;

//  case '3':
//      niepotrzebne, bo 3 odpowiada za wyjœcie z programu
//      i jest sprawdzane w warunku petli glownej programu
    }
}

