#include "enginecontroller.h"
#include "ui_enginecontroller.h"
#include "qdialvalue.h"

EngineController::EngineController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EngineController)
{
    ui->setupUi(this);
    engine = new UCIEngine();
    goButtonPressed = false;

    ui->verticalLayout->addWidget(engine->output);
    connect(ui->goButton, SIGNAL(pressed()), this, SLOT(toggleGoStop()));
    //connect(ui->refreshButton, SIGNAL(pressed()), engine, SLOT(showOutput()));
    connect(engine, SIGNAL(newBestmove()), this, SLOT(showBestmove()));
    connect(engine, SIGNAL(newBestmove()), this, SLOT(showOtherMoves()));
    connect(engine, SIGNAL(stateChanged(int)), this, SLOT(updateController(int)));

    /* Connect Search Parameter Controllers to Engine */
    connect(ui->searchdepth, SIGNAL(valueChanged(int)), engine, SLOT(setSearchDepth(int)));
    connect(ui->movetime, SIGNAL(valueChanged(int)), engine, SLOT(setMovetime(int)));
    connect(ui->nodes, SIGNAL(valueChanged(int)), engine, SLOT(setNodes(int)));

    /* Link QDials and QSpinBoxes */
    connect(ui->spinBox_depth, SIGNAL(valueChanged(int)), ui->searchdepth, SLOT(setValue(int)));
    connect(ui->spinBox_movetime, SIGNAL(valueChanged(int)), ui->movetime, SLOT(setValue(int)));
    connect(ui->spinBox_nodes, SIGNAL(valueChanged(int)), ui->nodes, SLOT(setValue(int)));
    connect(ui->searchdepth, SIGNAL(valueChanged(int)), ui->spinBox_depth, SLOT(setValue(int)));
    connect(ui->movetime, SIGNAL(valueChanged(int)), ui->spinBox_movetime, SLOT(setValue(int)));
    connect(ui->nodes, SIGNAL(valueChanged(int)), ui->spinBox_nodes, SLOT(setValue(int)));

    connect(ui->playButton, SIGNAL(pressed()), this, SLOT(play()));

    connect(engine, SIGNAL(newDepth(int)), this, SLOT(showDepth(int)));

    connect(ui->radio_play, SIGNAL(pressed()), this, SLOT(turnOn()));
    connect(ui->radio_think, SIGNAL(pressed()), this, SLOT(turnOn()));
    connect(ui->radio_power, SIGNAL(pressed()), this, SLOT(turnOff()));

    ui->searchdepth->setMinimum(0);
    ui->searchdepth->setMaximum(50);
    ui->movetime->setMinimum(0);
    //ui->movetime->setMaximum(100000);
    ui->nodes->setMinimum(0);
    ui->nodes->setMaximum(500);
    ui->spinBox_depth->setMaximum(ui->searchdepth->maximum());
    ui->spinBox_movetime->setMaximum(ui->movetime->maximum());
    ui->spinBox_nodes->setMaximum(ui->nodes->maximum());
    turnOff();
}

EngineController::~EngineController()
{
    delete ui;
}

bool EngineController::isOn() {
    return !ui->radio_power->isChecked();
}

void EngineController::turnOff() {
    ui->goButton->setEnabled(false);
    ui->playButton->setEnabled(false);
    ui->spinBox_depth->setEnabled(false);
    ui->spinBox_movetime->setEnabled(false);
    ui->spinBox_nodes->setEnabled(false);
    ui->searchdepth->setEnabled(false);
    ui->movetime->setEnabled(false);
    ui->nodes->setEnabled(false);
    ui->progressBar->setEnabled(false);
    ui->bestmove->setEnabled(false);
    ui->otherMoves->setEnabled(false);
    engine->output->setEnabled(false);
}

void EngineController::turnOn() {
    ui->goButton->setEnabled(true);
    ui->playButton->setEnabled(true);
    ui->spinBox_depth->setEnabled(true);
    ui->spinBox_movetime->setEnabled(true);
    ui->spinBox_nodes->setEnabled(true);
    ui->searchdepth->setEnabled(true);
    ui->movetime->setEnabled(true);
    ui->nodes->setEnabled(true);
    ui->progressBar->setEnabled(true);
    ui->bestmove->setEnabled(true);
    ui->otherMoves->setEnabled(true);
    engine->output->setEnabled(true);
}

void EngineController::setBoard(Board *board) {
    this->board = board;
}

void EngineController::setGame(Game* game) {
    this->game = game;
}

void EngineController::go() {
    if(isOn()) {
        if(!goButtonPressed) {
            if(ui->radio_play->isChecked()) {
                if(ui->checkBox_black->isChecked() && !ui->checkBox_white->isChecked() && game->getActiveColor() == 'w') return;
                if(ui->checkBox_white->isChecked() && !ui->checkBox_black->isChecked() && game->getActiveColor() == 'b') return;
            }
        }
        if(!engine->isThinking()) {
            engine->setPosition(game->board->getFenstring());
            engine->go();
            ui->goButton->setText("stop");
            ui->playButton->setEnabled(false);
            ui->progressBar->setValue(0);
        }
        goButtonPressed = false;
    }
}

void EngineController::toggleGoStop() {
    if(isOn()) {
        if(engine->isThinking()) {
            engine->stop();
            ui->goButton->setText("stopping");
        } else {
            goButtonPressed = true;
            go();
        }
    }
}

void EngineController::play() {
    /* make moves in long algebraic notation "d2d4" or "f7f8q" */
    if(engine->getBestmove().size() == 4 || engine->getBestmove().size() == 5) {
        game->move(engine->getBestmove());

        /* Let other widgets know, that we made a move */
        emit madeMove();
        ui->playButton->setDisabled(true);

        /* Think on next move if radiobutton play and color checkbox are checked */
        if(ui->radio_play->isChecked() || ui->radio_think->isChecked()) {
            if((ui->checkBox_black->isChecked() && game->getActiveColor() == 'b')
                    || (ui->checkBox_white->isChecked() && game->getActiveColor() == 'w')) go();
        }
    }
}

void EngineController::showOtherMoves() {
    ui->otherMoves->setText(QString::fromStdString(engine->getOtherMoves()));
}

void EngineController::showBestmove() {
    /* Write Bestmove to Screen */
    ui->bestmove->setText(QString::fromStdString(engine->getBestmove()));
    ui->progressBar->setValue(100);

    /* Make a move or activate play button */
    if(ui->radio_play->isChecked()) {
        if(ui->checkBox_black->isChecked() && game->getActiveColor() == 'b') play();
        if(ui->checkBox_white->isChecked() && game->getActiveColor() == 'w') play();
    } else
    ui->playButton->setEnabled(true);
    emit newBestmove(engine->getBestmove());
}

void EngineController::showDepth(int depth) {
    double x = (double) depth/ (double) ui->searchdepth->value() * 100;
    double fx = pow(2.5,(x/10 - 5));
    if((int)fx > ui->progressBar->value())
    ui->progressBar->setValue((int)fx);
}

void EngineController::updateController(int state) {
    cout << "update controller state " << state << endl;
    switch (state) {
    case 0:
        ui->goButton->setText("go");
        break;
    case 1:
        ui->goButton->setText("stop");
        break;
    }
}

void EngineController::undock() {
    setParent(0);
    show();
}
