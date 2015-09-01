#include "enginecontroller.h"
#include "ui_enginecontroller.h"

EngineController::EngineController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EngineController)
{
    ui->setupUi(this);
}

EngineController::~EngineController()
{
    delete ui;
}
