using namespace std;

struct LANgame // obiekt odczytanej z sieci deklaracji gry
{
    string name;
    int ID;
    string hostIP;
    unsigned short port;
    sf::Clock activeTime; // zegar odpowiedzialny za wygasniecie gry
};

bool CompareLANgames(LANgame &first, LANgame &second)
{
    if( first.ID == second.ID )
        if( first.name == second.name )
            if( first.hostIP == second.hostIP )
                return true;
    return false;
}

void OdczytajGreLAN( unsigned short &aktywny_port, vector <LANgame> &LANgames)
{
    LANgame new_LANgame;
    bool ifNew = true;
    GameBroadcast >> new_LANgame.name >> new_LANgame.ID >> new_LANgame.hostIP;

    for(int i=0; i<LANgames.size(); i++)                    // sprawdzenie, czy odebrana gra jest juz
        if( CompareLANgames( new_LANgame, LANgames[i] ) )   // na liscie aktywnych gier
        {
            ifNew = false;
            LANgames[i].activeTime.restart();  // zrestartowanie zegara odpowiedzialnegoo za jej wygasniecie
            break;
        }

    if( ifNew )  // jesli nie jest na liści to dodanie jej do niej
    {
        new_LANgame.port = aktywny_port;
        LANgames.push_back(new_LANgame);
    }
}

void OdczytGierLAN(vector <LANgame> &LANgames, sf::UdpSocket sockets[], int &liczba_socketow)
{
    if( selektor.wait( sf::seconds(0.5f) ) ) // sprawdzenie, czy są sockety gotowe do odbioru
    {
        for(int i=0; i<liczba_socketow; i++)    // wyszukanie tych socketow
            if( selektor.isReady(sockets[i]) )  //
            {
                unsigned short aktywny_port = sockets[i].getLocalPort();  // sprawdzenie portu, na jakim działa dany socket
                if( sockets[i].receive( GameBroadcast, hisIP, aktywny_port ) == sf::Socket::Done)  // odbiór informacji o grze
                    OdczytajGreLAN(aktywny_port, LANgames);  // odczyt ze strumienia informacji o grze do wektora
                else // obsluga bledu
                {
                    GameBroadcast.clear();
                    cout << "Blad! Nie udalo sie odczytac danych z portu gotowego do odbioru." << endl;
                }
            }
    }
}

void wybierzGre(vector<LANgame> &LANgames, char &gameNumber, sf::UdpSocket sockets[], int &liczba_socketow, bool &portERROR)
{
    GameName = LANgames[gameNumber].name;       //zapisanie danych o grze
    gameID = LANgames[gameNumber].ID;           //
    hisIP = LANgames[gameNumber].hostIP;        //
    wybrany_port = LANgames[gameNumber].port;   //

    for(int i=0; i<liczba_socketow; i++)    // zwalnianie portów i opróżnienie selektora
        sockets[i].unbind();                //
    selektor.clear();                       //

    if( sock.bind( wybrany_port, myIP ) != sf::Socket::Done )
        portERROR = true;
    else
    {
        selektor.add(sock);
        while(!connection)
        {
            GameBroadcast.clear();
            GameBroadcast << GameName << gameID << myIP.toString();  // przygotowanie odpowiedzi w strumieniu
            if( sock.send( GameBroadcast, hisIP, wybrany_port) != sf::Socket::Done )  // wyslanie odowiedzi
                portERROR = true;
            else
            {
                if( !selektor.wait( sf::seconds(5.0f)) ) // oczekiwanie na potwierdzenie połączenia
                    portERROR = true;
                else
                 {
                    GameBroadcast.clear();
                    if( sock.receive( GameBroadcast, hisIP, wybrany_port) != sf::Socket::Done ) // odebranie potwierdzenia
                        portERROR = true;
                    else
                    {
                        string answer;                  // sprawdzenie poprawnosci potwierdzenia
                        GameBroadcast >> answer;        //
                        if(answer == myIP.toString())   //
                            connection = true;          //
                        else
                            GameBroadcast.clear();
                    }
                }
            }

            if( portERROR ) // obsluga bledów
            {
                GameBroadcast.clear();
                selektor.clear();
                sock.unbind();
                cout << "Blad sieciowy! Ponawiam." << endl;

            }
        }
    }
}

void opcja2()
{
    int liczba_socketow = 0;                                                        // utworzenie gniazd
    sf::UdpSocket sockets[liczba_portow];                                           // nasluchujacych
    for(int i=0; i<liczba_portow; i++)                                              // czy sa aktywne gry
        if( sockets[liczba_socketow].bind(port[i], myIP) == sf::UdpSocket::Done )   // w sieci
        {                                                                           //
            selektor.add( sockets[liczba_socketow] );                               //
            liczba_socketow++;                                                      //
        }                                                                           //

    if(liczba_socketow==0) // obsluga bledu
    {
        cout    << "Blad! Brak wolnych portow, obslugiwanych przez program."    << endl
                << "Sprobuj ponownie za chwile"                                 << endl
                                                                                << endl
                << "Wcisnij dowolny klawisz";
        getch();
    }
    else
    {
        vector <LANgame> LANgames;  // wektor do przechowywania gier w sieci z możliwością dołączenia
        bool portERROR = false;

        do
        {
            OdczytGierLAN(LANgames, sockets, liczba_socketow);

            system("CLS");
            cout    << "Wyszukuje gier w sieci."    << endl
                                                    << endl
                    << "Aktywne gry: "              << endl
                                                    << endl;
            bool ifGame = false;

            // wypisanie gier i czyszczenie nieaktywnych
            for(int i=0; i<LANgames.size(); i++)
            {
                if( LANgames[i].activeTime.getElapsedTime() > sf::seconds(2.5f) )
                {
                    LANgames.erase(LANgames.begin() + i);
                    i--;
                }
                else
                {
                    ifGame = true;
                    cout << i+1 << ".   " << LANgames[i].name << "   ID gry: " << LANgames[i].ID << "   IP hosta: " << LANgames[i].hostIP << "   Port sieciowy: " << LANgames[i].port << endl;
                }
            }

            if(ifGame)  // jeśli jest jakaś gra z możliwością dołączenia
            {
                cout << endl << "Wybierz gre - wpisz jej l.p." << endl << endl;

                if( kbhit() )  // zacznij wcztywanie dopiero, jak gracz zacznie coś wpisywać
                {
                    char gameNumber;

                    cin >> gameNumber;
                    gameNumber--;  // konwersja na indeks w wektorze
                    gameNumber-=48;  // kowersja z ascii na dziesietny

                    if( gameNumber>=0 && gameNumber < LANgames.size() )  //obsługa błędu urzytkownika
                        wybierzGre(LANgames, gameNumber, sockets, liczba_socketow, portERROR);
                }
            }
        }while(!connection && !portERROR);
    }
}
