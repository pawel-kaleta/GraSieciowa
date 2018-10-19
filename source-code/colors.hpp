#include <math.h>
#include <stdio.h>

using namespace std;

sf::Vector3f XYtoHSV( sf::Vector2i position, int &r)
{
    // algorytm konwertuj�cy pozycj� w uk�adzie wsp�rz�dnych na kolor w przestrzeni HSV
    float H,S,V;
    int x,y;

    x = position.x - 300;   // przesuni�cie odniesienia uk�adu wsp�rz�dnych na �rodek planszy
    y = position.y - 300;   //

    H = atan2(y,x) * 180.0f/M_PI + 180+r;  // wyliczenie k�ta - odcienia koloru
    if(H>360)
        H-=360;

    S = sqrt( x*x + y*y );  // wyliczenie odleg�o�ci od �rodka planszy - nasycenia koloru

    if( S > 300.0f ) // plansza jest kwadratem, a HSV to ko�o (sto�ek), wi�c jak wychodz� poza ko�o musz� zmodyfikowa� warto�ci, aby pozosta� w dopuszczalnych zakresach
    {
        V = 1-((S-300) / 124.264f);  // zmniejszam jasno�� poza ko�em stopniowo a� do zera w samym rogu planszy (124.264 to odleg�o�� rogu planszy od ko�a), przeliczam warto�� na zakres 0-1
        S = 1.0f;  // nasycenie pe�ne
    }
    else
    {
        V = 1;  // jasno�� pe�na
        S = S / 300.0f;  // przeliczenie nasycenia na zakres 0-1
    }

    return sf::Vector3f(H,S,V);
}

sf::Vector3i HSVtoRGB(sf::Vector3f HSV)
{
    // jako �e biblioteka operuje na kolorach definiowanych w RGB, dodatkowy algorytm konwertuj�cy
    float H,S,V,R,G,B;

    H = HSV.x;
    S = HSV.y;
    V = HSV.z;

    H /=60;             // wyliczenie fragmentu ko�a barw, w kt�rym jeste�my
    int i = floor(H);   //

    float f = H-i;  // wyliczenie jak daleko w nim jeste�my

    float p = V * (1-S);            // wyliczenie warto�ci dla sk�adowych koloru
    float q = V * (1-(S*f));        //
    float t = V * (1-(S*(1-f)));    //

    if      (i==0)   { R=V; G=t; B=p; }   // nadanie tych warto�ci odpowiednim sk�adowym koloru
    else if (i==1)   { R=q; G=V; B=p; }   // na podstawie fragmentu ko�a, w kt�rym jeste�my
    else if (i==2)   { R=p; G=V; B=t; }   //
    else if (i==3)   { R=p; G=q; B=V; }   //
    else if (i==4)   { R=t; G=p; B=V; }   //
    else if (i==5)   { R=V; G=p; B=q; }   //

    int r = R*255;  // przeliczenie z zakresu 0-1 na 0-255
    int g = G*255;  //
    int b = B*255;  //

    return sf::Vector3i(r,g,b);
}


