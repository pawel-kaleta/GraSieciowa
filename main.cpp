#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib> // system("CLS");
#include <conio.h> // getch();
#include <time.h> // srand(time(NULL));
#include <vector>
#include <windows.h> // zarządzanie konsolą

using namespace std;

COORD a;                    // obiekt koordynatów kursora w konsoli
CONSOLE_CURSOR_INFO cci;    // obiekt do manipulacji konsolą

const unsigned short liczba_portow = 4; // liczba portów, które program ma obsługiwać, większa liczba zmiejsza prędkość nawiązywania połączenia, ale zwiększa niezawodność.
unsigned short port[liczba_portow];     // porty sieciowe, które program ma obsługiwać, tworzone na początku maina z liczba_portow
unsigned short wybrany_port;            // port, na którym odbywa się komunikacja odnośnie części właściwej gry

string GameName;
int gameID;                                             // ID gry, aby rozróznic gry o takich samych nazwach z tego samego IP
sf::Packet GameBroadcast;                               // rozgłaszanie w sieci gier - obiekt tworzący paczki so wysyłi przez sieć, operuje się nim jak strumieniem i daje jako argument dla funkcji wysyłającej i odbierającej dane przez sieć
sf::IpAddress myIP = sf::IpAddress::getLocalAddress();  // obiekt przechowujący IP komputera, na który uruchamiany jest program, przypisuję go do socketu pracującego na konkretnym porcie
sf::IpAddress hisIP = sf::IpAddress::Any;               // obiekt przechowujący IP drugiego kompuera, używany do odbioru danych z sieci
sf::UdpSocket sock;                                     // obiekt, ktrego funkcje służą do wysyłki i odbioru danych przez sieć, wiążę go z moim IP i portem sieciowym
sf::SocketSelector selektor;                            // obiekt do sprawdzania, czy można coś odczytać z soketów
bool connection;                                        // zmienna informująca czy udało się nawiązać połączenie i czy można rozpocząć grę

sf::Packet gameData; // przesył danych o grze - obiekt tworzący paczki do wysyłi przez sieć, operuje się nim jak strumieniem i daje jako argument dla funkcji wysyłającej i odbierającej dane przez sieć

// dołączenie własnych plików PO zmiennych i obiektach globalnych!
#include "gra.hpp"
#include "menu.hpp"

int main()
{
    srand( time(NULL) );  // losowania bazuja na zawsze aktualnym czasie, a nie na czasie z początku programu
    a.X = 0;    // wyzerowanie zmiennej do ustalania pozycji kursora konosli
    a.Y = 0;    //

    // predefiniowana lista portów, aby zawsze znaleźć jakiś wolny
    for(int i=0; i<liczba_portow; i++)
        port[i] = 56000 + i;

    short int opcja;    // zmienna przechowująca wybór opcji w menu głównym

    do // głowna petla programu
    {
        connection = false;         // "wyzerowanie" zmiennych globalnych
        hisIP = sf::IpAddress::Any; // przy ponownym "uruchommieniu" programu

        MainMenu(opcja);            // wyswietlenie menu glownego i wczytanie wybranej opcji

        opcjeMenu(opcja);           // wykonnie opcji wybranej przezuzytkownika

        if( connection )
        {
            gra(2-opcja);             // uruchomienie gry wlasciwej (konwersja opcji na bool rozróżniający serwer i klienta)
            selektor.clear();
        }

    }while( opcja!='3' );

    return 0;
}
