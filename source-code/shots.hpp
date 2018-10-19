#include <vector>
#include <math.h>

using namespace std;

class shots: public sf::Drawable, sf::Transformable
{
public:
    shots(bool ja, sf::ContextSettings &settings);

    void dodaj(sf::Vector2f position, unsigned short rotation);
    void usun(int i);

    void aktualizuj();

    int number();
    sf::Vector2f getPosition(int i);

private:
    sf::RenderTexture tekstura;

    float speed = 7.0f;

    struct pocisk
    {
        unsigned short kierunek;
        sf::Vector2f speed;
        sf::Sprite sprajt;
    };

    vector <pocisk> pociski;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

shots::shots(bool ja, sf::ContextSettings &settings)
{
    // przygotowanie tekstury dla sprajtu pocisków
    sf::Color kolor;
    if( ja )
        kolor = sf::Color(0,180,0);
    else
        kolor = sf::Color::Red;

    tekstura.create(7,7, settings);

    sf::CircleShape circle;
    circle.setPosition(1,1);
    circle.setRadius(2.5);
    circle.setFillColor(sf::Color::White);
    circle.setOutlineThickness(1);
    circle.setOutlineColor(sf::Color::Black);
    tekstura.draw(circle);

    circle.setOutlineThickness(0);
    circle.setRadius(1.5);
    circle.setPosition(2,2);
    circle.setFillColor(kolor);
    tekstura.draw(circle);

    tekstura.display();
}

void shots::dodaj(sf::Vector2f position, unsigned short rotation)
{
    pocisk NewPocisk;

    NewPocisk.kierunek = rotation;

    float kierunek = (rotation-1)*45;

    NewPocisk.speed.x = sin(kierunek * M_PI / 180.0f) * speed;
    NewPocisk.speed.y = -cos(kierunek * M_PI / 180.0f) * speed;

    NewPocisk.sprajt.setTexture(tekstura.getTexture());
    NewPocisk.sprajt.setOrigin(3,3);
    NewPocisk.sprajt.setPosition( position + NewPocisk.speed + NewPocisk.speed );  // przesuniêce pocisku tworz¹ce wra¿enie wyrzucaia go przez possaæ gracza na jej obrze¿u, a nie z œrodka

    pociski.push_back(NewPocisk);
}

void shots::usun(int i)
{
    for(int j=i; j<pociski.size()-1; j++)
        swap(pociski[j], pociski[j+1]);

    pociski.resize(pociski.size()-1);
}

void shots::aktualizuj()
{
    for(int i=0; i<pociski.size(); i++)
    {
        pociski[i].sprajt.move( pociski[i].speed );
        sf::Vector2f pozycja = pociski[i].sprajt.getPosition();
        if( (pozycja.x < 0) || (pozycja.x > 600) )  // wychodzenie poza planszê
        {                                           //
            usun(i);                                //
            i--;                                    //
            continue;                               //
        }                                           //
        if( (pozycja.y < 0) || (pozycja.y > 600))   //
        {                                           //
            usun(i);                                //
            i--;                                    //
        }                                           //
    }
}

int shots::number()
{
    return pociski.size();
}

sf::Vector2f shots::getPosition(int i)
{
    return pociski[i].sprajt.getPosition();
}

void shots::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for(int i=0; i<pociski.size(); i++)
        target.draw(pociski[i].sprajt, states);
}
