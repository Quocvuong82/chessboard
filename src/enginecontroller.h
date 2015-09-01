#ifndef ENGINECONTROLLER_H
#define ENGINECONTROLLER_H

#include <QWidget>

namespace Ui {
class EngineController;
}

class EngineController : public QWidget
{
    Q_OBJECT

public:
    explicit EngineController(QWidget *parent = 0);
    ~EngineController();

private:
    Ui::EngineController *ui;
};

#endif // ENGINECONTROLLER_H
