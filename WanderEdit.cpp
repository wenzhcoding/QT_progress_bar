#include "WanderEdit.h"
#include "RulerProgress.h"
#include <QDebug>


WanderEdit::WanderEdit(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle(QString::fromLocal8Bit("编辑参数"));
    this->setGeometry(400,400,400,350);
    this->setParent(parent);


    m_RowCount = 4;
    m_ColCount = 5;
    m_TotalFrameNum = 300;
    m_SpeedFrame = 15;
    m_KeyFrame.push_back(1);
    m_KeyFrame.push_back(50);
    m_KeyFrame.push_back(187);
    m_KeyFrame.push_back(300);
    for (int i = 0; i < m_RowCount; i++)
    {
        m_AcceleratorFactor.push_back(1);

        m_SpeedReal.push_back(2679);
        m_TimeLength.push_back(3*(i+5)+1);
    }


    OkButton = new QPushButton(this);
    OkButton->setGeometry(120,310,80,20);
    OkButton->setText(QString::fromLocal8Bit("确认"));

    CancelButton = new QPushButton(this);
    CancelButton->setGeometry(210,310,80,20);
    CancelButton->setText(QString::fromLocal8Bit("取消"));

    ApplyButton = new QPushButton(this);
    ApplyButton->setGeometry(300,310,80,20);
    ApplyButton->setText(QString::fromLocal8Bit("应用"));

    FrameNumLabel = new QLabel(this);
    FrameNumLabel->setGeometry(40,10,50,20);
    FrameNumLabel->setText(QString::fromLocal8Bit("总帧数："));
    FrameNumEdit = new QLineEdit(this);
    FrameNumEdit->setGeometry(92,10,80,20);

    TimeLabel = new QLabel(this);
    TimeLabel->setGeometry(220,10,50,20);
    TimeLabel->setText(QString::fromLocal8Bit("总时间："));
    TimeEdit = new QLineEdit(this);
    TimeEdit->setGeometry(272,10,80,20);
    TimeEdit->setReadOnly(true);

    SpeedLabel = new QLabel(this);
    SpeedLabel->setGeometry(220,40,50,20);
    SpeedLabel->setText(QString::fromLocal8Bit("帧/秒："));
    SpeedEdit = new QLineEdit(this);
    SpeedEdit->setGeometry(272,40,80,20);

    IsAverageButton = new QCheckBox(this);
    IsAverageButton->setGeometry(92,40,60,20);
    IsAverageButton->setText(QString::fromLocal8Bit("匀速"));
    IsAverageButton->setChecked(true);

    FrameNumEdit->setText(QString("%1").arg(m_TotalFrameNum));
    SpeedEdit->setText(QString("%1").arg(m_SpeedFrame));
    TimeEdit->setText(QString("%1").arg(m_TotalFrameNum/m_SpeedFrame));

    TableInit();

    connect(OkButton,SIGNAL(clicked(bool)),this,SLOT(on_OkButton_clicked()));
    connect(CancelButton,SIGNAL(clicked(bool)),this,SLOT(on_CancelButton_clicked()));
    connect(ApplyButton,SIGNAL(clicked(bool)),this,SLOT(on_ApplyButton_clicked()));
    connect(IsAverageButton,SIGNAL(clicked(bool)),this,SLOT(on_IsAverageButton_clicked()));
    connect(FrameNumEdit,SIGNAL(editingFinished()),this,SLOT(on_FrameNumEdit_editingFinished()));
    connect(SpeedEdit,SIGNAL(editingFinished()),this,SLOT(on_SpeedEdit_editingFinished()));

    connect(FrameTable,SIGNAL(cellChanged(int ,int )),this,SLOT(on_FrameTable_cellChanged(int ,int )));


}

void WanderEdit::TableInit()
{

    FrameTable = new QTableWidget(this);
//    FrameTable->model()->setHeaderData();
    FrameTable->setGeometry(16,70,370,236);

    FrameTable->setRowCount(m_RowCount);
    FrameTable->setColumnCount(m_ColCount);
    QHeaderView* headerView = FrameTable->verticalHeader();
    headerView->hide();

    FrameTable->setColumnWidth(0,60);
    FrameTable->setColumnWidth(1,60);
    FrameTable->setColumnWidth(2,60);
    FrameTable->setColumnWidth(3,88);
    FrameTable->setColumnWidth(4,100);

    QStringList header;
    header<<QString::fromLocal8Bit("关键帧")<<QString::fromLocal8Bit("帧")<<QString::fromLocal8Bit("加速器")
          <<QString::fromLocal8Bit("速度（每秒）")<<QString::fromLocal8Bit("已用时间（秒）");
    FrameTable->setHorizontalHeaderLabels(header);
    UpdateTable();
}

void WanderEdit::UpdateTable()
{
    for(int row = 0;row < m_RowCount;row++)//按行写入
    {
        QTableWidgetItem *item_0 = new QTableWidgetItem;
        item_0->setText(QString("%1").arg(row));
        item_0->setFlags(Qt::NoItemFlags);

        QTableWidgetItem *item_1 = new QTableWidgetItem;
        item_1->setText(QString("%1").arg(m_KeyFrame[row]));
        item_1->setFlags(Qt::NoItemFlags);

        QTableWidgetItem *item_2 = new QTableWidgetItem;

        item_2->setText(QString("%1").arg(m_AcceleratorFactor[row]));


        QTableWidgetItem *item_3 = new QTableWidgetItem;
        item_3->setText(QString("%1").arg(m_SpeedReal[row]));
        item_3->setFlags(Qt::NoItemFlags);

        QTableWidgetItem *item_4 = new QTableWidgetItem;
        item_4->setText(QString("%1").arg(m_TimeLength[row]));
        item_4->setFlags(Qt::NoItemFlags);

        FrameTable->setItem(row,0,item_0);
        FrameTable->setItem(row,1,item_1);
        FrameTable->setItem(row,2,item_2);
        FrameTable->setItem(row,3,item_3);
        FrameTable->setItem(row,4,item_4);
    }
}

void WanderEdit::on_OkButton_clicked()
{
    //TO DO 确认数据
    this->close();
}

void WanderEdit::on_CancelButton_clicked()
{
    this->close();
}

void WanderEdit::on_ApplyButton_clicked()
{
    //TO DO 确认数据
    m_AcceleratorFactor = m_SpeedReal;
    UpdateTable();
}

void WanderEdit::on_FrameNumEdit_editingFinished()
{
    double time = FrameNumEdit->text().toInt()/SpeedEdit->text().toInt();
    TimeEdit->setText(QString("%1").arg(time));
}

void WanderEdit::on_SpeedEdit_editingFinished()
{
    double time = FrameNumEdit->text().toInt()/SpeedEdit->text().toInt();
    TimeEdit->setText(QString("%1").arg(time));
}

void WanderEdit::on_IsAverageButton_clicked()
{
    if(IsAverageButton->isChecked())
    {
        for(int i = 0;i < m_AcceleratorFactor.size();i++)
        {
            m_AcceleratorFactor[i] = 1;
        }
        UpdateTable();
    }
}

void WanderEdit::on_FrameTable_cellChanged(int ,int )
{
    //TO DO修改表格后刷新
    qDebug()<<"run here";
}
