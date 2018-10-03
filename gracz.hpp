#include <math.h>

#define M_PI 3.1415

class gracz: public sf::Drawable, sf::Transformable
{
public:
    gracz( bool ja , sf::ContextSettings &settings );

    sf::Vector2f getPosition();
    sf::Vector2f getSpeed();
    void setSpeed( sf::Vector2f newSpeed);

    void aktualizuj();

    sf::Clock czasOdAtaku;

    int direction;

    void patrz(unsigned short &dir);

    void idz(unsigned short dir);
    void ran();

private:
    sf::Sprite sprajt;
    sf::RenderTexture tekstura;  // obiekt, który dzia³a podobnie do okna - posiada bufor, który mo¿na czyœciæ i mo¿na na nim rysowaæ, a po "wyœwietleniu" pobraæ z obiektu teksturê do nadania jej innemu obiektowi

    float speed = 0.5;
    sf::Vector2f XYspeed;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

gracz::gracz( bool ja , sf::ContextSettings &settings )
{
    direction = 1;

    // wygenerowanie tekstury dla sprajtu postaci gracza
    sf::Color kolor;

    if( ja )
    {
        kolor = sf::Color(0,180,0);
        sprajt.setPosition(300,525);
    }
    else
    {
        kolor = sf::Color::Red;
        sprajt.setPosition(300,75);
        sprajt.setRotation(180);  // odwrócenie przeciwnika przodem
    }

    tekstura.create(32,32, settings);

    sf::CircleShape circle;

    circle.setOrigin(15,15);
    circle.setPosition(16,16);
    circle.setRadius(15);
    circle.setFillColor(sf::Color::White);
    circle.setOutlineThickness(1);
    circle.setOutlineColor(sf::Color::Black);
    tekstura.draw(circle);
    circle.setOutlineThickness(0);

    circle.setRadius(10);
    circle.setFillColor(kolor);
    circle.setPosition(21,21);
    tekstura.draw(circle);

    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(0,-15));
    triangle.setPoint(1, sf::Vector2f(15,0));
    triangle.setPoint(2, sf::Vector2f(-15,0));
    triangle.setPosition(16,16);
    triangle.setFillColor(sf::Color::Black);
    tekstura.draw(triangle);

    triangle.setPoint(1, sf::Vector2f(10,0));
    triangle.setPoint(2, sf::Vector2f(-10,0));
    triangle.setPosition(16,16);
    triangle.setFillColor(kolor);
    tekstura.draw(triangle);

    circle.setRadius(5);
    circle.setPosition(26,26);
    circle.setFillColor(sf::Color::Black);
    tekstura.draw(circle);

    triangle.setPoint(1, sf::Vector2f(5,0));
    triangle.setPoint(2, sf::Vector2f(-5,0));
    triangle.setFillColor(sf::Color::Black);
    tekstura.draw(triangle);

    tekstura.display();

    sprajt.setTexture(tekstura.getTexture());
    sprajt.setOrigin(16,16);


///////////////////////////////////////////////////////////
//    TWORZENIE IKONY PROGRAMU
//    tekstura.create(62,62, settings);
//
//    sf::CircleShape circle;
//    circle.setOrigin(30,30);
//    circle.setPosition(31,31);
//    circle.setRadius(30);
//    circle.setFillColor(sf::Color::White);
//    circle.setOutlineThickness(1);
//    circle.setOutlineColor(sf::Color::Black);
//    tekstura.draw(circle);
//    circle.setOutlineThickness(0);
//
//    circle.setRadius(20);
//    circle.setFillColor(kolor);
//    circle.setPosition(40,40);
//    tekstura.draw(circle);
//
//    sf::ConvexShape triangle;
//    triangle.setPointCount(3);
//    triangle.setPoint(0, sf::Vector2f(0,-30));
//    triangle.setPoint(1, sf::Vector2f(30,0));
//    triangle.setPoint(2, sf::Vector2f(-30,0));
//    triangle.setPosition(31,31);
//    triangle.setFillColor(sf::Color::Black);
//    tekstura.draw(triangle);
//
//    triangle.setPoint(1, sf::Vector2f(20,0));
//    triangle.setPoint(2, sf::Vector2f(-20,0));
//    triangle.setPosition(31,31);
//    triangle.setFillColor(kolor);
//    tekstura.draw(triangle);
//
//    circle.setRadius(10);
//    circle.setPosition(51,51);
//    circle.setFillColor(sf::Color::Black);
//    tekstura.draw(circle);
//
//    triangle.setPoint(1, sf::Vector2f(10,0));
//    triangle.setPoint(2, sf::Vector2f(-10,0));
//    triangle.setFillColor(sf::Color::Black);
//    tekstura.draw(triangle);
//
//    tekstura.display();
//
//    sprajt.setTexture(tekstura.getTexture());
//    sprajt.setOrigin(31,31);
//
//    ////////////////////////////////////////
//    ZAPISANIE PLIKU IKONY
//    sf::Texture aaaa;
//    aaaa.create(62,62);
//    aaaa.update(tekstura.getTexture());
//    sf::Image obraz = aaaa.copyToImage();
//    obraz.saveToFile("GraSieciowa.PNG");
}

sf::Vector2f gracz::getPosition()
{
    return sprajt.getPosition();
}

sf::Vector2f gracz::getSpeed()
{
    return XYspeed;
}

void gracz::setSpeed( sf::Vector2f newSpeed )
{
    XYspeed = newSpeed;
}

void gracz::aktualizuj()
{
    sprajt.move(XYspeed);

    int x = sprajt.getPosition().x;                 // obs³uga wyjœcia poza planszê
    int y = sprajt.getPosition().y;                 // teleportacja jak w popularnej grze Snake
                                                    //
    if      ( x>600 )                               //
        sprajt.setPosition( sf::Vector2f(0,y) );    //
    else if ( x<  0 )                               //
        sprajt.setPosition( sf::Vector2f(600,y) );  //
                                                    //
    if      ( y>600 )                               //
        sprajt.setPosition( sf::Vector2f(x,0) );    //
    else if ( y<  0 )                               //
        sprajt.setPosition( sf::Vector2f(x,600) );  //
}

void gracz::patrz(unsigned short &dir)
{
    sprajt.setRotation( (dir-1) * 45 );
    direction = dir;
}

void gracz::idz(unsigned short dir)
{
    // automatyczne zwalnianie
    if( dir==11 )                   //                  12
    {                               //             02        22
        XYspeed.x *= 0.98;          //                                          2
        XYspeed.y *= 0.98;          //           01     11     21       0 1 2   1
    }                               //                                          0
    else                            //             00        20
    {                               //                  10
        int rotation;

        switch( dir )
        {
        case 12:    rotation = 0;   break;
        case 22:    rotation = 45;  break;
        case 21:    rotation = 90;  break;
        case 20:    rotation = 135; break;
        case 10:    rotation = 180; break;
        case  0:    rotation = 225; break;
        case  1:    rotation = 270; break;
        case  2:    rotation = 315; break;
        }

        XYspeed.x +=  sin(rotation * M_PI / 180.0f) * speed;  // zwiêkszanie odpowiednich sk³adowych prêdkoœci
        XYspeed.y += -cos(rotation * M_PI / 180.0f) * speed;  //

        float limit = 40 / ( (XYspeed.x*XYspeed.x) + (XYspeed.y*XYspeed.y) );  // ogranicznik prêdkoœci

        if( limit < 1 )         // przeskalowanie sk³adowych prêdkoœci, aby nie przekraczaæ ograniczenia
        {                       //
            XYspeed.x *= limit; //
            XYspeed.y *= limit; //
        }
    }
}

void gracz::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(sprajt);
}
