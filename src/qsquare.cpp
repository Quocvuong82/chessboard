#include "qsquare.h"
#include "QMouseEvent"
#include "QDrag"
#include "QApplication"
#include "QMimeData"
#include <iostream>

int QSquare::ID = 0;

QSquare::QSquare()
{
    id = ID;
    ID++;
    setAcceptDrops(true);
}

void QSquare::emitSignal() {
    emit clicked(id);
}

void QSquare::mousePressEvent(QMouseEvent *e)
{
    emit clicked(id);
    if (e->button() == Qt::LeftButton) {
        std::cout << "QSquare: mouse press event" << std::endl;
    }
        dragStartPosition = e->pos();
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

    std::cout << "creating a new drag" << std::endl;
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    //mimeData->setData(mimeType, data);
    drag->setMimeData(mimeData);
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void QSquare::dragEnterEvent(QDragEnterEvent *event)
{
    //if (event->mimeData()->hasFormat("text/plain"))
        event->acceptProposedAction();
}

void QSquare::dropEvent(QDropEvent *event)
{
    textBrowser->setPlainText(event->mimeData()->text());
    mimeTypeCombo->clear();
    mimeTypeCombo->addItems(event->mimeData()->formats());

    event->acceptProposedAction();
}
