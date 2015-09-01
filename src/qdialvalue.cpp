#include "qdialvalue.h"
#include "boost/lexical_cast.hpp"
#include <string>

using namespace std;

QDialValue::QDialValue()
{
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
}

void QDialValue::setValue(int value) {
    setText(QString::fromStdString(boost::lexical_cast<string>(value)));
}

void QDialValue::textChanged(QString text) {
    try
      {
        int value = boost::lexical_cast<int>(text.toStdString());
        emit onValueChanged(value);
      }
      catch (const boost::bad_lexical_cast& e)
      {
        emit onValueChanged(0);
      }

    //QLineEdit::textChanged()
}
