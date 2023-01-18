//
// Created by alexa on 01/06/2022.
//

#ifndef CPP_QT_TPMINIPROJET_LABELEDTEXTFIELD_H
#define CPP_QT_TPMINIPROJET_LABELEDTEXTFIELD_H

#include <QLabel>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

class LabeledTextField : public QWidget {
    Q_OBJECT
public:
    LabeledTextField(QWidget* parent = nullptr) : QWidget(parent){
        this->label = new QLabel();
        this->text = new QTextEdit();
        QHBoxLayout* mainLayout = new QHBoxLayout();

        this->text->setMaximumHeight(25);

        mainLayout->addWidget(label);
        mainLayout->addWidget(text);
        this->setLayout(mainLayout);
    }
    QLabel* label = nullptr;
    QTextEdit* text = nullptr;
};
#endif //CPP_QT_TPMINIPROJET_LABELEDTEXTFIELD_H

