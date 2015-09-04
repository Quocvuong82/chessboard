/********************************************************************************
** Form generated from reading UI file 'icgame.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ICGAME_H
#define UI_ICGAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ICGame
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QPushButton *pushButton;

    void setupUi(QWidget *ICGame)
    {
        if (ICGame->objectName().isEmpty())
            ICGame->setObjectName(QStringLiteral("ICGame"));
        ICGame->resize(400, 300);
        verticalLayoutWidget = new QWidget(ICGame);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 381, 281));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(verticalLayoutWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout->addWidget(listWidget);

        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);


        retranslateUi(ICGame);

        QMetaObject::connectSlotsByName(ICGame);
    } // setupUi

    void retranslateUi(QWidget *ICGame)
    {
        ICGame->setWindowTitle(QApplication::translate("ICGame", "Form", 0));
        pushButton->setText(QApplication::translate("ICGame", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class ICGame: public Ui_ICGame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ICGAME_H
