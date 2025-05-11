#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>
#include<QGraphicsAnchor>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class BubbleSort;
}
QT_END_NAMESPACE


struct BubbleSortStep
{
    QVector<int>arrayState;
    int currentline;
    int currentIndex1;
    int currentIndex2;
    int sortedIndex;
};
struct SelectionSortStep
{
    QVector<int>arrayState;
    int currentline;
    int currentmin;
    int sortedIndex;
    int currentIndex;
};
struct InsertionSortStep
{
    QVector<int>arrayState;
    int currentline;
    int currentIndex;
    int sortedIndex;
    int currentComparing;
};

struct MergeSortStep {
    QVector<int> arrayState;
    int currentline;
    int left;
    int right;
    int mid;
    QVector<int> tempArray;
    QVector<int> compareIndexes; // 当前比较的元素对
    QVector<int> mergedIndexes;  // 已合并的元素索引
};

struct QuickSortStep {
    QVector<int> arrayState;
    int currentline;
    int pivotIndex;
    int comparingIndex;
    int left;
    int right;
    QVector<int> swapIndexes;    // 当前交换的元素对
    QVector<int> partitionRange; // 当前分区范围 [left, right]
};

struct RandomQuickSortStep {
    QVector<int> arrayState;
    int currentline;
    int randomPivotIndex;
    int finalPivotIndex;
    int comparingIndex;
    int left;
    int right;
    QVector<int> swapIndexes;
    QVector<int> partitionRange;
};



struct CountingSortStep
{

    //步骤应该是根据arraystate生成从小到最大的countArray每个都是0
    //接着开始遍历所有数据，然后下面对应的countArray++
    //接着根据countArray再重新画图排序
    QVector<int>arrayState;//需要排序的数据
    QVector<int>countArray;//计数数组每个数的大小
    int currentline;//代码高两行
    int countIndex;//对应计数数组的索引
    int writingIndex;//
    int currentIndex;
};






class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_graphicsView_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

    void on_codeTextEdit_blockCountChanged(int newBlockCount);

    void on_speedSlider_actionTriggered(int action);

    void on_AlgorithmChoiceBox_activated(int index);

    void on_startButton_clicked();

    void on_pauseButton_clicked();

    void on_endButton_clicked();

    void on_continueButton_clicked();

    void on_datasizeBox_textChanged(const QString &arg1);

    void on_createButton_clicked();

    void playNextStep();

    void on_spinBox_valueChanged(int value);
    void on_speedSlider_valueChanged(int value)
    {
        sliderspeed = value;
        timer->setInterval(800 - sliderspeed);
    }


private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QGraphicsScene *scene;


    bool isPaused;
    int currentStep=0;
    int AlgorithmChoice=1;
    int datasize=20;
    QVector<int>data;
    int sliderspeed=0;

    int countingSortMinVal=0;

    QVector<BubbleSortStep>bubble_sort_step;
    QVector<SelectionSortStep>selection_sort_step;
    QVector<InsertionSortStep>insertion_sort_step;
    QVector<MergeSortStep>merge_sort_step;
    QVector<QuickSortStep>quick_sort_step;
    QVector<RandomQuickSortStep>random_quick_sort_step;
    QVector<CountingSortStep>count_sort_step;


    void setupCode();//书写代码
    void highlightCodeLine(int currentline);//高亮伪代码


    void drawBubblesort(QVector<int>&data,int h1=-1,int h2=-2,int h3=-3);
    void drawSelectionSort(QVector<int>&data,int h1=-1,int h2=-1,int h3=-1);
    void drawInsertionSort(QVector<int>&data,int h1=-1,int h2=-1,int h3=-1);
    void drawMergeSort(QVector<int>& data,int left,int right,QVector<int> compareIndexes,QVector<int> mergedIndexes);
    void drawQuickSort(QVector<int>& data,int pivotIndex,int comparingIndex,QVector<int> swapIndexes,QVector<int> partitionRange);
    void drawRandomQuickSort(QVector<int>& data,int randomPivotIndex,int finalPivotIndex,int comparingIndex,QVector<int> swapIndexes,QVector<int> partitionRange);
    void drawCountingSort(QVector<int>& data, QVector<int>& countArray,int processingIndex,int writingIndex);


    void generateBubbleSortSteps();//1
    void generateSelectionSortSteps();//2
    void generateInsertionSortSteps();//3
    void generateMergeSortSteps();//4
    void generateQuickSortSteps();//5
    void generateRandomQuickSortSteps();//6
    void generateCountingSortSteps();//7



    void resetSortState();




};
#endif // MAINWINDOW_H
