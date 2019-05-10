#include "database.h"

Database::Database()
{

}

Database::~Database()
{

}

void Database::init()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("WordHunter");
    db.setUserName("yzy");
    db.setPassword("521521");
    if(!db.open())
    {
        qDebug() << "数据库创建失败" ;
    }
    else
    {
        qDebug() << "数据库创建成功" ;
        query = new QSqlQuery;
        query->exec("create table gamer(username varchar(20) primary key, \
                    nickname varchar(20), password varchar(20), \
                    level int, experiencePoint int, \
                    passedStageNumber int, state int)");
        query->exec("create table examer(username varchar(20) primary key, \
                    nickname varchar(20), password varchar(20), \
                    level int, experiencePoint int, \
                    QuestionNumber int, state int)");
        query->exec("create table words(word varchar(30) primary key)");
    }
}

STATE Database::gamerSignin(QString username, QString password)
{
    STATE state = ERROR;
    query = new QSqlQuery;
    QString gamer = QString("select * from gamer where \
                            username = '%1' \
                            and password = '%2'").arg(username).arg(password);
    query->exec(gamer);
    qDebug() << "数据库中的登录函数";
    if(query->first())
    {
        state = static_cast<STATE>(query->value(6).toInt());
        qDebug() << "状态" <<state;
        if(state == OFFLINE)
        {
            QString online = QString("update gamer set \
                                      state = %1 \
                                      where username = '%2'").arg(ONLINE).arg(username);
            query->exec(online);
        }
    }
    else
    {
        qDebug() << query->lastError();
    }

    return state;
}

bool Database::gamerSignup(QString nickname, QString username, QString password)
{
    query = new QSqlQuery;
    QString gamer = QString("select * from gamer where \
                            username = '%1' ").arg(username);
    if(query->exec(gamer))
    {
        if(query->first())
        {
            return false;
        }
        else
        {
            QString nGamer = QString("insert into gamer values(?, ?, ?, ?, ?, ?, ?)");
            query->prepare(nGamer);
            query->bindValue(0, username);
            query->bindValue(1, nickname);
            query->bindValue(2, password);
            query->bindValue(3, 0);
            query->bindValue(4, 0);
            query->bindValue(5, 0);
            query->bindValue(6, OFFLINE);
            query->exec();
            qDebug() << "注册成功";
            return true;
        }
    }
    else
    {
        qDebug() << query->lastError();
        return false;
    }
}

STATE Database::examerSignin(QString username, QString password)
{
    STATE state = ERROR;
    query = new QSqlQuery;
    QString examer = QString("select * from examer where \
                              username = '%1' \
                              and password = '%2'").arg(username).arg(password);
    query->exec(examer);
    if(query->first())
    {
        state = static_cast<STATE>(query->value(6).toInt());
        qDebug() << "状态" <<state;
        if(state == OFFLINE)
        {
            QString online = QString("update examer set \
                                      state = %1 \
                                      where username = '%2'").arg(ONLINE).arg(username);
            query->exec(online);
        }
    }

    return state;
}

bool Database::examerSignup(QString nickname, QString username, QString password)
{
    query = new QSqlQuery;
    QString examer = QString("select * from examer where \
                             username = '%1'").arg(username);
    if(query->exec(examer))
    {
        if(query->first())
        {
            return false;
        }
        else
        {
            QString nExamer = QString("insert into examer values(?, ?, ?, ?, ?, ?, ?)");
            query->prepare(nExamer);
            query->bindValue(0, username);
            query->bindValue(1, nickname);
            query->bindValue(2, password);
            query->bindValue(3, 0);
            query->bindValue(4, 0);
            query->bindValue(5, 0);
            query->bindValue(6, OFFLINE);
            query->exec();
            qDebug() << "注册成功";
            return true;
        }
    }
    else
    {
        qDebug() << query->lastError();
        return false;
    }
}

Gamer Database::getGamerInfo(QString username)
{
    query = new QSqlQuery;
    QString getInfo = QString("select * from gamer where \
                                username = '%1'").arg(username);
    query->exec(getInfo);
    QString nickname;
    int level = 0, exp = 0, passedStageNum = 0;
    if(query->first())
    {
        nickname = query->value(1).toString();
        level = query->value(3).toInt();
        exp = query->value(4).toInt();
        passedStageNum = query->value(5).toInt();
    }
    Gamer gamer(nickname, username, level, exp, passedStageNum);
    return gamer;
}

Examer Database::getExamerInfo(QString username)
{
    query = new QSqlQuery;
    QString getInfo = QString("select * from examer where \
                                username = '%1'").arg(username);
    query->exec(getInfo);
    QString nickname;
    int level = 0, exp = 0, questionNum = 0;
    if(query->first())
    {
        nickname = query->value(1).toString();
        level = query->value(3).toInt();
        exp = query->value(4).toInt();
        questionNum = query->value(5).toInt();
    }
    Examer examer(nickname, username, level, exp, questionNum);
    return examer;
}

void Database::updateInfo(Gamer gamer)
{
    query = new QSqlQuery;
    QString updateInfo = QString("update gamer \
                                    set nickname = '%1', \
                                    level = %2, \
                                    experiencePoint = %3, \
                                    passedStageNumber = %4 \
                                    where username = '%5'").arg(gamer.getNickname()) \
                                                           .arg(gamer.getLevel()) \
                                                           .arg(gamer.getExperiencePoint()) \
                                                           .arg(gamer.getPassedStageNumber()) \
                                                           .arg(gamer.getUsername());
    query->exec(updateInfo);
}

void Database::updateInfo(Examer examer)
{
    query = new QSqlQuery;
    QString updateInfo = QString("update examer \
                                    set nickname = '%1', \
                                    level = %2, \
                                    experiencePoint = %3, \
                                    QuestionNumber = %4 \
                                    where username = '%5'").arg(examer.getNickname()) \
                                                           .arg(examer.getLevel()) \
                                                           .arg(examer.getExperiencePoint()) \
                                                           .arg(examer.getQuestionNumber()) \
                                                           .arg(examer.getUsername());
    query->exec(updateInfo);
}

bool Database::addWord(QString word)
{
    query = new QSqlQuery;
    QString isExisting = QString("select * from words where \
                           word = '%1'").arg(word);
    if(query->exec(isExisting))
    {
        if(query->first())
        {
            return false;
        }
        else
        {
            QString isNewWord = QString("insert into words values(?)");
            query->prepare(isNewWord);
            query->bindValue(0, word);
            query->exec();
            return true;
        }
    }
    else
    {
        qDebug() << query->lastError();
        return false;
    }
}

QString Database::getWord(int difficultDegree)
{
    query = new QSqlQuery;
    QString getword = QString("select * from words where length(word) = %1 \
                                order by random() limit 1").arg(difficultDegree);
    query->exec(getword);

    QString word;

    if(query->first())
    {
        word = query->value(0).toString();
    }
    else
    {
        qDebug() << query->lastError();
    }
    return word;
}

QVector<Gamer> Database::getGamerRanklist()
{
    query = new QSqlQuery;
    QString getRanklist = QString("select * from gamer");

    QVector<Gamer> gamerRanklist;

    query->exec(getRanklist);

    while(query->next())
    {
        QString nickname = query->value(1).toString();
        QString username = query->value(0).toString();
        int level = query->value(3).toInt();
        int exp = query->value(4).toInt();
        int passedStage = query->value(5).toInt();
        Gamer gamer(nickname, username, level, exp, passedStage);
        gamerRanklist.append(gamer);
    }
    return gamerRanklist;
}

QVector<Examer> Database::getExamerRanklist()
{
    query = new QSqlQuery;
    QString getRanklist = QString("select * from examer");

    QVector<Examer> examerRanklist;

    query->exec(getRanklist);

    while(query->next())
    {
        QString nickname = query->value(1).toString();
        QString username = query->value(0).toString();
        int level = query->value(3).toInt();
        int exp = query->value(4).toInt();
        int questionNum = query->value(5).toInt();
        Examer examer(nickname, username, level, exp, questionNum);
        examerRanklist.append(examer);
    }
    return examerRanklist;
}

Gamer Database::findGamer(QString username)
{
    query = new QSqlQuery;
    QString find = QString("select * from gamer where \
                            username = '%1'").arg(username);
    query->exec(find);

    if(query->first())
    {
        QString nickname = query->value(1).toString();
        QString username = query->value(0).toString();
        int level = query->value(3).toInt();
        int exp = query->value(4).toInt();
        int passedStage = query->value(5).toInt();
        Gamer gamer(nickname, username, level, exp, passedStage);
        return  gamer;
    }
    else
    {
        qDebug() << query->lastError();
        Gamer gamer;
        return gamer;
    }
}

Examer Database::findExamer(QString username)
{
    query = new QSqlQuery;
    QString find = QString("select * from examer where \
                            username = '%1'").arg(username);
    query->exec(find);

    if(query->first())
    {
        QString nickname = query->value(1).toString();
        QString username = query->value(0).toString();
        int level = query->value(3).toInt();
        int exp = query->value(4).toInt();
        int questionNum = query->value(5).toInt();
        Examer examer(nickname, username, level, exp, questionNum);
        return examer;
    }
    else
    {
        qDebug() << query->lastError();
        Examer examer;
        return examer;
    }
}

void Database::gamerSignout(QString username)
{
    query = new QSqlQuery;
    QString find = QString("select * from gamer where \
                            username = '%1'").arg(username);
    query->exec(find);

    if(query->first())
    {
        QString offline = QString("update gamer set \
                                  state = %1 \
                                  where username = '%2'").arg(OFFLINE).arg(username);
        query->exec(offline);
    }
    else
    {
        qDebug() << query->lastError();
    }
}

void Database::examerSignout(QString username)
{
    query = new QSqlQuery;
    QString find = QString("select * from examer where \
                            username = '%1'").arg(username);
    query->exec(find);

    if(query->first())
    {
        QString offline = QString("update examer set \
                                  state = %1 \
                                  where username = '%2'").arg(OFFLINE).arg(username);
        query->exec(offline);
    }
    else
    {
        qDebug() << query->lastError();
    }
}
