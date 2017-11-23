#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <QSettings>
#include <QVector>

struct Player{
    Player(){name = "error"; value = 0;}
    QString name;
    double value;
};

class ScoreList
{

public:
    ScoreList();
    bool isHighScore(int level, double val);
    void addToList(int level, QString name, double val);

    void saveScores(QString filename, QString tag);
    void loadScores(QString filename, QString tag);

    int count(int level);
    Player getAt(int level, int pos);
    QVector<Player> scores[3];

    bool biggerBetter;

    int listSize;
};

#endif // HIGHSCORES_H
