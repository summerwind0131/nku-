#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>
#include<QGraphicsAnchor>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


struct Step
{
    QVector<int>arrayState;
    int currentline;
    int index1;
    int index2;
    int index3;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

    void on_endButton_clicked();

    void on_pauseButton_clicked();

    void on_pushButton_4_clicked();

    void on_createButton_clicked();

    void on_spinBox_textChanged(const QString &arg1);

    void on_comboBoxAlgo_activated(int index);

    void on_Sliderspeed_actionTriggered(int action);

    void on_plainTextEdit_blockCountChanged(int newBlockCount);

    void on_graphicsView_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

    void playNextStep();

    void on_spinBox_valueChanged(int value);
private:

    //绘制页面的操作
    void setupCode();//书写伪代码
    void drawBars(QVector<int>&data,int h1=-1,int h2=-1,int h3=-1);//画数据
    void highlightCodeLine(int line);//高亮代码行

    //产生不同算法的步骤
    void generateBubbleSortSteps();
    void generateMergeSortSteps();

    Ui::MainWindow *ui;
    QTimer *timer;
    bool isPaused;
    int currentStep=0;
    //画图部分
    //数据部分
    //步骤部分

    QVector<Step> step;
    QVector<int>data;
    QGraphicsScene *scene;
    int datasize=0;   //数据大小
    int sliderspeed;  //排序速度
    int AlgorithmChoice=1;//选择的算法

};
#endif // MAINWINDOW_H
