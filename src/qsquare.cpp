#include "qsquare.h"

int QSquare::ID = 0;

QSquare::QSquare()
{
    id = ID;
    ID++;
}

void QSquare::emitSignal() {
    emit clicked(id);
}

void QSquare::mousePressEvent(QMouseEvent *e)
{
    emit clicked(id);
   /*switch(e->key()) {
   case Qt::Key_Right:
       nextPos();
       break;
   case Qt::Key_Left:
       prevPos();
       break;
   }*/
}
