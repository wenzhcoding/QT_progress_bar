#ifndef WANDEREDIT_H
#define WANDEREDIT_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableView>
#include <QCheckBox>
#include <QHeaderView>
#include<vector>
class WanderEdit : public QWidget
{
    Q_OBJECT
public:
    explicit WanderEdit(QWidget *parent = 0);
    void TableInit();
    void UpdateTable();
    void SetParameter(int );
    void GetFrameNum();
    void GetAccelerateFactor();

private:
    QPushButton *OkButton;        //确认按钮
    QPushButton *CancelButton;    //取消按钮
    QPushButton *ApplyButton;     //应用按钮
    QLabel *FrameNumLabel;
    QLabel *SpeedLabel;
    QLabel *TimeLabel;
    QCheckBox *IsAverageButton;
    QLineEdit *FrameNumEdit;
    QLineEdit *SpeedEdit;
    QLineEdit *TimeEdit;

    QTableWidget *FrameTable;
    int m_RowCount;
    int m_ColCount;

    int m_TotalFrameNum;
    int m_SpeedFrame;
    std::vector<int> m_AcceleratorFactor;
    std::vector<int> m_KeyFrame;
    std::vector<int> m_SpeedReal;
    std::vector<int> m_TimeLength;
    int m_Length;

//    QLabel
signals:

private slots:

    void on_FrameNumEdit_editingFinished();
    void on_SpeedEdit_editingFinished();
    void on_OkButton_clicked();
    void on_CancelButton_clicked();
    void on_ApplyButton_clicked();
    void on_IsAverageButton_clicked();
    void on_FrameTable_cellChanged(int ,int );
};

#endif // WANDEREDIT_H
