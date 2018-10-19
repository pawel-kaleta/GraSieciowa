class hud: public sf::Drawable, sf::Transformable
{
public:
    hud( bool ja , sf::ContextSettings &settings );

    void ran();
    void strzel();
    void aktualizuj();

    int life;
    int ammo;

private:

    int lifeMax = 50;
    int ammoMax = 30;
    float lifeBarSize = 300;
    float ammoBarSize = 300;
    sf::RectangleShape lifeBar;
    sf::RectangleShape ammoBar;

    sf::Clock reload;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

hud::hud( bool ja , sf::ContextSettings &settings )
{
    life = lifeMax;
    ammo = ammoMax;

    ammoBar.setSize( sf::Vector2f(300.0f, 2.0f) );
    lifeBar.setSize( sf::Vector2f(300.0f,20.0f) );

    ammoBar.setFillColor( sf::Color::Yellow );

    if( ja )
    {
        lifeBar.setFillColor( sf::Color(0,180,0) );

        ammoBar.setPosition( sf::Vector2f(0.0f,600.0f) );
        lifeBar.setPosition( sf::Vector2f(0.0f,602.0f) );
    }
    else
    {
        lifeBar.setFillColor( sf::Color::Red );

        ammoBar.setPosition( sf::Vector2f(300.0f,600.0f) );
        lifeBar.setPosition( sf::Vector2f(300.0f,602.0f) );
    }
}

void hud::ran()
{
    life--;
    lifeBarSize = 300 * life / lifeMax;
    lifeBar.setSize( sf::Vector2f(lifeBarSize,20) );
}

void hud::strzel()
{
    ammo--;
    ammoBarSize = 300 * ammo / ammoMax;
    ammoBar.setSize( sf::Vector2f(ammoBarSize,2) );
}

void hud::aktualizuj()
{
    if( reload.getElapsedTime() > sf::seconds(0.3f))
    {
        if(ammo<ammoMax)
            ammo++;
        ammoBarSize = 300 * ammo / ammoMax;
        ammoBar.setSize( sf::Vector2f(ammoBarSize,2) );
        reload.restart();
    }
}

void hud::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(lifeBar);
    target.draw(ammoBar);
}
