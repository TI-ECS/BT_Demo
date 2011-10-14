#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "ui_Keyboard.h"

#include <QtGui>

class Keyboard : public QDialog, public Ui::Keyboard
{
Q_OBJECT

public:
    Keyboard(QWidget *parent = 0, Qt::WindowFlags fl = Qt::Dialog
					 | Qt::WindowStaysOnTopHint
					 | Qt::X11BypassWindowManagerHint);

    virtual ~Keyboard();
    QString getText();

private slots:
    void keyClicked();
    void okClicked();
    void cancelClicked();
};

#endif /* _KEYBOARD_H_ */
