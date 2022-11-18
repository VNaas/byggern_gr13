Kontrolleren ligger i PID_controller.c
Motor_init i motor driver skal kjøre greia til endepunktene og gjøre en måling for å lese av maksverdi
for å kunne skalere målinger.
For å fikse PIDen, sjekk at matematikken serbra ut, og tune på parametrene.Hvis du får til å printe ting fra node2 kan det hjelpe.
Hvis ikke printing fungerer kan man bli kreativ med LED-lysene.

Noen ganger låser OLED skjermen seg. Da må du ta ut strømmen til joystick-brettet, sette inn igjen, trykke reset på joystick-brettet, og så
trykke manuell reset på breadboeardet. Hvis dette ikke hjelper prøv å flashe koden på nytt til breadboardet.