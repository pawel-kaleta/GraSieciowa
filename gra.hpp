#include "fraktal.hpp"
#include "gracz.hpp"
#include "shots.hpp"
#include "hud.hpp"

sf::ContextSettings settings;
sf::Clock fps;

unsigned short ruch;
bool shot;
unsigned short direction;

void drawFrame(sf::RenderWindow &window, gracz &ON, gracz &JA, fraktal &Fraktal, shots &pociskiMoje, shots &pociskiJego, hud &MyHUD, hud &HisHUD)
{
    window.clear();  // wyczyszczenie bufora klatki
    window.draw( Fraktal );     // rysowanie w buforze kolejnych obiekt�w
    window.draw( pociskiMoje ); //
    window.draw( pociskiJego ); //
    window.draw( ON );          //
    window.draw( JA );          //
    window.draw( MyHUD );       //
    window.draw( HisHUD );      //
    window.display();  // wy�wietlenie klatki z bufora
}

void interaktywnosc(bool &koniec, sf::RenderWindow &window, gracz &JA, gracz &ON, shots &pociskiMoje, shots &pociskiJego, hud &MyHUD, hud &HisHUD)
{
    ON.patrz(direction);                                        // wykonanie ruchow przeciwnika
    if( shot )                                                  //
    {                                                           //
        pociskiJego.dodaj( ON.getPosition() , ON.direction );   //
        HisHUD.strzel();                                        //
    }                                                           //
    ON.idz(ruch);                                               //

    sf::Event zdarzenie;
    while( window.pollEvent(zdarzenie) )  // odczytanie zdarzenia dla utworzonego okna
    {
        if( zdarzenie.type == sf::Event::KeyReleased  &&  zdarzenie.key.code == sf::Keyboard::Escape )
            koniec = true;  // wyj�cie z gry

        bool delay = 0;                                                                         // op�nienie celem
                                                                                                // zwi�kszenie p�ynno�ci
        if(zdarzenie.type == sf::Event::KeyReleased || zdarzenie.type == sf::Event::KeyPressed) // sterowania - naciskanie i
        {                                                                                       // odpuszczanie dw�ch
            if( zdarzenie.key.code == sf::Keyboard::W ||                                        // klawiszy prawie jednocze�nie
                zdarzenie.key.code == sf::Keyboard::D ||                                        // uznawane za jedn� akcj�
                zdarzenie.key.code == sf::Keyboard::S ||                                        //
                zdarzenie.key.code == sf::Keyboard::A)                                          //
                delay = 1;                                                                      //
        }                                                                                       //

        if(!delay)
        {
            unsigned short dir = 0;                                 // ka�da z cyfr zmiennej dir informuje o tym, czy posta� patrzy
                                                                    // w dan� stron� - g�ra, prawo, d�, lewo
            if( sf::Keyboard::isKeyPressed(sf::Keyboard::W) )       //
                if( !sf::Keyboard::isKeyPressed(sf::Keyboard::S) )  // dlatego nigdy w parach 1 i 3 oraz 2 i 4 cyfra nie s�
                    dir +=1000;                                     // jedocze�nie na "1", nie mo�na patrze� jednocze�nie
            if( sf::Keyboard::isKeyPressed(sf::Keyboard::D) )       // w lewo i w prawo
                if( !sf::Keyboard::isKeyPressed(sf::Keyboard::A) )  //
                    dir += 100;                                     //
            if( sf::Keyboard::isKeyPressed(sf::Keyboard::S) )       //
                if( !sf::Keyboard::isKeyPressed(sf::Keyboard::W) )  //
                    dir +=  10;                                     //
            if( sf::Keyboard::isKeyPressed(sf::Keyboard::A) )       //
                if( !sf::Keyboard::isKeyPressed(sf::Keyboard::D) )  //
                    dir +=   1;                                     //
                                                                    //
            if(dir!=0)                                              //
            {                                                       //
                switch( dir )                                       //
                {                                                   //
                    case 1000: dir=1; break;                        //
                    case 1100: dir=2; break;                        //
                    case  100: dir=3; break;                        //
                    case  110: dir=4; break;                        //
                    case   10: dir=5; break;                        //
                    case   11: dir=6; break;                        //
                    case    1: dir=7; break;                        //
                    case 1001: dir=8; break;                        //
                }
                JA.patrz(dir);
            }
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if( JA.czasOdAtaku.getElapsedTime() > sf::seconds(0.1) && MyHUD.ammo > 0)  // ograniczenie szybkostrzelno�ci i sprawdzenie amunicji
        {
            pociskiMoje.dodaj( JA.getPosition() , JA.direction );
            JA.czasOdAtaku.restart();
            MyHUD.strzel();
            shot = 1;   // zmienna do przeslania przez siec
        }               //
        else            //
            shot = 0;   //
    }                   //
    else                //
        shot = 0;       //

    int x = sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) - sf::Keyboard::isKeyPressed( sf::Keyboard::Left );   // lewno -1, prawo +1, brak ruchu w poziomie 0
    int y = sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) - sf::Keyboard::isKeyPressed( sf::Keyboard::Down );      // d� -1, g�ra +1, brak ruchu w pionie 0

    JA.idz( (x+1)*10 + (y+1) );  // sklejenie "x" i "y" w jedn� zmienn�: najpierw przekszta�camy zakres z -1 do +1, na 0 do +2, a potem traktujemy je jak cyfry jenej liczby
    ruch = (x+1)*10 + ( 2-(y+1) ); // zmienna do przeslania przez siec, przeliczona na odbicie lustrzane

    // zmienna do przeslania przez siec, przeliczona na lustrzane odbicie w pionie (obr�t o 180 i odbicie w poziomie)
    direction = JA.direction + 4;   // obr�t o 180
    if(direction>8)                 //
        direction-=8;               //
    direction = 10-direction;  // odbicie lustrzane w poziomie
}

void kolizje(gracz &ON, gracz &JA, shots &pociskiMoje, shots &pociskiJego, hud &MyHUD, hud &HisHUD)
{
    sf::Vector2f distance;
    distance.x = ON.getPosition().x - JA.getPosition().x;           // kolizja graczy
    distance.y = ON.getPosition().y - JA.getPosition().y;           //
    if( (distance.x*distance.x) + (distance.y*distance.y) < 1024 )  // 1024 = 32^2  -  wzajemne po�o�enie dw�ch okr�g�w
    {
        distance = ON.getSpeed();       // prosta zamiana pr�dko�ci
        ON.setSpeed( JA.getSpeed() );   //
        JA.setSpeed( distance );        //
    }

    for(int i=0; i<pociskiMoje.number(); i++)  // kolizja przeciwnika z moimi pociskami
    {
        distance.x = ON.getPosition().x - pociskiMoje.getPosition(i).x;
        distance.y = ON.getPosition().y - pociskiMoje.getPosition(i).y;

        if( (distance.x*distance.x) + (distance.y*distance.y) < 256 ) // 256 = 16^2
        {
            HisHUD.ran();
            pociskiMoje.usun(i);
            i--;
        }
    }

    for(int i=0; i<pociskiJego.number(); i++)  // kolizja graca z pociskami przeciwnika
    {
        distance.x = JA.getPosition().x - pociskiJego.getPosition(i).x;
        distance.y = JA.getPosition().y - pociskiJego.getPosition(i).y;

        if( (distance.x*distance.x) + (distance.y*distance.y) < 256 ) // 256 = 16^2
        {
            MyHUD.ran();
            pociskiJego.usun(i);
            i--;
        }
    }
}

void gra(bool server)
{
    sf::RenderWindow window(sf::VideoMode(600, 622), "GraSeciowa", sf::Style::Default);  // utworzenie nowego okna do wy�wietlania gameplay'u
    window.requestFocus();  // wys�anie systemowi operacyjnemu pro�my o nadanie fucusu na to okno
    settings.antialiasingLevel = 8;  // poziom wyg�adzania renderowanych linni

    fraktal Fraktal;  // obiekt tworz�cy t�o
    gracz JA( true , settings );
    gracz ON( false , settings );
    shots pociskiMoje( true , settings );
    shots pociskiJego( false , settings );
    hud MyHUD( true , settings );   // interfejsy
    hud HisHUD( false , settings ); //

    bool koniec = false;

    gameData.clear();
    selektor.clear();
    selektor.add(sock);

    ruch = 11;      // inicjacja warto�ci pocz�tkowych o grze
    shot = false;   //
    direction = 5;  //

    if(server) // inicjacja komunikacji wewnatrz gry (brak rozroznienia we wlasciwym kodzie gry na serwer i klienta, oba czekalyby najpierw na druga strone)
    {
        gameData << ruch << shot << direction;
        while(sock.send( gameData , hisIP , wybrany_port ) != sf::UdpSocket::Done) // w petli, aby miec pewnosc, ze zostanie wyslane (w przypadku niedokaczenia wysylki kontynuacja - szczeg�y funkcjonowania klasy w bibliotece)
                ;
    }

    while(!koniec) // glowna petla gry
    {
        int FPS;
        int krokiFraktala = 0;

        do // ograniczenie liczby klatek na sekunde, aby gra miala stala predkosc na kazdym komputerze - nadwyzka zasobow wykorzystna do wyswietlania efektu graficznego w tle
        {
            Fraktal.krok();  // wykonanie kolejnego kroku efektu graficznego w tle

            drawFrame(window, ON, JA, Fraktal, pociskiMoje, pociskiJego, MyHUD, HisHUD);  // wy�wietlenie klatki gry

            krokiFraktala++;
            FPS = sf::seconds(1.0f) / fps.getElapsedTime();  // wyliczenie pr�dko�ci chwilowej gry

        }while( FPS>60);

        fps.restart();  // zrestartowanie zegara odpowiedzialnego za konrtol� pr�dko�ci gry

        gameData.clear();
        while( sock.receive( gameData , hisIP, wybrany_port)!=sf::Socket::Done )  // odbi�r informacji o akcjach przeciwnika
            ;

        gameData >> ruch >> shot >> direction;  // wczytanie ze strumienia zmiennych okre�laj�cych decyzje przeciwnika

        interaktywnosc(koniec, window, JA, ON, pociskiMoje, pociskiJego, MyHUD, HisHUD);  // akcje wykonywane przez graczy

        pociskiMoje.aktualizuj();   // fizyka gry - ruch
        pociskiJego.aktualizuj();   //
        JA.aktualizuj();            //
        ON.aktualizuj();            //
        MyHUD.aktualizuj();     // aktualizacja interface'u
        HisHUD.aktualizuj();    //

        kolizje(ON, JA, pociskiMoje, pociskiJego, MyHUD, HisHUD);  // fizyka gry - kolizje

        gameData.clear();
        gameData << ruch << shot << direction;  // przygoowanie do wysy�ki informacji o decyzjach gracza
        while( sock.send( gameData , hisIP , wybrany_port) != sf::UdpSocket::Done )  // wysy�ka tych informacji
            ;

        if( MyHUD.life<=0 || HisHUD.life<=0 )  // sprawdzenie czy gra si� zako�czy�a
            koniec = true;
    }
}

