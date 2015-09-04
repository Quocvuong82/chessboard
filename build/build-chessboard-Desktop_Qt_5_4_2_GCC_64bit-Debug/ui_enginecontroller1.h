/********************************************************************************
** Form generated from reading UI file 'enginecontroller1.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENGINECONTROLLER1_H
#define UI_ENGINECONTROLLER1_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EngineController
{
public:

    void setupUi(QWidget *EngineController)
    {
        if (EngineController->objectName().isEmpty())
            EngineController->setObjectName(QStringLiteral("EngineController"));
        EngineController->resize(400, 300);

        retranslateUi(EngineController);

        QMetaObject::connectSlotsByName(EngineController);
    } // setupUi

    void retranslateUi(QWidget *EngineController)
    {
        EngineController->setWindowTitle(QApplication::translate("EngineController", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class EngineController: public Ui_EngineController {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENGINECONTROLLER1_H
