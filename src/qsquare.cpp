#include "qsquare.h"
#include "QMouseEvent"
#include "QDrag"
#include "QApplication"
#include "QMimeData"
#include <iostream>
#include "boost/lexical_cast.hpp"
using namespace boost;
using namespace std;
int QSquare::ID = 0;

QSquare::QSquare()
{
    id = ID;
    ID++;
    setAcceptDrops(true);
    piece = new QLabel(this);
    if(id % 2 == 1 - (id / 8 % 2)) this->QLabel::setPixmap(square_l); else this->QLabel::setPixmap(square_d);
}

void QSquare::setPixmap(QPixmap p) {
    piece->setPixmap(p);
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

    //mimeData->setData("text/plain", );
    mimeData->setText(QString::fromStdString(boost::lexical_cast<std::string>(id)));
    drag->setMimeData(mimeData);
    QPixmap pixmap = *this->piece->pixmap();
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
    int source = lexical_cast<int>(event->mimeData()->text().toStdString());
    if(id != source) emit dropped(id, source);
    piece->show();
}
