#include <math.h>
#include <stdio.h>

using namespace std;

sf::Vector3f XYtoHSV( sf::Vector2i position, int &r)
{
    // algorytm konwertuj¹cy pozycjê w uk³adzie wspó³rzêdnych na kolor w przestrzeni HSV
    float H,S,V;
    int x,y;

    x = position.x - 300;   // przesuniêcie odniesienia uk³adu wspó³rzêdnych na œrodek planszy
    y = position.y - 300;   //

    H = atan2(y,x) * 180.0f/M_PI + 180+r;  // wyliczenie k¹ta - odcienia koloru
    if(H>360)
        H-=360;

    S = sqrt( x*x + y*y );  // wyliczenie odleg³oœci od œrodka planszy - nasycenia koloru

    if( S > 300.0f ) // plansza jest kwadratem, a HSV to ko³o (sto¿ek), wiêc jak wychodzê poza ko³o muszê zmodyfikowaæ wartoœci, aby pozostaæ w dopuszczalnych zakresach
    {
        V = 1-((S-300) / 124.264f);  // zmniejszam jasnoœæ poza ko³em stopniowo a¿ do zera w samym rogu planszy (124.264 to odleg³oœæ rogu planszy od ko³a), przeliczam wartoœæ na zakres 0-1
        S = 1.0f;  // nasycenie pe³ne
    }
    else
    {
        V = 1;  // jasnoœæ pe³na
        S = S / 300.0f;  // przeliczenie nasycenia na zakres 0-1
    }

    return sf::Vector3f(H,S,V);
}

sf::Vector3i HSVtoRGB(sf::Vector3f HSV)
{
    // jako ¿e biblioteka operuje na kolorach definiowanych w RGB, dodatkowy algorytm konwertuj¹cy
    float H,S,V,R,G,B;

    H = HSV.x;
    S = HSV.y;
    V = HSV.z;

    H /=60;             // wyliczenie fragmentu ko³a barw, w którym jesteœmy
    int i = floor(H);   //

    float f = H-i;  // wyliczenie jak daleko w nim jesteœmy

    float p = V * (1-S);            // wyliczenie wartoœci dla sk³adowych koloru
    float q = V * (1-(S*f));        //
    float t = V * (1-(S*(1-f)));    //

    if      (i==0)   { R=V; G=t; B=p; }   // nadanie tych wartoœci odpowiednim sk³adowym koloru
    else if (i==1)   { R=q; G=V; B=p; }   // na podstawie fragmentu ko³a, w którym jesteœmy
    else if (i==2)   { R=p; G=V; B=t; }   //
    else if (i==3)   { R=p; G=q; B=V; }   //
    else if (i==4)   { R=t; G=p; B=V; }   //
    else if (i==5)   { R=V; G=p; B=q; }   //

    int r = R*255;  // przeliczenie z zakresu 0-1 na 0-255
    int g = G*255;  //
    int b = B*255;  //

    return sf::Vector3i(r,g,b);
}


