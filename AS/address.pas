WEKTOR DS 10*INT
CZTERY DC INT(4)
WYNIK1 DS INT
WYNIK2 DS INT
WYNIK3 DS INT
WYNIK4 DS INT

//Adresowanie za pomocą etykiety początku tablicy - 1 element
L 0,WEKTOR
L 10,CZTERY
//Adresowanie za pomoca przesunięcie(rejestr z adresem) - 2 element
L 1,0(10)
//Adresowanie za pomocą adresu bezwzględnego - 3 element
L 2,8
A 10,CZTERY
A 10,CZTERY
//Adresowanie względem etykiety początku tablicy - 4 element
L 3,WEKTOR(10)


ST 0,WYNIK1
ST 1,WYNIK2
ST 2,WYNIK3
ST 3,WYNIK4