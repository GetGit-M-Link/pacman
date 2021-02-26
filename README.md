# Prüfung: „Einführung in die Softwareentwicklung“
## Instruktionen zum Bauen des Projektes
-Lies sich bei sowohl aus KDevelop heraus, als auch in QTCreator als auch in der commandline mit CMake problemlos kompilieren
QTCreator sagt : Based on Qt 5.15.2 (GCC 10.2.0, 64 bit)

-Developed on Linux


## Notizen an die Korrigierenden
- Ich habe mich entschieden dass die Geister am Anfang auf einem Punkt stehen. Somit gibt es einen Punkt mehr in meiner Version.

Ausbaustufe 2:

    -Es ist möglich zwischen drei Karten im Menü auszuwählen

    - Mit q kann das Programm jederzeit beendet werden (nach Bestätigung) während der Abfrage wird der Timer pausiert und 
      falls man doch weiterspielt wieder gestartet

    - Man kann im Menü die Schwierigkeitsstufe wählen

    - Am Ende der Runde wird angeboten ein neues Spiel zu beginnen, dabei wird die Zeit gespeichert um dann
    
    - in der nächsten Runde als Bestzeit zu gelten die es zu schlagen gilt (nur solange man nochmal Spielen bestätigt, geht man ins
      Menü wird die Bestzeit entfernt, weil man ja dann theoretisch einen anderen Level aus der Textdatei laden könnte)

Bonuspunkte:
    nach 5 Sekunden im Level spawn eine Unverwundbarkeitspille (grün)
    das geschieht nach der rrnd Funktion aus der ConsoleDemo an einem Punkt an dem zu Levelbeginn ein Punkt war.
    Wenn ein Geist über sie drüber schwebt wird sie zerstört (Punkte unter der Pille werden wieder hergestellt).
    Wenn Pacman sie isst kann er die Geister fressen (diese verschwinden von der Karte).
    Die "schlauen Geister" laufen dabei aktiv weg, bei den "dummen Geistern" habe ich mich bewusst dafür entschieden, dass sie auch
    zufällig "weglaufen" ihre Bewegung ändert sich also nicht.
    Nach 5 Sekunden hört die Wirkung der Pille auf und die Geister jagen wieder Pacman.


## Quellen
- https://www.techiedelight.com/convert-vector-chars-std-string/
- https://www.cplusplus.com/reference/string/string/find/ 
