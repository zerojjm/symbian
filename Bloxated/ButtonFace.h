#ifndef CLICKAREA_H
#define CLICKAREA_H

#include <QFrame>
#include <QMouseEvent>
#include <QGridLayout>
#include <QLabel>

class ButtonFace : public QFrame
{
    Q_OBJECT
public:
    explicit ButtonFace(QWidget *parent = 0);
    int colorIndex;
protected:
 //    bool event(QEvent *event);
signals:

public slots:
    void setNumberMode(int index);
    void setColorMode(int index);
private:
    QLabel* lblText;

};

#endif // CLICKAREA_H
