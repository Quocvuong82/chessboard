#ifndef QDIALVALUE_H
#define QDIALVALUE_H
#include <QLineEdit>

class QDialValue : public QLineEdit
{
    Q_OBJECT
public:
    QDialValue();
public slots:
    void setValue(int value);
    void textChanged(QString);
signals:
    void onValueChanged(int);
};

#endif // QDIALVALUE_H
