using namespace std;

void WaitConnection()
{
    string GameName_aswer;
    int gameID_answer;

    do // nawiazywanie polaczenia do gry
    {
        do // rozglaszanie utworzonej gry
        {
            GameBroadcast.clear();  // czyszczenie strumienia
            GameBroadcast << GameName << gameID << myIP.toString();  // wczytanie do strumienia odpowiednich zmiennych z informacjami o grze
            if(sock.send(GameBroadcast, sf::IpAddress::Broadcast, wybrany_port) != sf::Socket::Done)  // wysłanie zawartości strumienia do wszystkich adresów IP w podsieci
                GameBroadcast.clear();  // obsluga bledu - w przypadku niepowodzenia należy wyczyścić strumień z możliwych pozostałości
        }while( !selektor.wait( sf::seconds(1) ) );  // powtarzanie do czasu uzyskania odpowiedzi

        while( selektor.wait( sf::seconds(1) ) && !connection )
        //while( selektor.isReady( sock ) )
        {
            GameBroadcast.clear();  // czyszczenie strumienia przed odbiorem
            if( sock.receive(GameBroadcast, hisIP, wybrany_port) != sf::Socket::Done )  // odbior odpowiedzi
                GameBroadcast.clear();                                                  // obsluga bledu
            else
            {
                GameBroadcast >> GameName_aswer >> gameID_answer;  // wczytanie ze strumienia danych do odpowiednich zmiennych
                if( GameName==GameName_aswer && gameID==gameID_answer)  // sprawdzenie, czy to faktycznie
                {                                                       // odpowiedz czy tylko wlasna
                    string posrednik;                                   // wiadomosc rozglaszajaca grę
                    GameBroadcast >> posrednik;                         //
                    hisIP = posrednik;                                  //
                    if(hisIP!=myIP)                                     //
                    {
                        connection = true;
                        GameBroadcast.clear();  // czyszczenie strumienia przed wczytaniem danych do niego
                        GameBroadcast << hisIP.toString();  // przygotowanie w strumieniu odpowiedzi dla klienta potwierdzającej nawiązanie połączenia
                        if(sock.send(GameBroadcast, hisIP, wybrany_port) != sf::Socket::Done)  // wysłanie tej odpowiedzi
                            cout << " blad sieciowy ";  // obsługa błędu
                    }
                }
            }
        }
    }while(!connection);
}

void opcja1()
{
    bool ifPort = false;                                    // wybór wolnego portu
    for(int i=0; i<liczba_portow; i++)                      //
    {                                                       //
        if(sock.bind( port[i], myIP ) == sf::Socket::Done)  //
        {                                                   //
            wybrany_port = port[i];                         //
            ifPort = true;                                  //
            break;                                          //
        }                                                   //
    }                                                       //

    if(!ifPort) // obsluga bledu
    {
        cout    << "Blad! Brak wolnych portow, obslugiwanych przez program."    << endl
                << "Sprobuj ponownie za chwile"                                 << endl
                                                                                << endl
                << "Wcisnij dowolny klawisz"                                    << endl
                                                                                << endl;
        getch();
    }
    else
    {
        selektor.add(sock);

        cout    << "Powiazano z portem nr " << wybrany_port                     << endl
                                                                                << endl
                << "Podaj nazwe swojej gry (nazwa nie moze zawierac spacji!!)"  << endl
                                                                                << endl;
        cin >> GameName;
        gameID = rand(); // wylosowanie ID gry

        system("CLS"); // wywolanie polecenia systemowego czyszczacego konsole
        cout    << "Nazwa gry: " << GameName            << endl     // wyswietlenie informacji
                << "ID gry: " << gameID                 << endl     // o utworzonej grze
                << "IP hosta: " << myIP                 << endl     //
                << "Port sieciowy: " << wybrany_port    << endl     //
                                                        << endl     //
                << "Oczekuje na gracza"                 << endl     //
                                                        << endl;    //

        WaitConnection(); // nawiazane polaczenia do gry
    }
}
