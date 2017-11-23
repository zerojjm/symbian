#include "highscores.h"

ScoreList::ScoreList()
{
    biggerBetter = true;
    listSize = 10;
}

bool ScoreList::isHighScore(int level, double val)
{
    if(level>2)
        return false;

    if(scores[level].size()<listSize)
        return true;
    foreach(const Player &p, scores[level])
    {
        if(biggerBetter && val>p.value)
            return true;
        if(!biggerBetter && val<p.value)
            return true;
    }
    return false;
}

void ScoreList::addToList(int level, QString name, double val)
{
    if(level>2)
        return;
    Player p;
    p.name = name;
    p.value = val;
    bool added = false;
    for(int i=0;i<scores[level].size();++i)
    {
        if((biggerBetter && val > scores[level].at(i).value)
            ||(!biggerBetter && val< scores[level].at(i).value))
            {
            scores[level].insert(i,p);
            added = true;
            break;
        }
    }
    if(!added)
    {
        scores[level].push_back(p);
    }
    if(scores[level].size()>listSize)
        scores[level].resize(listSize);
}

void ScoreList::saveScores(QString filename, QString tag)
{
    QSettings fscores(filename,QSettings::IniFormat);
    fscores.beginGroup(tag);

    for(int level = 0; level<3;++level)
    {
        fscores.beginWriteArray("Score"+QString::number(level));
        for (int i = 0; i < scores[level].size(); ++i)
        {
             fscores.setArrayIndex(i);
             fscores.setValue("name", scores[level].at(i).name);
             fscores.setValue("value", scores[level].at(i).value);
        }
        fscores.endArray();
    }
    fscores.endGroup();
}

void ScoreList::loadScores(QString filename, QString tag)
{
    QSettings fscores(filename,QSettings::IniFormat);
    fscores.beginGroup(tag);

    for(int level = 0; level<3;++level)
    {
        int size = fscores.beginReadArray("Score"+QString::number(level));
        for (int i = 0; i < size; ++i)
        {
             fscores.setArrayIndex(i);
             Player p;
             p.name = fscores.value("name").toString();
             p.value = fscores.value("value").toDouble();
             scores[level].append(p);
        }
        fscores.endArray();
    }
    fscores.endGroup();
}

int ScoreList::count(int level)
{
    if(level>2)
        return 0;
    return scores[level].size();
}

Player ScoreList::getAt(int level, int pos)
{
    if(level>2)
        return Player();
    if(scores[level].count()<=pos)
        return Player();
    return scores[level].at(pos);
}
