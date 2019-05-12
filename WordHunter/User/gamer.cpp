#include "gamer.h"

Gamer::Gamer()
    : User("", "", 0, 0), passedStageNumber(0)
{

}

Gamer::Gamer(QString nn, QString un, int lv, int exp, int passNum)
    : User(nn, un, lv, exp), passedStageNumber(passNum)
{

}

Gamer::~Gamer()
{

}

int Gamer::getPassedStageNumber() const
{
    return passedStageNumber;
}

void Gamer::addStage(int stageAddition)
{
    passedStageNumber += stageAddition;
}

