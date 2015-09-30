#include "qsquare.h"
#include "QMouseEvent"
#include "QDrag"
#include "QApplication"
#include "QMimeData"
#include <iostream>
#include "boost/lexical_cast.hpp"
using namespace boost;
using namespace std;

QString QSquare::PATH = "/home/alex/build-chessboard-Desktop_Qt_5_4_2_GCC_64bit-Debug/";

int QSquare::ID = 0;
QSquare::QSquare()
{
    id = ID;
    ID++;
    setAcceptDrops(true);
    piece = new QLabel(this);
    if(id % 2 == 1 - (id / 8 % 2)) this->QLabel::setPixmap(square_l); else this->QLabel::setPixmap(square_d);
    piece->setStyleSheet("background:transparent");
}

void QSquare::highlight(bool activate) {
    if(activate) this->QLabel::setStyleSheet("border: 2px solid #ffff00"); else
        this->QLabel::setStyleSheet("border: 0px solid #000000");
}

void QSquare::highlight(bool activate, QString color) {
    if(activate) this->QLabel::setStyleSheet("border: 2px solid " + color); else
        this->QLabel::setStyleSheet("border: 0px solid #000000");
}

/* Resets the Squares style to its original state if it matches the style pattern */
void QSquare::clearHighlight(QString style) {
    if(this->QLabel::styleSheet().compare(style) == 0) this->QLabel::setStyleSheet("border: 0px solid #ffff00");
}

void QSquare::setPixmap(QPixmap p) {
    piece->setPixmap(p);
    piece->show();
}

void QSquare::emitSignal() {
    emit clicked(id);
}

void QSquare::mousePressEvent(QMouseEvent *e)
{
    emit clicked(id);
    if (e->button() == Qt::LeftButton) {
        dragStartPosition = e->pos();
        //std::cout << "QSquare: mouse press event" << std::endl;
    }

   /*switch(e->key()) {
   case Qt::Key_Right:
       nextPos();
       break;
   case Qt::Key_Left:
       prevPos();
       break;
   }*/
}

void QSquare::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    //std::cout << "creating a new drag" << std::endl;
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    /* Mime-Data */
    QPixmap pixmap = *this->piece->pixmap();
    mimeData->setText(QString::fromStdString(boost::lexical_cast<std::string>(id)));
    mimeData->setImageData(QVariant(pixmap));
    drag->setMimeData(mimeData);

    drag->setPixmap(pixmap);
    drag->setHotSpot(QPoint(drag->pixmap().width()/2,
                                drag->pixmap().height()/2));
    piece->hide();
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void QSquare::dragEnterEvent(QDragEnterEvent *event)
{
    //if (event->mimeData()->hasFormat("text/plain"))
        event->acceptProposedAction();
}

void QSquare::dropEvent(QDropEvent *event)
{
    cout << event->mimeData()->text().toStdString() << " was dropped on " << id << endl;
    /*textBrowser->setPlainText(event->mimeData()->text());
    mimeTypeCombo->clear();
    mimeTypeCombo->addItems(event->mimeData()->formats());*/
    event->acceptProposedAction();
    int source = lexical_cast<int>(event->mimeData()->text().toStdString()) % 64;
    int target = id % 64;
    if(target != source) emit dropped(target, source);
    //QVariant v = event->mimeData()->imageData();
    piece->setPixmap(event->mimeData()->imageData().value<QPixmap>());
    piece->show();
}
