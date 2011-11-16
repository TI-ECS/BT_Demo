/*
* Bluetooth Demo
*
* Bluetooth demo application.
*
* Copyright (C) 2011 ProFUSION embedded systems
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of ProFUSION embedded systems nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

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
