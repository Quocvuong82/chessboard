#include "qboard.h"
#include <QPainter>
#include <string>
#include <boost/lexical_cast.hpp>
#include "qsquare.h"
using namespace std;

QBoard::QBoard(QWidget *parent) : QWidget(parent), Board()
{

    /* Create the 64 squares of the board */
    QPixmap square_d = QPixmap(QSquare::QSquare::PATH + "textures/wood_d.png");
    QPixmap square_l = QPixmap(QSquare::PATH + "textures/wood_l.png");

    int i;
    initSquares();
    Grid = new QGridLayout;
    Grid->setHorizontalSpacing(0);
    Grid->setVerticalSpacing(0);
    Slider = new QSlider(Qt::Horizontal);
    Slider->setMinimum(0);
    connect(Slider, SIGNAL(sliderMoved(int)), this, SLOT(setPosition(int)));

    /* Board Labeling */

    /* Letters */
    int minWidth = 0;
    for (int i = 1; i < 9; i++) {
        QLabel* l = new QLabel;
        string str = boost::lexical_cast<string>(i);
        str[0] = str[0] + 16;
        l->setText(QString::fromStdString(str));
        l->setAlignment(Qt::AlignCenter);
        Grid->addWidget(l, 9, i);
    }
    for (int i = 1; i < 9; i++) {
        QLabel* l = new QLabel;
        string str = boost::lexical_cast<string>(i);
        str[0] = str[0] + 16;
        l->setText(QString::fromStdString(str));
        l->setAlignment(Qt::AlignCenter);
        Grid->addWidget(l, 0, i);
    }

    /* Numbers */
    for (int i = 1; i < 9; i++) {
        QLabel* l = new QLabel;
        l->setText(QString::fromStdString(boost::lexical_cast<string>(9 - i)));
        l->setAlignment(Qt::AlignCenter);
        Grid->addWidget(l, i, 0);
    }
    for (int i = 1; i < 9; i++) {
        QLabel* l = new QLabel;
        l->setText(QString::fromStdString(boost::lexical_cast<string>(9 - i)));
        l->setAlignment(Qt::AlignCenter);
        Grid->addWidget(l, i, 9);
    }

    /* Board Squares */
    for (int y = 0; y < 8; y++) {
       for (int x = 0; x < 8; x++) {
           i = y * 8 + x;
           Grid->addWidget(squares[i], 8 - y, x + 1);
       }
    }

    Grid->setMargin(0);

    /*initialPosition = pos;
    currentPosition = &initialPosition;
    position = pos;

    parent = 1;
    castleWK = true;
    castleWQ = true;
    castleBK = true;
    castleBQ = true;*/
    //currentPosition = new Position();
    writePositionTosquares();
}

void QBoard::initSquares() {
    square_d = QPixmap(QSquare::PATH + "textures/wood_d.png");
    square_l = QPixmap(QSquare::PATH + "textures/wood_l.png");

    int scale = 64;

    for(int i = 0; i < 12; i++) {
        pieces[i] = QPixmap::fromImage(QImage(scale,scale, QImage::Format_ARGB32));
        pieces[i].fill(Qt::transparent);
        pieces[i].scaled(scale,scale);
    }

    for(int i = 0; i < 12; i++) {
        piecesSVG[i] = new QSvgWidget;
        piecesSVG[i]->setStyleSheet("background: transparent");
    }
    piecesSVG[0]->load(QString (QSquare::PATH + "textures/WhitePawn.svg"));
    piecesSVG[1]->load(QString (QSquare::PATH + "textures/WhiteRook.svg"));
    piecesSVG[2]->load(QString (QSquare::PATH + "textures/WhiteKnight.svg"));
    piecesSVG[3]->load(QString (QSquare::PATH + "textures/WhiteBishop.svg"));
    piecesSVG[4]->load(QString (QSquare::PATH + "textures/WhiteQueen.svg"));
    piecesSVG[5]->load(QString (QSquare::PATH + "textures/WhiteKing.svg"));
    piecesSVG[6]->load(QString (QSquare::PATH + "textures/BlackPawn.svg"));
    piecesSVG[7]->load(QString (QSquare::PATH + "textures/BlackRook.svg"));
    piecesSVG[8]->load(QString (QSquare::PATH + "textures/BlackKnight.svg"));
    piecesSVG[9]->load(QString (QSquare::PATH + "textures/BlackBishop.svg"));
    piecesSVG[10]->load(QString (QSquare::PATH + "textures/BlackQueen.svg"));
    piecesSVG[11]->load(QString (QSquare::PATH + "textures/BlackKing.svg"));

    for(int i = 0; i < 12; i++) {
        piecesSVG[i]->resize(scale,scale);
        painter.push_back(new QPainter(&pieces[i]));
        piecesSVG[i]->render(painter[i], QPoint(), QRegion(), QWidget::DrawChildren);
    }
    int i;
    for (int y = 0; y < 8; y++) {
       for (int x = 0; x < 8; x++) {
           i = y * 8 + x;
           squares.append(new QSquare);
           if((y + x) % 2 == 0) squares[i]->setPixmap(square_d); else
               squares[i]->setPixmap(square_l);
               squares[i]->setFixedSize(scale,scale);
       }
    }

    /* Handle Dropped- and Clicked-Signals */
    for(int j = 0; j < squares.size(); j++) {
        connect(squares[j], SIGNAL(clicked(int)), this, SLOT(squareClicked(int)));
        connect(squares[j], SIGNAL(dropped(int, int)), this, SLOT(squareDropped(int, int)));
    }
}


void QBoard::writePositionTosquares() {
    QString fenstr;
    for (int i = 7; i >= 0; i--) {
        fenstr.append(QString::fromStdString(currentPosition->getRow(i)));
    }
    //cout << "created QString fenstr" << endl;
    //cout << fenstr.toStdString() << endl;
    //for (int i = 0; i < 64; i++) {
    //cout << fenstr.toStdString();
    int i;
     for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            i = y * 8 + x;
            switch(fenstr[i].unicode()) {
            case static_cast<int>('P'):
                squares[i]->setPixmap(pieces[0]);
                break;
            case static_cast<int>('R'):
                squares[i]->setPixmap(pieces[1]);
                break;
            case static_cast<int>('N'):
                squares[i]->setPixmap(pieces[2]);
                break;
            case static_cast<int>('B'):
                squares[i]->setPixmap(pieces[3]);
                break;
            case static_cast<int>('Q'):
                squares[i]->setPixmap(pieces[4]);
                break;
            case static_cast<int>('K'):
                squares[i]->setPixmap(pieces[5]);
                break;
            case static_cast<int>('p'):;
                squares[i]->setPixmap(pieces[6]);
                break;
            case static_cast<int>('r'):
                squares[i]->setPixmap(pieces[7]);
                break;
            case static_cast<int>('n'):;
                squares[i]->setPixmap(pieces[8]);
                break;
            case static_cast<int>('b'):
                squares[i]->setPixmap(pieces[9]);
                break;
            case static_cast<int>('q'):
                squares[i]->setPixmap(pieces[10]);
                break;
            case static_cast<int>('k'):
                squares[i]->setPixmap(pieces[11]);
                break;
            default:;
                if(fenstr[i] == '0' || fenstr[i] == '-') {
                    QPixmap empty = QPixmap::fromImage(QImage(64,64, QImage::Format_ARGB32));
                    empty.fill(Qt::transparent);
                    squares[i]->setPixmap(empty);
                }
            }
            //squares[i]->setText(QString (fenstr[i]));
        }
    }
     //cout << endl;
    //cout << "wrote Position to Squares" << endl;

}

void QBoard::nextPos() {
    Board::nextPos();
    writePositionTosquares();
    Slider->setValue(moveNr);
}

void QBoard::nextPos(int index) {
    Board::nextPos(index);
    Slider->setValue(moveNr);
}

void QBoard::prevPos() {
    Board::prevPos();
    writePositionTosquares();
    Slider->setValue(moveNr);
}

void QBoard::loadFile(string filename) {
    Board::loadFile(filename);
    Slider->setMaximum(NrofMoves);
}

void QBoard::setPosition(int id) {
    //cout << moveNr << " " << id << endl;
    if(id > NrofMoves) return;
    moveNr = id;
    currentPosition = positions[id];
    writePositionTosquares();
}

void QBoard::show() {
    writePositionTosquares();
}

bool QBoard::setPosition(Fen pos) {

    if(Board::setPosition(pos)) {
        string movecmd = currentPosition->getMove();
        cout << "Move: " << movecmd << endl;
        if(movecmd.length() > 0) {
            /* Highlight Squares */
            for(int i = 0; i < squares.size(); i++) {
                squares[i]->highlight(false);
            }
            int x = static_cast<int>(movecmd[2]) - 97;
            int y = 8 - (static_cast<int>(movecmd[3]) - 48);
            int x_origin = static_cast<int>(movecmd[0]) - 97;
            int y_origin = 8 - (static_cast<int>(movecmd[1]) - 48);
            int i = (7-y) * 8 + x;
            int j = (7-y_origin) * 8 + x_origin;
            squares[i]->highlight(true);
            squares[j]->highlight(true);
        }

        Slider->setMaximum(NrofMoves);
        Slider->setValue(NrofMoves);
        emit madeMove();
    }
}

void QBoard::move(string movecmd) {
    Board::move(movecmd);

    /* Highlight Squares */
    for(int i = 0; i < squares.size(); i++) {
        squares[i]->highlight(false);
    }
    int x = static_cast<int>(movecmd[2]) - 97;
    int y = 8 - (static_cast<int>(movecmd[3]) - 48);
    int x_origin = static_cast<int>(movecmd[0]) - 97;
    int y_origin = 8 - (static_cast<int>(movecmd[1]) - 48);
    int i = (7-y) * 8 + x;
    int j = (7-y_origin) * 8 + x_origin;
    squares[i]->highlight(true);
    squares[j]->highlight(true);

    show();
    cout << "Nr of Moves" << NrofMoves << endl;
    Slider->setMaximum(NrofMoves);
    Slider->setValue(NrofMoves);
}

void QBoard::squareDropped(int target, int source) {
    int y = source / 8 + 1;
    int x = source % 8;
    string cmd;
    cmd.push_back(static_cast<char>(x + 97));
    cmd.push_back(static_cast<char>(y + 48));
    y = target / 8 + 1;
    x = target % 8;
    cmd.push_back(static_cast<char>(x + 97));
    cmd.push_back(static_cast<char>(y + 48));
    //Board b; b.move(); b.sh
    cout << "SquareDropped: " << cmd << endl;
        /* Make a move */
        move(cmd);
        emit madeMove();
        //updateStatusBar();

        /* Aktivate oppent (chess engine) */
        //engineController->go();
}

void QBoard::squareClicked(int id) {
    /*int y = id / 8 + 1;
    int x = id % 8;
    string cmd;
    cmd.push_back(static_cast<char>(x + 97));
    cmd.push_back(static_cast<char>(y + 48));
    cout << "Square " << cmd << " clicked " << endl;
    if(clickcmd) {
        QString txt = input->text();
        input->setText(txt.append(QString::fromStdString(cmd)));
        clickcmd = false;
        readInput();
    } else {
        input->setText(QString::fromStdString(cmd));
        clickcmd = true;
    }*/
}
