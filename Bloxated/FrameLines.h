#ifndef FRAMELINES_H
#define FRAMELINES_H

#include <QFrame>

class FrameLines : public QFrame
{
    Q_OBJECT
public:
    explicit FrameLines(QWidget *parent = 0);
    int index;

    void setColorMode();
    void setNumberMode();
signals:

public slots:

};

#endif // FRAMELINES_H
