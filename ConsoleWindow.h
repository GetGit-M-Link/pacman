#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H

#include <QMainWindow>
#include <qwidget.h>
#include <vector>
#include <string>
#include <functional>


/// Ein einfaches Konsolenfenster. Es unterstützt die folgenden Features,
/// welche bei der Standard-Console (std::cin/std::cout) nicht plattformübergreifend verfügbar sind:
/// - Wahlfreier ("random access") Zugriff auf Zeichen im Fenster (man kann an jede Koordinate schreiben)
/// - Asynchrone Abfrage, ob eine Taste gedrückt ist (ohne auf Benutzer warten zu müssen).
///
/// Achtung: Die Architektur ist Ereignisorientiert!
/// - Der Bildschirm wird regelmäßig aktualisiert (Vorgabe: 20x pro Sekunde)
/// - Vor jedem Neuzeichnen wird die Methode "onRefresh()" aufgerufen.
///   Hier muss der Code für die Anwendungslogik integriert werden.
/// - Über set/getCharacter kann Zeichenweise auf die Konsole geschrieben werden (Format: Spalte, Zeile, Zeichen)
/// - Mit getPressedKey() kann man abfragen, welche Taste gerade gedrückt ist.
///
/// Alle "Zeichen" sind einfache ASCII-Zeichen im Bereich 32-126 (keine Steuerzeichen).
/// Für die Cursortasten gibt es spezielle Codes CURSOR_LEFT,...,CURSOR_UP, die für
/// getPressedKey() genutzt werden könnne (für set/getCharacter macht das keinen Sinn).
///
class ConsoleWindow : public QMainWindow
{
    Q_OBJECT

 private:
    /// Private Hilfsklasse, intern - kann man ignorieren
    class ConsoleWidget : public QWidget {
     private:
        /// speichert alle die Zeichen, die dargestellt werden als 1D array (x + width*y Format)
        std::vector<char> textBuffer;

        /// Breite des 2D Arrays
        unsigned width;
        /// Höhere des 2D Arrays
        unsigned height;
        /// Fontgröße in Pixeln
        unsigned charSize;
        /// zuletzt gedrückte Taste als ASCII-CODE (inkl. eigene Cursorcodes)
        char lastKey;
        /// interne Delegationsvariable für direkte key-press-events
        std::function<void()> onKeyPress;

     protected:
        /// überschriebener Eventhandler für Tastatur (press down)
        void keyPressEvent(QKeyEvent *event) override;
        /// überschriebener Eventhandler für Tastatur (release)
        void keyReleaseEvent(QKeyEvent *event) override;
        /// überschriebener Eventhandler Paint-Events (Zeichnen der Konsole)
        void paintEvent(QPaintEvent *event) override;

     public:
        ConsoleWidget(std::function<void()> onKeyPressFunc, QWidget *parent = nullptr, unsigned width = 64, unsigned height = 48, unsigned charSize = 12);
        void setCharSize(unsigned charSize);
        inline void setCharacter(int x, int y, char character);
        inline char getCharacter(int x, int y);
        inline unsigned getWidth() { return width; }
        inline unsigned getHeight() { return height; }
        inline char getPressedKey() { return lastKey; }
    };

    /// Das Konsolen Widget
    ConsoleWidget *console;
    /// Dieser Timer sended ca. 20 Events pro Sekunde für die "onRefresh"-Events
    QTimer *refreshTimer;
    /// Menü-Objekt
    QMenu *fileMenu;
    /// Menü-Objekt
    QMenu *viewMenu;
    /// Menü-Objekt / Aktion
    QAction *exitAct;
    /// Menü-Objekt / Aktion
    QAction *smallAct;
    /// Menü-Objekt / Aktion
    QAction *mediumAct;
    /// Menü-Objekt / Aktion
    QAction *largeAct;
    /// intern - für key events
    void keyPressNotification();

 private slots:
    /// intern - wird bei Timerevents aufgerufen
    void refreshTimerTimeout();
    /// intern - File/Exit Menü
    void onExit();
    /// intern - View/smallFont
    void onSmallFont();
    /// intern - View/mediumFont
    void onMediumFont();
    /// intern - View/largeFont
    void onLargeFont();

 protected:
    /// Diese Funktion wird ca. 20-mal pro Sekunde aufgerufen
    /// Nach jedem Aufruf wird der Inhalt des internen Puffers auf den Bildschirm übertragen.
    /// Die eigene "Game-Loop" der Anwendung sollte hier eingebaut werden.
    /// Wichtig: Die Funktion muss schnell wieder beendet werden (unter 50ms)
    /// um eine glatte Animation zu erhalten. Zwischenergebnisse muss man entsprechend
    /// in Membervariablen zwischenspeichern.
    ///
    virtual void onRefresh() = 0;

    /// Optional: Diese Methode wird sofort aufgerufen, wenn eine Taste gedrückt wurde (unabhängig vom Timer).
    /// Dies kann dabei helfen, das Spiel flüssiger zu gestalten.
    ///
    virtual void onKeyPress() {}

 public:
    /// Constructor: Erzeugt ein neues Window mit vorgegebener Breite und Höhe
    ConsoleWindow(QWidget *parent = nullptr, unsigned width = 64, unsigned height = 48);

    unsigned inline getWidth() { return console->getWidth(); }
    unsigned inline getHeight() { return console->getHeight(); }

    /// Setzt das Zeichen an Koordinate (x,y) (=Spalte, Zeile) auf einen gegebenen Wert.
    /// Das Zeichen selbst muss ein ASCII-Zeichen sein, also im Bereich Dezimal 32...126
    /// Unicode (UTF-8 oder ähnliches) wird nicht unterstützt
    /// Zeichen unter 32 (Leerzeichen) oder über 126 ('~') werden auf Leerzeichen (' ') abgebildet
    /// Es ist möglich, char-Konstanten wie 'a', '*' oder '<' zu benutzen, um Zeichen zu spezifizieren.
    /// Die x,y-Koordinaten werden "geclippt", d.h., es passiert nichts, falls sie den zulässigen
    /// Wertebereich [0,...,width-1], [0,...,height-1] verlassen.
    void setCharacter(int x, int y, char character);

    /// Ließt das Zeichen an Spalte x und Zeile y aus.
    /// Die x,y Koordinaten werden wieder "geclippt": falls x,y außerhalb des
    /// zulässigen Wertebereichs liegen, gibt die Funktion 0 zurück
    char getCharacter(int x, int y);

    /// Schreibt einen längeren String auf die Konsole, startend bei x,y
    /// Zeichen außerhalb der sichtbaren Konsole werden "geclipped" (also abgeschnitten, kein Fehler)
    void writeString(int x, int y, std::string text);

    /// Löscht die Konsole, Standardmäßig mit Leerzeichen
    void clear(char character = ' ');

    /// Gibt die Taste zurück, die als letztes heruntergedrückt wurde
    /// (und noch nicht wieder losgelassen wurde).
    /// Die Funktion gibt nur ASCII-Zeichen zwischen 32...126 zurück,
    /// (fast) alles andere wird ignoriert.
    /// Ausnahme: Cursortasten. Hier werden spezielle Codes
    /// zurückgegeben (siehe unten).
    /// Falls gar keine Taste gedrückt wurde, gibt die Funktion null zurück.
    char getPressedKey();

    /// spezieller Code für Cursor-links
    static const char CURSOR_LEFT = 1;
    /// spezieller Code für Cursor-rechts
    static const char CURSOR_RIGHT = 2;
    /// spezieller Code für Cursor-hoch
    static const char CURSOR_UP = 3;
    /// spezieller Code für Cursor-runter
    static const char CURSOR_DOWN = 4;
    /// der Vollständigkeit halber: gar keine Taste gedrückt
    static const char NO_KEY = 0;

    /// Destruktor räumt auf
    ~ConsoleWindow() override;
};


#endif // CONSOLEWINDOW_H
