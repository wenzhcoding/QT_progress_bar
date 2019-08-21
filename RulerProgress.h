#ifndef RULERPROGRESS_H
#define RULERPROGRESS_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPoint>
#include <QColor>
#include <QTimer>
#include <QSize>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QLineEdit>
#include <QTime>
#include <vector>
#include <QPolygonF>
#include <WanderEdit.h>

namespace Ui {
class RulerProgress;
}

class RulerProgress : public QWidget
{
    Q_OBJECT

public:
    explicit RulerProgress(QWidget *parent = 0);
    ~RulerProgress();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *);
    void DrawBg(QPainter *painter);
    void DrawProgress(QPainter *painter);
    void DrawRulerTop(QPainter *painter);
    void DrawRulerBottom(QPainter *painter);
    void DrawKeyFrame(QPainter *painter);

private:
    int m_MinValue;                //最小值
    int m_MaxValue;                //最大值
    int m_LongStep;                   //长线条等分步长
    int m_ShortStep;                  //短线条等分步
    int m_CurrentValue;               //当前值
    int m_FrameNum;
    int m_FlushTime;
    std::vector<int> m_KeyFrame;
    QTimer *Timer;                  //定时器自动播放
    QPushButton *PlayButton;        //播放按钮
    QPushButton *EditButton;
    QPushButton *PreButton;         //上一关键帧
    QPushButton *NextButton;        //下一关键帧
    QLineEdit *LineEditPlay;        //当前值显示
//    WanderEdit *WanderEditDialogue;
private slots:
    void on_PlayButton_Clicked();           //播放按钮回调函数
    void on_EditButton_Clicked();
    void on_PreButton_Clicked();
    void on_NextButton_Clicked();
    void UpdateValue();             //Timer回调函数
    void LineEditPlayChanged();

public:

    //brief设置横道图参数
    //parameter 横道图尺寸
    void SetParameter(int FrameNum = 300);
    //设置关键帧位置
    void SetKeyFrame(std::vector<int> KeyFrame);
    //设置长线条等分步长
    void SetLongStep(int LongStep = 10);
    //设置短线条等分步长
    void SetShortStep(int ShortStep = 2);
    //设置当前值，并刷新视图
    void SetCurrentValue(int CurrentValue);
    //获取横道图尺寸信息
    int GetMinValue();
    //获取横道图尺寸信息
    int GetMaxValue();
    //获取当前帧数
    int GetCurrentValue();
    //获取长线条等分步长
    int GetLongStep();
    //获取短线条等分步长
    int GetShortStep();
    //获取刷新时间
    int GetFlushTime();

    virtual void SetEnabled(bool bEnable);






private:
    Ui::RulerProgress *ui;
};

#endif // RULERPROGRESS_H
