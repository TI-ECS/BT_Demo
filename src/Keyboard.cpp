#include "Keyboard.h"


#define MAKE_CONNECTION(i)                                                 \
    connect(pushButton_##i, SIGNAL(clicked()), this, SLOT(keyClicked()))

Keyboard::Keyboard(QWidget *parent, Qt::WindowFlags fl)
    :QDialog(parent, fl)
{
    setupUi(this);

    MAKE_CONNECTION(0);
    MAKE_CONNECTION(1);
    MAKE_CONNECTION(2);
    MAKE_CONNECTION(3);
    MAKE_CONNECTION(4);
    MAKE_CONNECTION(5);
    MAKE_CONNECTION(6);
    MAKE_CONNECTION(7);
    MAKE_CONNECTION(8);
    MAKE_CONNECTION(9);
    MAKE_CONNECTION(return);
    MAKE_CONNECTION(backspace);

    connect(this->buttonBox, SIGNAL(accepted()), this, SLOT(okClicked()));
    connect(this->buttonBox, SIGNAL(rejected()), this, SLOT(cancelClicked()));
}

Keyboard::~Keyboard()
{
}

void Keyboard::keyClicked()
{
    QAbstractButton *button = dynamic_cast<QAbstractButton *>(sender());
    if (!button)
        return;

    if (button->text() == "Return")
        okClicked();
    else if (button->text() == "Backspace")
        this->lineEdit->backspace();
    else
        this->lineEdit->insert(button->text());
}

void Keyboard::okClicked()
{
    done(QDialog::Accepted);
}

void Keyboard::cancelClicked()
{
    done(QDialog::Rejected);
}

QString Keyboard::getText()
{
    return this->lineEdit->text();
}
