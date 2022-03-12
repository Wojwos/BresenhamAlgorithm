# Prosta biblioteka graficzna
### Analiza zadania w tym uzasadnienie wyboru rozwiązania 
Zadanie polega na obliczaniu pikseli pomiędzy współrzędnymi. Do wykonania tego typu obliczeń najrozsądniejszym wyborem jest wykorzystanie algorytmu Bresenhama. Polega on na dokładnym obliczeniu punktów pomiędzy dwiema współrzędnymi bez żadnych przerw, dzięki temu jesteśmy w stanie otrzymać wyraźne linie proste. W algorytmie ważne jest uzyskanie odpowiednich współczynników określających położenie punktów między sobą w zależności czy linia będzie biegła pod kątem większym lub mniejszym niż 45 stopni oraz w dół/górę lub lewo/prawo.

Podział na wątki realizujemy przydzielając kolejno dwa punkty do każdej funkcji. Jeśli punktów jest mniej niż wątków, to ilość wątków zostanie zmniejszona do liczby punktów minus jeden. Powodem wyboru takiego podziału jest budowa funkcji wykonującej obliczenia, do której przekazujemy kolejno: wskaźnik na tablicę 400 na 400, współrzędną x1, y1, x2, y2.

### Opis założeń z rozbiciem na założenia części głównej programu oraz funkcje biblioteki 
Program główny otrzymuje współrzędne z wybranego pliku tekstowego kolejno x, y. Do wykonania potrzebuje podania ilości wątków (1-64) oraz wybór biblioteki (1-asm, 0-c++). Po podaniu poprawnych danych w oknie zostaną narysowane linie proste utworzone ze współrzędnych. Podany jest również czas wykonania obliczeń (bez rysowania).
Biblioteki obliczają piksele pomiędzy współrzędnymi.
Biblioteka wysokiego poziomu napisana w C++.
Biblioteka niskiego poziomu napisana w asemblera x64.

### Funkcje bibliotek dll C++ i asm i ich parametry oraz używane rejestry
Do funkcji przekazywany jest wskaźnik na tablicę dwuwymiarową 400 na 400 typu bool wypełnioną 0, współrzędne x1, y1, x2, y2. Funkcja to implementacja algorytmu Bresenhama. Podczas jej wykonywania dane w tablicy są modyfikowane, na podstawie współrzędnej x i y do tablicy wpisujemy 1 jeśli jest to piksel obliczanej linii.
Dane znajdują się w rejestrach:
-cx - wskaźnik na tablice
-dx - x1
-r8 - y1
-r9 - x2
Ostatni parametr y2 nie może być przekazany do kolejnego rejestru, dlatego y2 pobieramy ze stosu [rsp + 40]

### Dane wejściowe
Program pobiera dane z pliku txt podanego przez użytkownika. Dane w pliku muszą być podane w kolejności x y. Współrzędne muszą być podane z przedziału od 0 do 400, w innym przypadku dany punkt zostanie pominięty.
>**Przykładowy plik wejściowy:**
>10 10
>50 0
>150 50
>0 200

### Opis uruchamiania programu
Program jest prosty w obsłudze, należy wybrać plik tekstowy jaki nas interesuje, następnie podajemy ilość wątków (od 1 do 64) i wybieramy bibliotekę za pomocą wpisania numeru 1 lub 0. Ostatnim działaniem jest wciśnięcie przycisku rysuj, który wyświetli nam utworzony obraz.
