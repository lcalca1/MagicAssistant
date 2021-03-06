#include "NBAAssistant.h"

NBAAssistant::NBAAssistant(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<QList<TeamScore*>>("TeamScore");
}

NBAAssistant::~NBAAssistant()
{

}

QList<TeamScore*> NBAAssistant::getTeamscore() const
{
    return _TeamScore;
}

void NBAAssistant::setTeamScore(const QList<TeamScore*>& teamscore)
{
    _TeamScore = teamscore;
    emit teamscoreChanged(teamscore);
}

void NBAAssistant::getTodayScore()
{
    QEventLoop loop;

    QNetworkAccessManager manager;
	QNetworkRequest request;
	request.setUrl(QUrl("http://nba.hupu.com/"));
	// 由于虎扑已经升级为https，所以可以直接将URL改为https，也可以加如下属性，可重定向为https.
	request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QNetworkReply *reply = manager.get(request);

    ///< 用事件循环就是为了在一个函数中完成处理操作，当然可以不用事件循环阻塞，将处理解析reply的代码，放到连接finished信号的槽函数中即可.
    //请求结束并下载完成后，退出子事件循环.
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环.
    loop.exec();

    // 当finished信号发出，事件循环结束，此时网页源码已下载完毕，可以开始解析.
    QString source = QString::fromLocal8Bit(reply->readAll());
    analyzeCode(source);
}

void NBAAssistant::analyzeCode(QString source)
{
    int nCPos = source.indexOf("<span class=\"nameText\">");
    while(nCPos != -1)
    {
        TeamScore *data = new TeamScore;
        source = source.mid(nCPos);
        nCPos = source.indexOf("title");
        source = source.mid(nCPos);
        data->_homeTeam = source.section('\"', 1, 1);

        nCPos = source.indexOf("<span class=\"bifen\">");
        source = source.mid(nCPos);
        // 若比赛未开始，那么bifen下面没有title一栏.
        nCPos = source.indexOf("title");
        int nTemp = source.indexOf("<span class=\"nameText\">");

        if(nCPos < nTemp)
        {
            source = source.mid(nCPos);
            nCPos = source.indexOf(">") + 1;
            source = source.mid(nCPos);
            nCPos = source.indexOf("<");
            QString score = source.left(nCPos);
            QStringList scoreList = score.split(":");
            data->_homeScore = scoreList[0].toUInt();
            data->_awayScore = scoreList[1].toUInt();
        }
        else
        {
            data->_homeScore = 0;
            data->_awayScore = 0;
        }

        nCPos = source.indexOf("<span class=\"nameText\">");
        source = source.mid(nCPos);
        nCPos = source.indexOf("title");
        source = source.mid(nCPos);
        data->_awayTeam = source.section('\"', 1, 1);

        _TeamScore << data;

        //查找下一组.
        nCPos = source.indexOf("<span class=\"nameText\">");
    }
}









