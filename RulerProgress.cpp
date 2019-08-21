#include "RulerProgress.h"
#include <QDebug>


RulerProgress::RulerProgress(QWidget *parent) :
    QWidget(parent)
{
    this->setGeometry(500,500,500,300);

    m_FlushTime = 50;
    SetParameter(300);
    SetLongStep(10);
    SetShortStep(2);

    Timer = new QTimer(this);
    Timer->stop();

    PlayButton = new QPushButton(this);
//    PlayButton->setText(tr("Start"));
    PlayButton->move(200, 250);
    PlayButton->setStyleSheet("QPushButton{background:white;border:1px solid red}");

    QIcon icon;
    icon.addFile(tr("C:/Users/wenzh/Desktop/play.png"));
    PlayButton->setIcon(icon);
    QSize buttonsize = QSize::QSize(30,30);
    PlayButton->setIconSize(buttonsize);
    PlayButton->show();

    EditButton = new QPushButton(this);
    EditButton->setText("edit");
    EditButton->move(300, 250);

    PreButton = new QPushButton(this);
    PreButton->move(350, 250);
    PreButton->setText("pre");

    NextButton = new QPushButton(this);
    NextButton->move(400, 250);
    NextButton->setText("next");

    LineEditPlay = new QLineEdit(this);
    LineEditPlay->setText("Value");
    LineEditPlay->setGeometry(100,250,80,30);
    LineEditPlay->installEventFilter(this);


    connect(PlayButton,SIGNAL(clicked(bool)),this,SLOT(on_PlayButton_Clicked()));
    connect(PreButton,SIGNAL(clicked(bool)),this,SLOT(on_PreButton_Clicked()));
    connect(NextButton,SIGNAL(clicked(bool)),this,SLOT(on_NextButton_Clicked()));

    connect(EditButton,SIGNAL(clicked(bool)),this,SLOT(on_EditButton_Clicked()));
    connect(Timer, SIGNAL(timeout()), this, SLOT(UpdateValue()));
    connect(LineEditPlay,SIGNAL(editingFinished()),this,SLOT(LineEditPlayChanged()));

    m_KeyFrame.push_back(10);
    m_KeyFrame.push_back(100);
    m_KeyFrame.push_back(150);
}

RulerProgress::~RulerProgress()
{

}

void RulerProgress::paintEvent(QPaintEvent *)
{

    //绘制准备工作,启用反锯齿
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    //绘制渐变背景
    DrawBg(&painter);
    //绘制进度
    DrawProgress(&painter);
    //绘制标尺
    DrawRulerTop(&painter);
    //绘制关键帧
    DrawKeyFrame(&painter);

}

void RulerProgress::DrawBg(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::black);
    painter->setBrush(Qt::white);
    painter->drawRect(rect());
    painter->restore();
}

void RulerProgress::DrawProgress(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::black);
    painter->setBrush(Qt::white);

    double length = width();
    double increment = length / (m_MaxValue - m_MinValue);
    double initX = (m_CurrentValue - m_MinValue) * increment;


    painter->drawRect(0,0,int(initX), height());
    painter->restore();
}

void RulerProgress::DrawRulerTop(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::black);

    double initX = 0;

    //绘制横向标尺上部分底部线
    double initTopY = 0;
    QPointF lineTopLeftPot = QPointF(initX, initTopY);
    QPointF lineTopRightPot = QPointF(width() - initX, initTopY);
    painter->drawLine(lineTopLeftPot, lineTopRightPot);

    //绘制上部分及下部分横向标尺刻度
    double length = width();
    //计算每一格移动多少
    double increment = length / (m_MaxValue - m_MinValue);
    //长线条短线条长度
    int longLineLen = 20;
    int shortLineLen = 10;

    //根据范围值绘制刻度值及刻度值 长线条需要移动10像素 短线条需要移动5像素
    for (int i = m_MinValue; i <= m_MaxValue; i = i + m_ShortStep)
    {
        if (i % m_LongStep == 0)
        {
            QPointF topPot = QPointF(initX, initTopY);
            QPointF bottomPot = QPointF(initX, initTopY + longLineLen);
            painter->drawLine(topPot, bottomPot);

            //第一个值和最后一个值不要绘制
            if (i == m_MinValue || i == m_MaxValue)
            {
                initX += increment * m_ShortStep;
                continue;
            }
            //TO DO
            int timelength = m_FlushTime * i;
            int MsInt = (timelength%1000)/100;
            int SInt = (timelength/1000)%60;
            int MInt = (timelength/60000)%60;
            QTime currentTime = QTime::QTime(0,MInt,SInt,MsInt) ;
            QString strValue = currentTime.toString("mm:ss.z");//QString("%1").arg((int)i, 0 );

            double textWidth = fontMetrics().width(strValue);
            double textHeight = fontMetrics().height();

            QPointF textPot = QPointF(initX - textWidth / 2, initTopY + textHeight + longLineLen);
            painter->drawText(textPot, strValue);           
        }
        else
        {
            if (i % (m_LongStep / 2) == 0)
            {
                shortLineLen = 10;
            } else
            {
                shortLineLen = 6;
            }

            QPointF topPot = QPointF(initX, initTopY);
            QPointF bottomPot = QPointF(initX, initTopY + shortLineLen);
            painter->drawLine(topPot, bottomPot);
        }

        initX += increment * m_ShortStep;
    }

    painter->restore();
}

void RulerProgress::DrawKeyFrame(QPainter *painter)
{
    double length = width();
    double increment = length / (m_MaxValue - m_MinValue);

    painter->save();
    painter->setPen(Qt::black);
    painter->setBrush(Qt::gray);
    size_t Num = m_KeyFrame.size();
    for(int i = 0;i < Num;i++)
    {
        int CenterPoint = m_KeyFrame[i] * increment;//关键帧所在帧数，对应坐标点
        QPolygonF polygon;
        polygon << QPointF(CenterPoint, 50) << QPointF(CenterPoint+10, 60) << QPointF(CenterPoint, 70) << QPointF(CenterPoint-10, 60);
        painter->drawPolygon(polygon, Qt::WindingFill);
    }
    painter->restore();
}


void RulerProgress::mousePressEvent(QMouseEvent *event)
{
    int curWindowPos = RulerProgress::pos().x();
    int curWindowWidth = RulerProgress::width();
    int curMousePos = event->globalX();
    m_CurrentValue = (curMousePos-curWindowPos)*(m_MaxValue-m_MinValue)/curWindowWidth;
    if(m_CurrentValue>m_MaxValue) m_CurrentValue = m_MaxValue;
    if(m_CurrentValue<m_MinValue) m_CurrentValue = m_MinValue;
    int timelength = m_FlushTime * m_CurrentValue;
    int MsInt = (timelength%1000)/100;
    int SInt = (timelength/1000)%60;
    int MInt = (timelength/60000)%60;
    QTime currentTime = QTime::QTime(0,MInt,SInt,MsInt) ;
    QString strValue = currentTime.toString("mm:ss.z");//QString("%1").arg((int)i, 0 );
    LineEditPlay->setText(strValue);//(QString("%1").arg(m_CurrentValue));
    update();


}

void RulerProgress::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    int curWindowPos = RulerProgress::pos().x();
    int curWindowWidth = RulerProgress::width();
    int curMousePos = event->globalX();
    m_CurrentValue = (curMousePos-curWindowPos)*(m_MaxValue-m_MinValue)/curWindowWidth;
    if(m_CurrentValue>m_MaxValue) m_CurrentValue = m_MaxValue;
    if(m_CurrentValue<m_MinValue) m_CurrentValue = m_MinValue;
    int timelength = m_FlushTime * m_CurrentValue;
    int MsInt = (timelength%1000)/100;
    int SInt = (timelength/1000)%60;
    int MInt = (timelength/60000)%60;
    QTime currentTime = QTime::QTime(0,MInt,SInt,MsInt) ;
    QString strValue = currentTime.toString("mm:ss.z");//QString("%1").arg((int)i, 0 );
    LineEditPlay->setText(strValue);//(QString("%1").arg(m_CurrentValue));
    update();
}

void RulerProgress::UpdateValue()
{

    if (m_CurrentValue < m_MaxValue)
    {
        m_CurrentValue += 1;
    }
    else
    {

        Timer->stop();
    }
    int timelength = m_FlushTime * m_CurrentValue;
    int MsInt = (timelength%1000)/100;
    int SInt = (timelength/1000)%60;
    int MInt = (timelength/60000)%60;
    QTime currentTime = QTime::QTime(0,MInt,SInt,MsInt) ;
    QString strValue = currentTime.toString("mm:ss.z");//QString("%1").arg((int)i, 0 );
    LineEditPlay->setText(strValue);//(QString("%1").arg(m_CurrentValue));
    update();

}

void RulerProgress::on_PlayButton_Clicked()
{
        //gbmp::ActionManager::SendCommand(ID_CMD_TEST_LINE1);
        if (Timer->isActive())
        {
            Timer->stop();
        }
        else
        {
            Timer->start(m_FlushTime);

        }

}

void RulerProgress::SetParameter(int FrameNum)
{
    m_MinValue = 0;
    m_MaxValue = FrameNum-1;
    m_CurrentValue = 0;
    m_FrameNum = FrameNum;
}

void RulerProgress::SetCurrentValue(int CurrentValue)
{
    m_CurrentValue = CurrentValue;
    update();
    LineEditPlay->setText(QString("%1").arg(m_CurrentValue));
}

void RulerProgress::SetLongStep(int LongStep)
{
    m_LongStep = LongStep;
}

void RulerProgress::SetShortStep(int ShortStep)
{
    m_ShortStep = ShortStep;
}

void RulerProgress::SetKeyFrame(std::vector<int> KeyFrame)
{
    //TO DO
    m_KeyFrame = KeyFrame;
}

int RulerProgress::GetCurrentValue()
{
    return m_CurrentValue;
}

int RulerProgress::GetMinValue()
{
    return m_MinValue;
}

int RulerProgress::GetMaxValue()
{
    return m_MaxValue;
}

int RulerProgress::GetLongStep()
{
    return m_LongStep;
}

int RulerProgress::GetShortStep()
{
    return m_ShortStep;
}

void RulerProgress::SetEnabled(bool bEnable)
{

}

void RulerProgress::LineEditPlayChanged()
{
    m_CurrentValue = LineEditPlay->text().toInt();
    if(m_CurrentValue>m_MaxValue) m_CurrentValue = m_MaxValue;
    if(m_CurrentValue<m_MinValue) m_CurrentValue = m_MinValue;
    update();
   // LineEditPlay->setText(QTime::QTime(0,0,0,m_CurrentValue).toString("hh:mm:ss.zzz"));
    LineEditPlay->setText(QString("%1").arg(m_CurrentValue));
}

void RulerProgress::on_EditButton_Clicked()
{
    WanderEdit *WanderEditDialogue = new WanderEdit;
    WanderEditDialogue->show();
}

void RulerProgress::on_PreButton_Clicked()
{

    for(int i = m_KeyFrame.size() - 1;i >= 0;i--)
    {
        if(m_CurrentValue>m_KeyFrame[i])
        {
            m_CurrentValue = m_KeyFrame[i];
            int timelength = m_FlushTime * m_CurrentValue;
            int MsInt = (timelength%1000)/100;
            int SInt = (timelength/1000)%60;
            int MInt = (timelength/60000)%60;
            QTime currentTime = QTime::QTime(0,MInt,SInt,MsInt) ;
            QString strValue = currentTime.toString("mm:ss.z");//QString("%1").arg((int)i, 0 );
            LineEditPlay->setText(strValue);//(QString("%1").arg(m_CurrentValue));
            update();
            break;
        }
    }

}

void RulerProgress::on_NextButton_Clicked()
{
    for(int i = 0;i<m_KeyFrame.size();i++)
    {
        if(m_CurrentValue<m_KeyFrame[i])
        {
            m_CurrentValue = m_KeyFrame[i];
            m_CurrentValue = m_KeyFrame[i];
            int timelength = m_FlushTime * m_CurrentValue;
            int MsInt = (timelength%1000)/100;
            int SInt = (timelength/1000)%60;
            int MInt = (timelength/60000)%60;
            QTime currentTime = QTime::QTime(0,MInt,SInt,MsInt) ;
            QString strValue = currentTime.toString("mm:ss.z");//QString("%1").arg((int)i, 0 );
            LineEditPlay->setText(strValue);//(QString("%1").arg(m_CurrentValue));
            update();
            break;
        }
    }
}
