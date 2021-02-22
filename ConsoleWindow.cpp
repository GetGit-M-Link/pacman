#include "ConsoleWindow.h"

#include <QKeyEvent>
#include <QPainter>
#include <QHBoxLayout>
#include <QFontDatabase>
#include <QTimer>
#include <QFrame>
#include <QMenu>
#include <QMenuBar>

// --- ConsoleWindow::ConsoleWidget

/// convertes qt keyboard event to ascii code (using special values of class ConsoleWindow)
char convert_2_ascii(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up)
        return ConsoleWindow::CURSOR_UP;
    if (event->key() == Qt::Key_Down)
        return ConsoleWindow::CURSOR_DOWN;
    if (event->key() == Qt::Key_Left)
        return ConsoleWindow::CURSOR_LEFT;
    if (event->key() == Qt::Key_Right)
        return ConsoleWindow::CURSOR_RIGHT;
    const QString &str = event->text();
    if (str.count() <= 0)
        return 0;
    char result = str[0].toLatin1();
    if (result < 32)
        return 0;
    if (result > 126)
        return 0;
    return result;
}


void ConsoleWindow::ConsoleWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;
    lastKey = convert_2_ascii(event);
    event->accept();
    onKeyPress();
}

void ConsoleWindow::ConsoleWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;
    lastKey = 0;
    std::ignore = event;
    event->accept();
}

void ConsoleWindow::ConsoleWidget::paintEvent(QPaintEvent *event)
{
    std::ignore = event;
    QPainter painter(this);
    QRect r = geometry();
    r.setX(0);
    r.setY(0);
    painter.fillRect(r, QColor(0, 0, 0));
    QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    fixedFont.setPixelSize(int(charSize));
    painter.setFont(fixedFont); // QFont("Consolas", int(charSize))
    painter.setPen(Qt::white);
    QFontMetrics metrics(fixedFont);
    int offset = metrics.ascent(); // vertical distance to baseline - for placement in y-direction
    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
            painter.drawText(int(x * charSize),
                             int(y * charSize) + offset,
                             QString(textBuffer[x + width * y]));
        }
    }
}

ConsoleWindow::ConsoleWidget::ConsoleWidget(std::function<void()> onKeyPressFunc,
                                            QWidget *parent,
                                            unsigned width,
                                            unsigned height,
                                            unsigned charSize)
    : QWidget(parent), // parent constructor
      textBuffer(width * height,
                 ' '), // textBuffer constructor - std::vector<char>(initial_size, initial_value)
    onKeyPress(onKeyPressFunc)
{
    this->width = width;
    this->height = height;
    this->charSize = charSize;
    setMinimumWidth(int(width * charSize));
    setMaximumWidth(int(width * charSize));
    setMinimumHeight(int(height * charSize));
    setMaximumHeight(int(height * charSize));
    lastKey = 0;
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void ConsoleWindow::ConsoleWidget::setCharSize(unsigned charSize)
{
    this->charSize = charSize;
    setMinimumWidth(int(width * charSize));
    setMaximumWidth(int(width * charSize));
    setMinimumHeight(int(height * charSize));
    setMaximumHeight(int(height * charSize));
    update();
}

void ConsoleWindow::ConsoleWidget::setCharacter(int x, int y, char character)
{
    if (x < 0 || x >= int(width) || y < 0 || y >= int(height)) {
        return;
    }
    if (character < ' ' || character > 126) {
        character = ' ';
    }
    textBuffer[static_cast<size_t>(x + int(width) * y)] = character;
}

char ConsoleWindow::ConsoleWidget::getCharacter(int x, int y)
{
    if (x < 0 || x >= int(width) || y < 0 || y >= int(height))
        return 0;
    return textBuffer[static_cast<size_t>(x + int(width) * y)];
}


// --- ConsoleWindow


void ConsoleWindow::refreshTimerTimeout()
{
    onRefresh();
    update();
}

void ConsoleWindow::onExit()
{
    close();
}

void ConsoleWindow::onSmallFont()
{
    console->setCharSize(8);
    centralWidget()->adjustSize();
    adjustSize();
}

void ConsoleWindow::onMediumFont()
{
    console->setCharSize(16);
    centralWidget()->adjustSize();
    adjustSize();
}

void ConsoleWindow::onLargeFont()
{
    console->setCharSize(32);
    centralWidget()->adjustSize();
    adjustSize();
}

void ConsoleWindow::keyPressNotification()
{
    onKeyPress();
}

ConsoleWindow::ConsoleWindow(QWidget *parent, unsigned width, unsigned height) : QMainWindow(parent)
{
    QFrame *frame = new QFrame(this);
    frame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setCentralWidget(frame);
    console = new ConsoleWidget(std::bind(&ConsoleWindow::keyPressNotification, this), frame, width, height);
    QHBoxLayout *layout = new QHBoxLayout(frame);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(console);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    refreshTimer = new QTimer(this);
    refreshTimer->setInterval(50);
    refreshTimer->setSingleShot(false);
    QObject::connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refreshTimerTimeout()));
    refreshTimer->start();
    console->setFocus();

    fileMenu = menuBar()->addMenu("&File");
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip("close the window");
    connect(exitAct, &QAction::triggered, this, &ConsoleWindow::onExit);
    fileMenu->addAction(exitAct);

    viewMenu = menuBar()->addMenu("&View");
    smallAct = new QAction(tr("&Small Font"), this);
    smallAct->setStatusTip("use 8px font");
    connect(smallAct, &QAction::triggered, this, &ConsoleWindow::onSmallFont);
    viewMenu->addAction(smallAct);
    mediumAct = new QAction(tr("&Medium Font"), this);
    mediumAct->setStatusTip("use 16px font");
    connect(mediumAct, &QAction::triggered, this, &ConsoleWindow::onMediumFont);
    viewMenu->addAction(mediumAct);
    largeAct = new QAction(tr("&Large Font"), this);
    largeAct->setStatusTip("use 32px font");
    connect(largeAct, &QAction::triggered, this, &ConsoleWindow::onLargeFont);
    viewMenu->addAction(largeAct);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void ConsoleWindow::setCharacter(int x, int y, char character)
{
    console->setCharacter(x, y, character);
}

char ConsoleWindow::getCharacter(int x, int y)
{
    return console->getCharacter(x, y);
}

void ConsoleWindow::writeString(int x, int y, std::string text)
{
    int l = int(text.size());
    for (int i = 0; i < l; i++) {
        console->setCharacter(x + i, y, text[size_t(i)]);
    }
}

void ConsoleWindow::clear(char character)
{
    int width = int(getWidth());
    int height = int(getHeight());
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            console->setCharacter(x, y, character);
        }
    }
}

char ConsoleWindow::getPressedKey()
{
    return console->getPressedKey();
}


ConsoleWindow::~ConsoleWindow()
{
    delete console;
}


