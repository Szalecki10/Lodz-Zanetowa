# Lodz-Zanetowa
W ramach projektu została zrealizowana łódź zanętowa wydrukowana w 3d (petg)
Na łodzi zamontowane są czujniki odległości, silnik bldc ze sterownikiem, serwo do sterowania, serwo do wyrzucenia zanęty z koszyka,
termometr.
Pilot składa się z arduino służącego za nadajnik, do którego podłączony jest potencjometr (prędkość) oraz joystick analogowy (sterowanie lewo prawo)
oraz raspberry pi pico służącego za odbiornik i przetwarząjącego dane nadane przez arduino na łodzi.
Transminsja przy pomocy nrf24l01 przebiega następująco:
arduino (pilot) nadaje sygnał sterujący
arduino (łódź) odbiera sygnał, dostraja elementy sterujące (silnik i serwo)
arduino (łódź) wykonuje pomiary (termometr, czujniki odległości) 
arduino (łódź) przełącza się w tryb nadawczy, wysyła wyniki i wraca do trybu nasłuchiwania
raspberry (pilot) odbiera wyniki pomiarów i wyświetla je na ekranie OLED.
Zastosowaliśmy między innymi SPI, onewire