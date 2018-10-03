#include "colors.hpp"
#include <time.h>

// obiekt s�u�y do wy�wietlania w tle gry fraktalu za pomoc� metody "gra w chaos":
// zaczynaj�c od pewnego punktu kolejne iteracje za pomoc� ka�dorazowo losowo wybranej funkcji
// dodaj� kolejne punkty

// funkcje iteracyjne - przeskok o po�ow� odleg�o�ci do danego punktu
// punktami s� rogi planszy i �rodek


class fraktal: public sf::Drawable, sf::Transformable
{
public:
    fraktal(void);

    void krok();

private:
    sf::RectangleShape backgroud;  // obiekt t�a, kt�ry sysujemy w oknie
    sf::RenderTexture backgroudTexture;  // obiekt na kt�rym dokonujemy wysownia kolejnych punkt�w fraktalu - dzia�a jak okno - jego wygl�d jest przenoszony w ka�dej klatce na obiekt background
    sf::Vector2i punkt;
    sf::Vector2i punkty[7];
    int i = 0;  // zliczanie wykonanych krok�w
    int r = rand()%360;  // wylosowany k�t palety kolor�w fraktalu
    int wyb;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

fraktal::fraktal(void)
{
    backgroud.setSize( sf::Vector2f(600.0f, 600.0f) );

    int a = rand()%600;             // wylosowanie punktu pocz�tkowego
    int b = rand()%600;             //
    punkt = sf::Vector2i( a , b );  //

    backgroudTexture.create(600,600);
    backgroudTexture.clear(sf::Color::Black);
    backgroudTexture.display();
    backgroud.setTexture(&backgroudTexture.getTexture());

    // predefiniowana lista punkt�w potrzebnych w funkcjach iteracyjnych
    punkty[0] = sf::Vector2i(  0,  0);
    punkty[1] = sf::Vector2i(  0,600);
    punkty[2] = sf::Vector2i(600,600);
    punkty[3] = sf::Vector2i(600,  0);
    punkty[4] = sf::Vector2i(300,300);  // 3x ten sam punkt dla �atwej manipulacji prawdopodobie�stwem
    punkty[5] = sf::Vector2i(300,300);  // wywo�ania poszczeg�lnych funkcji iteracyjnych
    punkty[6] = sf::Vector2i(300,300);  //
}

void fraktal::krok()
{
    int a;  // ograniczenie zakresu punkt�w - manipulacja prawdopodobie�stwem poszczeg�lnych funkcji iteracyjnych
            if(i<  2000000)
            {
                if(i< 200000)
                {
                    if(i<20000)
                        a=7;  // szybkie nadanie og�lnego kszta�tu - 3x wi�ksze prawd. wybrania �rodka, ni� rogu
                    else
                        a=6;
                }
                else
                    a=5;
                i++;
            }
            else
                a=4;  // dok�adniejsze wype�nianie wolnych przestrzeni na planszy - r�wne prawdopodobie�stwo wyboru �rodka, jak i rogu

            wyb = rand()%a;

            punkt.x = (punkt.x + punkty[wyb].x) / 2;  // przeskok o po�ow� odleg�o�ci do wylosowaego punktu
            punkt.y = (punkt.y + punkty[wyb].y) / 2;  //

            sf::RectangleShape point(sf::Vector2f(1,1)); // utworzenie obiektu punktu do wy�wietlenia
            point.setPosition(punkt.x,punkt.y);  // nadanie mu pozycji
            sf::Vector3i kolor = HSVtoRGB( XYtoHSV( punkt,r ) );  // utworzenie dla niego koloru ( pozycja w XoY > kolor w HSV > kolor w RGB)
            point.setFillColor( sf::Color(kolor.x,kolor.y,kolor.z) );  // nadanie mu go
            backgroudTexture.draw(point);  // narysowanie punktu w buforze tekstury BEZ JEGO UPRZEDNIEGO WYCZYSZCZENIA - zachowujemy dotychczas postawione punkty
            backgroudTexture.display();  // "wy�wietlenie" testury
            backgroud.setTexture(&backgroudTexture.getTexture());  // nadanie jej obiektowi t�a
}

void fraktal::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(backgroud);
}
