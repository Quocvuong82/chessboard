/********************************************************************************
** Form generated from reading UI file 'icgamelist.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ICGAMELIST_H
#define UI_ICGAMELIST_H

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

class Ui_ICGameList
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QPushButton *pushButton;

    void setupUi(QWidget *ICGameList)
    {
        if (ICGameList->objectName().isEmpty())
            ICGameList->setObjectName(QStringLiteral("ICGameList"));
        ICGameList->resize(1085, 429);
        verticalLayout = new QVBoxLayout(ICGameList);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listWidget = new QListWidget(ICGameList);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout->addWidget(listWidget);

        pushButton = new QPushButton(ICGameList);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);


        retranslateUi(ICGameList);

        QMetaObject::connectSlotsByName(ICGameList);
    } // setupUi

    void retranslateUi(QWidget *ICGameList)
    {
        ICGameList->setWindowTitle(QApplication::translate("ICGameList", "Form", 0));
        pushButton->setText(QApplication::translate("ICGameList", "Refresh", 0));
    } // retranslateUi

};

namespace Ui {
    class ICGameList: public Ui_ICGameList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ICGAMELIST_H
