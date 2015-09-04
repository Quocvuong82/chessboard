/********************************************************************************
** Form generated from reading UI file 'enginecontroller.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENGINECONTROLLER_H
#define UI_ENGINECONTROLLER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EngineController
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_buttons;
    QRadioButton *radio_power;
    QRadioButton *radio_think;
    QRadioButton *radio_play;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *checkBox_black;
    QCheckBox *checkBox_white;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *goButton;
    QPushButton *playButton;
    QVBoxLayout *verticalLayout_depth;
    QLabel *label;
    QDial *searchdepth;
    QSpinBox *spinBox_depth;
    QVBoxLayout *verticalLayout_movetime;
    QLabel *label_2;
    QDial *movetime;
    QSpinBox *spinBox_movetime;
    QVBoxLayout *verticalLayout_nodes;
    QLabel *label_3;
    QDial *nodes;
    QSpinBox *spinBox_nodes;
    QHBoxLayout *horizontalLayout_2;
    QLabel *bestmove;
    QLabel *otherMoves;
    QProgressBar *progressBar;

    void setupUi(QWidget *EngineController)
    {
        if (EngineController->objectName().isEmpty())
            EngineController->setObjectName(QStringLiteral("EngineController"));
        EngineController->resize(461, 300);
        verticalLayout = new QVBoxLayout(EngineController);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_buttons = new QVBoxLayout();
        verticalLayout_buttons->setObjectName(QStringLiteral("verticalLayout_buttons"));
        radio_power = new QRadioButton(EngineController);
        radio_power->setObjectName(QStringLiteral("radio_power"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        radio_power->setFont(font);
        radio_power->setChecked(true);

        verticalLayout_buttons->addWidget(radio_power);

        radio_think = new QRadioButton(EngineController);
        radio_think->setObjectName(QStringLiteral("radio_think"));

        verticalLayout_buttons->addWidget(radio_think);

        radio_play = new QRadioButton(EngineController);
        radio_play->setObjectName(QStringLiteral("radio_play"));

        verticalLayout_buttons->addWidget(radio_play);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        checkBox_black = new QCheckBox(EngineController);
        checkBox_black->setObjectName(QStringLiteral("checkBox_black"));

        horizontalLayout_4->addWidget(checkBox_black);

        checkBox_white = new QCheckBox(EngineController);
        checkBox_white->setObjectName(QStringLiteral("checkBox_white"));

        horizontalLayout_4->addWidget(checkBox_white);


        verticalLayout_buttons->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        goButton = new QPushButton(EngineController);
        goButton->setObjectName(QStringLiteral("goButton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(goButton->sizePolicy().hasHeightForWidth());
        goButton->setSizePolicy(sizePolicy);
        goButton->setMaximumSize(QSize(60, 16777215));
        goButton->setIconSize(QSize(24, 24));

        horizontalLayout_3->addWidget(goButton);

        playButton = new QPushButton(EngineController);
        playButton->setObjectName(QStringLiteral("playButton"));
        playButton->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_3->addWidget(playButton);


        verticalLayout_buttons->addLayout(horizontalLayout_3);


        horizontalLayout->addLayout(verticalLayout_buttons);

        verticalLayout_depth = new QVBoxLayout();
        verticalLayout_depth->setObjectName(QStringLiteral("verticalLayout_depth"));
        label = new QLabel(EngineController);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_depth->addWidget(label);

        searchdepth = new QDial(EngineController);
        searchdepth->setObjectName(QStringLiteral("searchdepth"));
        searchdepth->setMaximum(50);
        searchdepth->setNotchTarget(5);
        searchdepth->setNotchesVisible(true);

        verticalLayout_depth->addWidget(searchdepth);

        spinBox_depth = new QSpinBox(EngineController);
        spinBox_depth->setObjectName(QStringLiteral("spinBox_depth"));

        verticalLayout_depth->addWidget(spinBox_depth);


        horizontalLayout->addLayout(verticalLayout_depth);

        verticalLayout_movetime = new QVBoxLayout();
        verticalLayout_movetime->setObjectName(QStringLiteral("verticalLayout_movetime"));
        label_2 = new QLabel(EngineController);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_movetime->addWidget(label_2);

        movetime = new QDial(EngineController);
        movetime->setObjectName(QStringLiteral("movetime"));
        movetime->setMaximum(10000);
        movetime->setSingleStep(100);
        movetime->setPageStep(1000);
        movetime->setWrapping(false);
        movetime->setNotchTarget(1);
        movetime->setNotchesVisible(true);

        verticalLayout_movetime->addWidget(movetime);

        spinBox_movetime = new QSpinBox(EngineController);
        spinBox_movetime->setObjectName(QStringLiteral("spinBox_movetime"));

        verticalLayout_movetime->addWidget(spinBox_movetime);


        horizontalLayout->addLayout(verticalLayout_movetime);

        verticalLayout_nodes = new QVBoxLayout();
        verticalLayout_nodes->setObjectName(QStringLiteral("verticalLayout_nodes"));
        label_3 = new QLabel(EngineController);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_nodes->addWidget(label_3);

        nodes = new QDial(EngineController);
        nodes->setObjectName(QStringLiteral("nodes"));
        nodes->setMaximum(500);
        nodes->setSingleStep(5);
        nodes->setPageStep(10);
        nodes->setNotchTarget(5);
        nodes->setNotchesVisible(true);

        verticalLayout_nodes->addWidget(nodes);

        spinBox_nodes = new QSpinBox(EngineController);
        spinBox_nodes->setObjectName(QStringLiteral("spinBox_nodes"));

        verticalLayout_nodes->addWidget(spinBox_nodes);


        horizontalLayout->addLayout(verticalLayout_nodes);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        bestmove = new QLabel(EngineController);
        bestmove->setObjectName(QStringLiteral("bestmove"));
        bestmove->setMaximumSize(QSize(100, 16777215));
        QFont font1;
        font1.setPointSize(25);
        font1.setBold(false);
        font1.setWeight(50);
        bestmove->setFont(font1);

        horizontalLayout_2->addWidget(bestmove);

        otherMoves = new QLabel(EngineController);
        otherMoves->setObjectName(QStringLiteral("otherMoves"));
        otherMoves->setMaximumSize(QSize(330, 16777215));

        horizontalLayout_2->addWidget(otherMoves);


        verticalLayout->addLayout(horizontalLayout_2);

        progressBar = new QProgressBar(EngineController);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);


        retranslateUi(EngineController);

        QMetaObject::connectSlotsByName(EngineController);
    } // setupUi

    void retranslateUi(QWidget *EngineController)
    {
        EngineController->setWindowTitle(QApplication::translate("EngineController", "Form", 0));
        radio_power->setText(QApplication::translate("EngineController", "off", 0));
        radio_think->setText(QApplication::translate("EngineController", "think", 0));
        radio_play->setText(QApplication::translate("EngineController", "play", 0));
        checkBox_black->setText(QApplication::translate("EngineController", "black", 0));
        checkBox_white->setText(QApplication::translate("EngineController", "white", 0));
        goButton->setText(QApplication::translate("EngineController", "Go", 0));
        playButton->setText(QApplication::translate("EngineController", "Play", 0));
        label->setText(QApplication::translate("EngineController", "Depth", 0));
        label_2->setText(QApplication::translate("EngineController", "Movetime", 0));
        label_3->setText(QApplication::translate("EngineController", "Nodes", 0));
        bestmove->setText(QApplication::translate("EngineController", "m0v3", 0));
        otherMoves->setText(QApplication::translate("EngineController", "other moves", 0));
    } // retranslateUi

};

namespace Ui {
    class EngineController: public Ui_EngineController {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENGINECONTROLLER_H
