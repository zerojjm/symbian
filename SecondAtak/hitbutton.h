#ifndef HITBUTTON_H
#define HITBUTTON_H

#include <QPushButton>

class HitButton : public QPushButton
{
    Q_OBJECT
public:
    explicit HitButton(QWidget *parent = 0);

    int x;
    int y;
    bool posValid;

signals:

public slots:

};

#endif // HITBUTTON_H
