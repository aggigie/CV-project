# CV-project
>Widzenie komputerowe, semestr letni 2020

Napisać program, który umożliwi wykrycie czy w filmie:

a) nie ma czarnych marginesów (pojawiają się w starych filmach, których rozdzielczość jest niedopasowana
do rozdzielczości stacji telewizyjnej)

b) pojawiają się nie zmieniające fragmenty w kolejnych klatkach (tu można zastosować porównanie
histogramów, najlepiej dla podzielonych fragmentów klatek)

c) zakłócenia, np. puste lub uszkodzone klatki 

Opis metod:
* `<void StillFrameDetector::Detect(const Mat& inp, Mat& out)>`

Metoda wczytuje klatkę z filmu, następnie dzieli ją na kawałki (liczba kawałków regulowana parametrem). 
Dla każdego takiego kawałka wyliczane zostają histogramy (dla każdego koloru RGB). Przy analizowaniu kolejnej klatki, histogramy 
porównywane są z histogramami poprzedniej klatki. O tym, czy pojawiają się zmieniające się fragmenty decyduje suma ilości różnic w kolorach dla każdego histogramu. 

* `<void BlackBarsFrameDetector::Detect(const Mat& inp, Mat& out)>`
Metoda wczytuje klatkę z filmu, następnie binaryzuje ją. Wyliczana zostaje największy dostępny kontur, który ogranicza pole bez czarnego tła. 

* `<void CorruptedFrameDetector::Detect(const Mat& inp, Mat& out)>`
Metoda wyłapuje z klatki potencjalne zakłócenia: szum, prześwietlenie. Następnie binaryzuje klatkę i wylicza liczbę wykrytych obiektów. Porównuje z oryginalną wersją i decyduje, czy 
dana klatka była uszkodzona.
