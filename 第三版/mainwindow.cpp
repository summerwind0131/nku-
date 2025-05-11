#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<iostream>
#include<QTime>
#include <QStack>
#include <QList>
#include<QQueue>
#include <QTextDocument>
#include <QTextBlock>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,scene(new QGraphicsScene(this))
    ,currentStep(0)
    ,timer(new QTimer(this))
{
    ui->setupUi(this);


    ui->graphicsView->setScene(scene);
    this->setStyleSheet("background-color: #f4f6f8; color: #2e3a59;");
    QLinearGradient gradient(0, 0, 0, ui->graphicsView->height());
    gradient.setColorAt(0, QColor("#ffffff"));  // 顶部白色
    gradient.setColorAt(1, QColor("#d3e0ea"));  // 底部浅蓝灰

    ui->graphicsView->setBackgroundBrush(QBrush(gradient));

    QString buttonStyle = R"(
    QPushButton {
        background-color: #4a90e2;
        color: white;
        border: none;
        padding: 8px 16px;
        border-radius: 6px;
        font-weight: bold;
    }
    QPushButton:hover {
        background-color: #6fb1fc;
    }
    QPushButton:disabled {
        background-color: #cccccc;
        color: #666666;
    }
)";


    ui->startButton->setStyleSheet(buttonStyle);
    ui->continueButton->setStyleSheet(buttonStyle);
    ui->createButton->setStyleSheet(buttonStyle);
    ui->endButton->setStyleSheet(buttonStyle);
    ui->pauseButton->setStyleSheet(buttonStyle);
    // codeTextEdit 美化
    ui->codeTextEdit->setStyleSheet(R"(
        QPlainTextEdit {
            background-color: #eef1f5;
            border: 1px solid #b0c4de;
            border-radius: 6px;
            padding: 6px;
            font-family: Consolas, monospace;
            color: #2e3a59;
        }
    )");

    // dataSizeBox（QSpinBox）美化
    ui->datasizeBox->setStyleSheet(R"(
        QSpinBox {
            background-color: #ffffff;
            border: 1px solid #b0c4de;
            border-radius: 4px;
            padding: 2px 6px;
        }
        QSpinBox::up-button, QSpinBox::down-button {
            background-color: #d3e0ea;
            border: none;
            width: 14px;
        }
    )");

    // speedSlider 美化
    ui->speedSlider->setStyleSheet(R"(
        QSlider::groove:horizontal {
            height: 6px;
            background: #d3e0ea;
            border-radius: 3px;
        }

        QSlider::handle:horizontal {
            background: #4a90e2;
            border: 1px solid #88ccff;
            width: 14px;
            margin: -5px 0;
            border-radius: 7px;
        }

        QSlider::sub-page:horizontal {
            background: #4a90e2;
            border-radius: 3px;
        }

        QSlider::add-page:horizontal {
            background: #b0c4de;
            border-radius: 3px;
        }
    )");


    bubble_sort_step.clear();
    selection_sort_step.clear();
    insertion_sort_step.clear();
    merge_sort_step.clear();
    quick_sort_step.clear();
    random_quick_sort_step.clear();
    count_sort_step.clear();
    setupCode();//设置代码
    connect(timer,&QTimer::timeout,this,&MainWindow::playNextStep);
    connect(ui->speedSlider, &QSlider::valueChanged, this, &MainWindow::on_speedSlider_valueChanged);
    connect(ui->datasizeBox, SIGNAL(valueChanged(int)), this, SLOT(on_spinBox_valueChanged(int)));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_graphicsView_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint)
{

}


void MainWindow::on_codeTextEdit_blockCountChanged(int newBlockCount)
{

}


void MainWindow::on_speedSlider_actionTriggered(int action)
{
    //接收用户调节排序的速度  对应sliderspeed
    sliderspeed = ui->speedSlider->value(); // 获取滑块数值
    timer->setInterval(800 - sliderspeed);
}


void MainWindow::on_AlgorithmChoiceBox_activated(int index)
{
    //接收用户选择的算法  对应着AlgorithmChoice
    resetSortState();
    AlgorithmChoice=index+1;
    setupCode();
}


void MainWindow::on_startButton_clicked()
{
    srand(QTime::currentTime().msec());
    isPaused=false;
    timer->start(800-sliderspeed);

}


void MainWindow::on_pauseButton_clicked()
{
    //暂停即可
    isPaused=true;
    timer->stop();
}


void MainWindow::on_endButton_clicked()
{
    //直接结束，清空图形，清空datasize，data,step等变量

    data.clear();
    datasize=0;
    currentStep=0;
    scene->clear();
    resetSortState();


}


void MainWindow::on_continueButton_clicked()
{
    //继续开始即可
    if(isPaused)
    {
        isPaused=false;
        timer->start(800-sliderspeed);
    }
}


void MainWindow::on_datasizeBox_textChanged(const QString &arg1)
{
    //接收用户设置的数组大小，改变datasize
    bool ok;
    int size = arg1.toInt(&ok);
    std::cout<<size;
    if (ok && size > 0) { // 确保转换成功且为正数
        datasize = size;
        // 你可以在这里根据 datasize 重新初始化数组等操作
    } else {
        // 输入无效，可以提示用户或设置默认值
        qDebug() << "Invalid input for array size.";
    }

}
void MainWindow::resetSortState()
{
    if (timer && timer->isActive()) timer->stop();
    currentStep = 0;
    selection_sort_step.clear();
    quick_sort_step.clear();
    insertion_sort_step.clear();

    count_sort_step.clear();
    // 清空 scene
    ui->graphicsView->scene()->clear();
}


void MainWindow::on_createButton_clicked()
{
    resetSortState();
    scene->clear();
    data.clear();
    currentStep=0;
    for(int i=0;i<datasize;i++)
    {
        data.append(rand()%40+2);
    }
    switch(AlgorithmChoice)
    {
    case 1:
    {
        BubbleSortStep step0({data,-1,-1,-1,-1});
        generateBubbleSortSteps();
        drawBubblesort(step0.arrayState,step0.currentIndex1,step0.currentIndex2,step0.sortedIndex);
        currentStep=0;
        playNextStep();
        break;
    }
    case 2:
    {
        SelectionSortStep step0({data,-1,-1,-1,-1});
        generateSelectionSortSteps();
        drawSelectionSort(step0.arrayState);
        currentStep=0;
        playNextStep();
        break;
    }
    case 3:
    {
        InsertionSortStep step0({data,-1,-1,-1,-1});
        generateInsertionSortSteps();
        drawInsertionSort(step0.arrayState);
        currentStep=0;
        playNextStep();
        break;
    }
    case 4:
    { // 归并排序
        MergeSortStep step0{data, -1, -1, -1, -1, {}, {}, {}};
        generateMergeSortSteps();
        drawMergeSort(step0.arrayState, -1, -1, {}, {});
        currentStep=0;
        playNextStep();
        break;
    }
    case 5:
    { // 快速排序
        QuickSortStep step0{data, -1, -1, -1, -1, -1, {}, {}};
        generateQuickSortSteps();
        drawQuickSort(step0.arrayState, -1, -1, {}, {0,data.size()});
        currentStep=0;
        playNextStep();
        break;
    }
    case 6:
    { // 随机快速排序
        RandomQuickSortStep step0{data, -1, -1, -1, -1, -1, {}, {}};
        generateRandomQuickSortSteps();
        drawRandomQuickSort(step0.arrayState, -1, -1, -1, {}, {});
        currentStep=0;
        playNextStep();
        break;
    }
    case 7:
    {

        generateCountingSortSteps();
        CountingSortStep step0=count_sort_step[0];
        drawCountingSort(step0.arrayState,step0.countArray,step0.countIndex,step0.writingIndex);
        currentStep=0;
        playNextStep();
        break;

    }
    }


}


void MainWindow::on_spinBox_valueChanged(int value)
{
    datasize = value;
    // 进行更新逻辑，例如初始化数据数组、刷新可视化
}

void MainWindow::playNextStep()
{
    timer->setInterval(800-sliderspeed);
    switch (AlgorithmChoice)
    {
    case 1:
    {
        if(isPaused||currentStep>=bubble_sort_step.size())
        {
            timer->stop();
            return;
        }
        BubbleSortStep step=bubble_sort_step[currentStep];
        drawBubblesort(step.arrayState,step.currentIndex1,step.currentIndex2,step.sortedIndex);
        highlightCodeLine(step.currentline);
        ++currentStep;
        break;
    }
    case 2:
    {
        if(isPaused||currentStep>=selection_sort_step.size())
        {
            timer->stop();
            return;
        }
        SelectionSortStep step=selection_sort_step[currentStep];
        drawSelectionSort(step.arrayState,step.currentmin,step.sortedIndex,step.currentIndex);
        highlightCodeLine(step.currentline);
        ++currentStep;
        break;
    }
    case 3:
    {
        if(isPaused||currentStep>=insertion_sort_step.size())
        {
            timer->stop();
            return;
        }
        InsertionSortStep step=insertion_sort_step[currentStep];
        drawInsertionSort(step.arrayState,step.currentIndex,step.sortedIndex,step.currentComparing);
        highlightCodeLine(step.currentline);
        ++currentStep;
        break;
    }
    case 4:
    { // 归并排序
        if(currentStep >= merge_sort_step.size()) {
            timer->stop();
            return;
        }
        MergeSortStep step = merge_sort_step[currentStep];
        drawMergeSort(step.arrayState, step.left, step.right,
                      step.compareIndexes, step.mergedIndexes);
        highlightCodeLine(step.currentline);
        ++currentStep;
        break;
    }
    case 5:
    { // 快速排序
        if(currentStep >= quick_sort_step.size())
        {
            timer->stop();
            return;
        }
        QuickSortStep step = quick_sort_step[currentStep];
        drawQuickSort(step.arrayState, step.pivotIndex,
                      step.comparingIndex, step.swapIndexes,
                      step.partitionRange);
        highlightCodeLine(step.currentline);
        ++currentStep;
        break;
    }
    case 6:
    { // 随机快速排序
        if(currentStep >= random_quick_sort_step.size()) {
            timer->stop();
            return;
        }
        RandomQuickSortStep step = random_quick_sort_step[currentStep];
        drawRandomQuickSort(step.arrayState, step.randomPivotIndex,
                            step.finalPivotIndex, step.comparingIndex,
                            step.swapIndexes, step.partitionRange);
        highlightCodeLine(step.currentline);
        ++currentStep;
        break;
    }
    case 7:
    {
        if(isPaused||currentStep>=count_sort_step.size())
        {
            timer->stop();
            return;
        }
        CountingSortStep step=count_sort_step[currentStep];
        drawCountingSort(step.arrayState,step.countArray,step.countIndex,step.writingIndex);
        highlightCodeLine(step.currentline);
        ++currentStep;
        break;
    }
    default:
        break;
    }
}


void MainWindow::setupCode()
{
    QStringList lines;
    switch (AlgorithmChoice)
    {
    case 1: // Bubble Sort
        lines <<
            "1. for i from 0 to n-1" <<
            "2.     for j from 0 to n-i-1" <<
            "3.         if data[j] > data[j+1]" <<
            "4.             swap data[j] and data[j+1]";
        break;
    case 2: // Selection Sort
        lines <<
            "1. for i from 0 to n-1" <<
            "2.     minIndex = i" <<
            "3.     for j from i+1 to n-1" <<
            "4.         if data[j] < data[minIndex]" <<
            "5.             minIndex = j" <<
            "6.     swap data[i] and data[minIndex]";
        break;
    case 3: // Insertion Sort
        lines <<
            "1. for i from 1 to n-1" <<
            "2.     key = data[i]" <<
            "3.     j = i - 1" <<
            "4.     while j >= 0 and data[j] > key" <<
            "5.         data[j+1] = data[j],j = j - 1" <<
            "6.     data[j+1] = key";
        break;
    case 4: // MergeSort
        lines <<
            "1. if left < right" <<
            "2.     mergeSort(left, mid)" <<
            "3.     mergeSort(mid+1, right)" <<
            "4.     merge(left, mid, right)" <<
            "5.         while left <= mid and right <= r" <<
            "6.             if left <= right: append left else append right" <<
            "7.         copy rest and merge back";
        break;
    case 5://QuickSort
        lines <<
            "1. function quickSort(arr, left, right)" <<
            "2.     if left >= right return" <<
            "3.     pivot = arr[right]" <<
            "4.     i = left - 1" <<
            "5.     for j from left to right - 1" <<
            "6.         if arr[j] < pivot" <<
            "7.             i++" <<
            "8.             swap arr[i], arr[j]" <<
            "9.     swap arr[i+1], arr[right]" <<
            "10.    quickSort(arr, left, i)" <<
            "11.    quickSort(arr, i+2, right)";
        break;
    case 6: // Randomized QuickSort
        lines <<
            "1. function randomizedQuickSort(arr, left, right)" <<
            "2.     if left >= right return" <<
            "3.     pivotIndex = random(left, right)" <<
            "4.     swap arr[pivotIndex], arr[right]" <<
            "5.     pivot = arr[right]" <<
            "6.     i = left - 1" <<
            "7.     for j from left to right - 1" <<
            "8.         if arr[j] < pivot" <<
            "9.             i++" <<
            "10.            swap arr[i], arr[j]" <<
            "11.    swap arr[i+1], arr[right]" <<
            "12.    randomizedQuickSort(arr, left, i)" <<
            "13.    randomizedQuickSort(arr, i+2, right)";
        break;

    case 7: // Counting Sort
        lines <<
            "1. find max value in data" <<
            "2. create count array of size max+1" <<
            "3. for each element in data" <<
            "4.     count[element]++" <<
            "5. for i from 1 to max" <<
            "6.     count[i] += count[i-1]" <<
            "7. for i from n-1 to 0" <<
            "8.     output[--count[data[i]]] = data[i]" <<
            "9. copy output back to data";
        break;

    case 8: // Radix Sort
        lines <<
            "1. find max number in data" <<
            "2. for exp = 1; max/exp > 0; exp *= 10" <<
            "3.     perform counting sort by digit at exp" <<
            "4.         count digits 0~9" <<
            "5.         compute prefix sums" <<
            "6.         build output by digit order" <<
            "7.         copy output back to data";
        break;


    default:
        lines << "Algorithm not supported yet.";
        break;
    }

    ui->codeTextEdit->clear();
    for (const QString &line : lines)
    {
        ui->codeTextEdit->setPlainText(lines.join("\n")); // 使用换行符连接，避免多次append
    }
}


void MainWindow::highlightCodeLine(int pseudoLineNumber)
{
    QList<QTextEdit::ExtraSelection> selections;

    // 有效性检查
    if(pseudoLineNumber < 1) return; // 伪代码行号从1开始

    QTextDocument* doc = ui->codeTextEdit->document();
    int actualLineNumber = pseudoLineNumber - 1; // 转换为0-based

    if(actualLineNumber >= doc->lineCount()) return;

    // 清除旧高亮
    ui->codeTextEdit->setExtraSelections(selections);

    // 设置新高亮
    QTextEdit::ExtraSelection selection;
    selection.format.setBackground(Qt::yellow);
    QTextCursor cursor(doc->findBlockByLineNumber(actualLineNumber));
    cursor.select(QTextCursor::LineUnderCursor);
    selection.cursor = cursor;
    selections.append(selection);

    ui->codeTextEdit->setExtraSelections(selections);
}



void MainWindow::drawBubblesort(QVector<int>& data, int h1, int h2, int h3)
{
    scene->clear();

    int n = datasize;
    int viewWidth = ui->graphicsView->width();
    int viewHeight = ui->graphicsView->height();

    int barWidth = viewWidth / n;
    int maxVal = *std::max_element(data.begin(), data.end());

    for (int i = 0; i < n; ++i)
    {
        // 高度按最大值比例缩放，保留边距
        int barHeight = static_cast<double>(data[i]) / maxVal * (viewHeight - 40);
        QRectF rect(i * barWidth + 2, viewHeight - barHeight - 20, barWidth - 4, barHeight);

        QColor color;

        // 高亮正在比较的两个元素
        if (i == h1 || i == h2)
        {
            color = QColor(255, 100, 100); // 红色（浅红）
        }
        else if (i >= n - h3)
        {
            color = QColor(100, 220, 100); // 绿色（已排序）
        }
        else
        {
            color = QColor(100, 150, 255); // 蓝色（默认）
        }

        QGraphicsRectItem* bar = scene->addRect(rect, QPen(Qt::black), QBrush(color));

        // 可选：在柱子上方添加数值标签
        QGraphicsTextItem* label = new QGraphicsTextItem(QString::number(data[i]));
        label->setDefaultTextColor(Qt::black);
        label->setFont(QFont("Arial", 8));
        label->setPos(rect.x() + (barWidth - 4) / 4, rect.y() - 15);
        scene->addItem(label);
    }
}


void MainWindow::drawSelectionSort(QVector<int>&data,int h1,int h2,int h3)
{
    scene->clear();
    int n=datasize;
    int width=ui->graphicsView->width();
    int height=ui->graphicsView->height();
    int barWidth=width/n;
    int maxVal = *std::max_element(data.begin(), data.end());
    int barheight=height/maxVal;

    for(int i=0;i<n;i++)
    {
        QRectF rect(i*barWidth, height - data[i] * barheight, barWidth-2, data[i]*barheight);
        QColor color;
        if(i<=h2)
        {
            color=QColor(102, 205, 170); // 已完成: 浅绿色
        }
        else if(i==h1)
        {
            color=QColor(255, 99, 71); // 当前最小值: 番茄红
        }
        else if(i==h3)
        {
            color=QColor(30, 144, 255); // 正在比较: 道奇蓝
        }
        else
        {
            color=QColor(255, 215, 0); // 其他: 金黄
        }
        scene->addRect(rect,QPen(Qt::black),QBrush(color));
        QGraphicsTextItem* text = scene->addText(QString::number(data[i]));
        text->setDefaultTextColor(Qt::black);
        text->setPos(i * barWidth + 2, height - data[i] * barheight - 20);
    }
}

void MainWindow::drawInsertionSort(QVector<int>&data,int h1,int h2,int h3)
{
    scene->clear();
    int n=datasize;
    int width=ui->graphicsView->width();
    int height=ui->graphicsView->height();
    int barWidth=width/n;
    int maxVal = *std::max_element(data.begin(), data.end());
    int barheight=height/maxVal;


    for(int i=0;i<n;i++)
    {
        QRectF rect(i*barWidth, height - data[i] * barheight, barWidth-2, data[i]*barheight);
        QColor color;
        if(i==h1)
        {
            color=QColor(255, 69, 0); // 当前: 橘红
        }
        else if(i==h3)
        {
            color=QColor(255, 215, 0); // 比较对象: 金黄
        }
        else if(i<h2&&i!=h1&&i!=h3)
        {
            color=QColor(144, 238, 144); // 已排序: 淡绿
        }
        else
        {
            color=QColor(100, 149, 237); // 未排序: 淡蓝
        }
        scene->addRect(rect,QPen(Qt::black),QBrush(color));
        QGraphicsTextItem* text = scene->addText(QString::number(data[i]));
        text->setDefaultTextColor(Qt::black);
        text->setPos(i * barWidth + 2, height - data[i] * barheight - 20);
    }
}








void MainWindow::generateBubbleSortSteps()
{
    //根据datasize，data产生对应的步骤
    //生成的步骤要包含：正在比较的？已经比较好的？以及正在执行的伪代码行数
    //此处先想一个简单的，复杂的以后再想
    //此处的step中，第一个维度数据，第二个维度代码行数，第三个维度正在比较的两个数，第四个维度已经比较完的
    bubble_sort_step.clear();
    int n=data.size();
    for(int i=0;i<n;i++)
    {
        bubble_sort_step.push_back({data,1,-1,-1,i});
        for(int j=0;j<n-i-1;j++)
        {
            bubble_sort_step.push_back({data,2,j+1,j+1,i});
            if(data[j]>data[j+1])
            {
                bubble_sort_step.push_back({data,3,j,j+1,i});
                std::swap(data[j],data[j+1]);
                bubble_sort_step.push_back({data,4,j,j+1,i});
            }
        }
    }
}

void MainWindow::generateSelectionSortSteps()
{
    //此处的步骤是
    //for(int i=0;i<n;i++)
    //{  min=data[i];for(int j=i+1;j++;j<n)
    //if(data[j]<min) min=data[j],suoyin=j
    //swap(data[i],data[suoyin])
    //第一个是当前的最小值，第二个是当前正在查找的，第三个是什么也没有的，第四个是已经排过的
    //第一个是当前最小值，第二个是已经排过的，第三个是目前在查的

    selection_sort_step.clear();
    int n=data.size();
    int min=data[0];
    int mark=0;
    for(int i=0;i<n;i++)
    {
        selection_sort_step.push_back({data,1,i,i-1,-1});
        min=data[i];
        mark=i;
        selection_sort_step.push_back({data,2,i,i-1,-1});
        for(int j=i+1;j<n;j++)
        {
            selection_sort_step.push_back({data,3,mark,i-1,j});
            selection_sort_step.push_back({data,4,mark,i-1,j});
            if(data[j]<=min)
            {
                min=data[j];
                mark=j;
                selection_sort_step.push_back({data,5,mark,i-1,-1});
            }

        }
        std::swap(data[i],data[mark]);
        selection_sort_step.push_back({data,6,mark,i-1,-1});

    }
}

void MainWindow::generateInsertionSortSteps()
{
    //第一个索引是目前正在比较的那个，第二个就是前面已经排序过的，第三个就是现在在比较的
    insertion_sort_step.clear();
    int n=datasize;
    for(int i=1;i<n;i++)
    {
        insertion_sort_step.push_back({data,1,i,i,-1});  //拉出来一个比较的
        int current=data[i];
        insertion_sort_step.push_back({data,2,i,i,-1});
        for(int j=i-1;j>0;j--)
        {
            insertion_sort_step.push_back({data,3,j,i,j-1});//拉出来一个开始和前面的比较
            insertion_sort_step.push_back({data,4,j,i,j-1});
            if(data[j]>=current)
            {
                //那就交换
                int a=data[j];
                data[j]=current;
                data[j+1]=a;
                insertion_sort_step.push_back({data,5,j,i,j+1});
            }

        }
    }
}
void MainWindow::generateMergeSortSteps()
{
    merge_sort_step.clear();
    QVector<int> tempData = data;
    QStack<QVector<int>> stack;
    QQueue<QVector<int>> mergeQueue;
    int n=tempData.size()-1;
    // 第一阶段：分割步骤
    stack.push({0, n});
    while (!stack.isEmpty())
    {
        QVector<int> range = stack.pop();
        int left = range[0];
        int right = range[1];

        if (left >= right) continue;

        int mid = left + (right - left)/2;
        merge_sort_step.push_back({tempData, 2,  left, right, mid,{},{},{}});

        // 模拟递归顺序（先右后左）
        stack.push({mid+1, right});
        stack.push({left, mid});
        mergeQueue.enqueue({left, right, mid});
    }

    // 第二阶段：合并步骤
    while (!mergeQueue.isEmpty())
    {
        QVector<int> params = mergeQueue.dequeue();
        int left = params[0];
        int right = params[1];
        int mid = params[2];

        QVector<int> temp(right - left + 1);
        int i = left, j = mid+1, k = 0;

        // 记录合并开始
        merge_sort_step.push_back({tempData,4,left, right,mid,temp,{},{} });

        while (i <= mid && j <= right)
        {
            // 记录比较步骤
            merge_sort_step.push_back({
                tempData,5, left,right,mid,temp,{i, j},{}});

            if (tempData[i] <= tempData[j])
            {
                temp[k++] = tempData[i++];
            } else
            {
                temp[k++] = tempData[j++];
            }

            // 记录移动步骤
            merge_sort_step.push_back({tempData,6,   left,right,mid,temp,{}, {k-1 + left}});
        }

        // 处理剩余元素
        while (i <= mid)
        {
            temp[k++] = tempData[i++];
            merge_sort_step.push_back({ tempData,7,   left,right,mid,temp,{},{k-1 + left}});
        }

        while (j <= right)
        {
            temp[k++] = tempData[j++];
            merge_sort_step.push_back({tempData,7, left,right,mid,temp, {}, {k-1 + left}});
        }

        // 更新原数组
        for (int p = 0; p < k; p++)
        {
            tempData[left + p] = temp[p];
            merge_sort_step.push_back({tempData,8,  left,right, mid,temp,{},{left + p}});
        }
    }
}


void MainWindow::generateQuickSortSteps()
{
    quick_sort_step.clear();
    QVector<int> tempData = data;
    QStack<QVector<int>> stack;
    int n=tempData.size()-1;
    stack.push({0, n});

    while (!stack.isEmpty())
    {
        QVector<int> range = stack.pop();
        int left = range[0];
        int right = range[1];

        quick_sort_step.push_back({tempData,1, -1,-1,left,right,{},{left, right}});

        if (left >= right) {
            quick_sort_step.push_back({
                tempData,
                2,  // if left >= right return
                -1,
                -1,
                left,
                right,
                {},
                {left,right}
            });
            continue;
        }

        int pivot = tempData[right];
        int i = left - 1;

        quick_sort_step.push_back({
            tempData,
            3,  // pivot = arr[right]
            right,
            -1,
            left,
            right,
            {},
            {left,right}
        });

        for (int j = left; j < right; j++) {
            quick_sort_step.push_back({
                tempData,
                5,  // for j from left to right-1
                right,
                j,
                left,
                right,
                {},
                {left,right}
            });

            if (tempData[j] < pivot) {
                i++;
                std::swap(tempData[i], tempData[j]);
                quick_sort_step.push_back({
                    tempData,
                    8,  // swap arr[i] and arr[j]
                    right,
                    j,
                    left,
                    right,
                    {i, j},
                    {left,right}
                });
            }
        }

        std::swap(tempData[i+1], tempData[right]);
        quick_sort_step.push_back({
            tempData,
            9,  // swap arr[i+1] and arr[right]
            right,
            -1,
            left,
            right,
            {i+1, right},
            {left,right}
        });

        stack.push({i+2, right});
        stack.push({left, i});
    }
}


void MainWindow::generateRandomQuickSortSteps()
{
    random_quick_sort_step.clear();
    QVector<int> tempData = data;
    QStack<QVector<int>> stack;
    int n=tempData.size()-1;
    stack.push({0, n});

    while (!stack.isEmpty()) {
        QVector<int> range = stack.pop();
        int left = range[0];
        int right = range[1];

        random_quick_sort_step.push_back({
            tempData,
            1,  // function randomizedQuickSort
            -1,
            -1,
            -1,
            left,
            right,
            {},
            {left, right}
        });

        if (left >= right) {
            random_quick_sort_step.push_back({
                tempData,
                2,  // if left >= right return
                -1,
                -1,
                -1,
                left,
                right,
                {},
                {}
            });
            continue;
        }

        // 随机选择pivot
        int randomIndex = left + rand() % (right - left + 1);
        std::swap(tempData[randomIndex], tempData[right]);
        random_quick_sort_step.push_back({
            tempData,
            4,  // swap arr[pivotIndex] and arr[right]
            randomIndex,
            right,
            -1,
            left,
            right,
            {randomIndex, right},
            {}
        });

        int pivot = tempData[right];
        int i = left - 1;

        random_quick_sort_step.push_back({
            tempData,
            5,  // pivot = arr[right]
            right,
            -1,
            -1,
            left,
            right,
            {},
            {}
        });

        for (int j = left; j < right; j++) {
            random_quick_sort_step.push_back({
                tempData,
                7,  // for j from left to right-1
                right,
                j,
                -1,
                left,
                right,
                {},
                {}
            });

            if (tempData[j] < pivot) {
                i++;
                std::swap(tempData[i], tempData[j]);
                random_quick_sort_step.push_back({
                    tempData,
                    10, // swap arr[i] and arr[j]
                    right,
                    j,
                    -1,
                    left,
                    right,
                    {i, j},
                    {}
                });
            }
        }

        std::swap(tempData[i+1], tempData[right]);
        random_quick_sort_step.push_back({
            tempData,
            11, // swap arr[i+1] and arr[right]
            right,
            -1,
            i+1,
            left,
            right,
            {i+1, right},
            {}
        });

        stack.push({i+2, right});
        stack.push({left, i});
    }
}
void MainWindow::generateCountingSortSteps()
{
    count_sort_step.clear();
    QVector<int> tempData = data;
    if(tempData.isEmpty()) return;

    int maxVal = *std::max_element(tempData.begin(), tempData.end());
    int minVal = *std::min_element(tempData.begin(), tempData.end());
    countingSortMinVal = minVal; // 保存最小值供绘制使用
    int range = maxVal - minVal + 1;

    // 步骤1：创建计数数组
    QVector<int> countArray(range, 0);
    count_sort_step.push_back({tempData, countArray, 1, -1, -1});

    // 步骤2：统计出现次数
    for(int i = 0; i < tempData.size(); ++i) {
        int index = tempData[i] - minVal;
        countArray[index]++;
        count_sort_step.push_back({tempData, countArray, 3, i, -1});
        count_sort_step.push_back({tempData, countArray, 4, i, -1});
    }

    // 步骤3：计算前缀和（原地转换）
    for(int i = 1; i < range; ++i) {
        countArray[i] += countArray[i-1];
        count_sort_step.push_back({tempData, countArray, 5, -1, -1});
    }

    // 步骤4：反向填充结果数组
    QVector<int> output(tempData.size());
    for(int i = tempData.size()-1; i >= 0; --i)
    {
        int val = tempData[i];
        int index = val - minVal;
        int pos = --countArray[index];
        output[pos] = val;

        QVector<int> visualArray = output;
        for(int j = pos+1; j < output.size(); ++j)
        {
            if(visualArray[j] == 0)
                visualArray[j] = tempData[j];
        }

        count_sort_step.push_back({visualArray, countArray, 7, i, pos});
        count_sort_step.push_back({visualArray, countArray, 8, i, pos});
    }

    // 步骤5：回写结果
    for(int i = 0; i < tempData.size(); ++i)
    {
        tempData[i] = output[i];
        count_sort_step.push_back({tempData, countArray, 9, i, i});
    }
}

void MainWindow::drawCountingSort(QVector<int>& data, QVector<int>& countArray, int processingIndex, int writingIndex)
{
    //第一个用来画目前的，也就是countIndex
    //第二个是已经画过的，就是writingIndex
    scene->clear();
    const int sceneWidth = ui->graphicsView->width();
    const int sceneHeight = ui->graphicsView->height();
    const int margin = 20;

    const int mainBarWidth = sceneWidth / data.size();
    const int dataHeightOffset = sceneHeight / 4; // 上下两个条形图拉开一定的距离，设置上边的条形图距离的偏移量

    // 绘制原始数据条形图
    for(int i = 0; i < data.size(); ++i)
    {
        const int value = data[i];
        const int x = i * mainBarWidth;
        const int height = value;

        QColor color;
        if(i == processingIndex)
        {
            color = QColor(255, 99, 71); // 当前处理: 番茄红
        }
        else if(i < writingIndex)
        {
            color = QColor(144, 238, 144); // 已写入: 浅绿
        }
        else
        {
            color = QColor(30, 144, 255); // 默认: 蓝
        }

        scene->addRect(x, dataHeightOffset - height,
                       mainBarWidth - 2, height,
                       QPen(Qt::black), QBrush(color));

        QGraphicsTextItem* text = scene->addText(QString::number(value));
        text->setDefaultTextColor(Qt::black);
        text->setPos(x + 2, dataHeightOffset - height - 20);
    }

    if(!countArray.isEmpty())
    {
        const int countBarWidth = sceneWidth / countArray.size();
        const int maxCount = *std::max_element(countArray.begin(), countArray.end());
        const int countHeightScale = maxCount > 0 ? (sceneHeight / 2 - margin) / maxCount : 0;

        // 绘制计数数组条形图
        for(int v = 0; v < countArray.size(); ++v) {
            const int count = countArray[v];
            if(count == 0) continue;

            const int x = v * countBarWidth;
            const int height = count * countHeightScale;

            // 计算实际值（计数排序范围）
            int actualValue = v + countingSortMinVal;

            bool isCurrent = (processingIndex != -1) && (actualValue == data.value(processingIndex));
            QColor color = isCurrent ? QColor(255, 215, 0) : QColor(255, 250, 200);

            scene->addRect(x, sceneHeight - height - margin,
                           countBarWidth - 2, height,
                           QPen(Qt::black), QBrush(color));

            QGraphicsTextItem* valueText = scene->addText(QString::number(actualValue));
            valueText->setDefaultTextColor(Qt::black);
            valueText->setPos(x + 2, sceneHeight - margin + 2);

            QGraphicsTextItem* countText = scene->addText(QString::number(count));
            countText->setDefaultTextColor(Qt::black);
            countText->setPos(x + 2, sceneHeight - height - margin - 15);
        }
    }
}

void MainWindow::drawMergeSort(QVector<int>& data, int left, int right, QVector<int> compareIndexes, QVector<int> mergedIndexes)
{
    scene->clear();
    int n = data.size();
    int width = ui->graphicsView->width();
    int height = ui->graphicsView->height();
    int barWidth = width / n;
    int maxVal = *std::max_element(data.begin(), data.end());
    double barScale = static_cast<double>(height) / maxVal;

    for(int i = 0; i < n; i++){
        QRectF rect(i * barWidth, height - data[i] * barScale, barWidth - 2, data[i] * barScale);
        QColor color(70, 130, 180); // 默认: 钢蓝色

        // 当前处理区间
        if(i >= left && i <= right){
            color = QColor(255, 255, 102); // 柠檬黄
        }

        // 正在比较的元素
        if(compareIndexes.contains(i)){
            color = QColor(255, 99, 71); // 番茄红
        }

        // 已合并元素
        if(mergedIndexes.contains(i)){
            color = QColor(60, 179, 113); // 中绿
        }

        scene->addRect(rect, QPen(Qt::black), QBrush(color));

        QGraphicsTextItem* text = scene->addText(QString::number(data[i]));
        text->setDefaultTextColor(Qt::black);
        text->setPos(i * barWidth + 2, height - data[i] * barScale - 20);

        // 绘制分割线
        if(i == left || i == right){
            scene->addLine(
                i * barWidth, height,
                i * barWidth, height - 20,
                QPen(QColor(199, 21, 133), 2, Qt::DashLine) // 深洋红虚线
                );
        }
    }
}


void MainWindow::drawQuickSort(QVector<int>& data, int pivotIndex, int comparingIndex, QVector<int> swapIndexes, QVector<int> partitionRange)
{
    scene->clear();
    int n = data.size();
    int width = ui->graphicsView->width();
    int height = ui->graphicsView->height();
    int barWidth = width / n;
    int maxVal = *std::max_element(data.begin(), data.end());
    double barScale = static_cast<double>(height) / maxVal;

    for(int i = 0; i < n; i++){
        QRectF rect(i * barWidth, height - data[i] * barScale, barWidth - 2, data[i] * barScale);
        QColor color(70, 130, 180); // 默认: 钢蓝

        if(i == pivotIndex){
            color = QColor(255, 215, 0); // pivot: 金黄
        } else if(i == comparingIndex){
            color = QColor(255, 99, 71); // 比较: 番茄红
        } else if(swapIndexes.contains(i)){
            color = QColor(60, 179, 113); // 交换: 中绿
        }

        if(!partitionRange.isEmpty() && i >= partitionRange[0] && i <= partitionRange[1]){
            color = color.lighter(150);
        }

        scene->addRect(rect, QPen(Qt::black), QBrush(color));
        QGraphicsTextItem* text = scene->addText(QString::number(data[i]));
        text->setDefaultTextColor(Qt::black);
        text->setPos(i * barWidth + 2, height - data[i] * barScale - 20);

        if(i == partitionRange[0] || i == partitionRange[1]){
            scene->addLine(i * barWidth, height, i * barWidth, height - 20, QPen(QColor(199, 21, 133), 2)); // 深洋红
        }
    }
}

void MainWindow::drawRandomQuickSort(QVector<int>& data, int randomPivotIndex, int finalPivotIndex, int comparingIndex, QVector<int> swapIndexes, QVector<int> partitionRange)
{
    scene->clear();
    int n = data.size();
    int width = ui->graphicsView->width();
    int height = ui->graphicsView->height();
    int barWidth = width / n;
    int maxVal = *std::max_element(data.begin(), data.end());
    double barScale = static_cast<double>(height) / maxVal;

    for(int i = 0; i < n; i++){
        QRectF rect(i * barWidth, height - data[i] * barScale, barWidth - 2, data[i] * barScale);
        QColor color(70, 130, 180); // 默认: 钢蓝

        if(i == randomPivotIndex){
            color = QColor(160, 82, 45); // 原始pivot: 赭色
        } else if(i == finalPivotIndex){
            color = QColor(255, 215, 0); // 最终pivot: 金黄
        } else if(i == comparingIndex){
            color = QColor(255, 99, 71); // 比较: 番茄红
        } else if(swapIndexes.contains(i)){
            color = QColor(60, 179, 113); // 交换: 中绿
        }

        if(!partitionRange.isEmpty() && i >= partitionRange[0] && i <= partitionRange[1]){
            color = color.lighter(150);
        }

        scene->addRect(rect, QPen(Qt::black), QBrush(color));
        QGraphicsTextItem* text = scene->addText(QString::number(data[i]));
        text->setDefaultTextColor(Qt::black);
        text->setPos(i * barWidth + 2, height - data[i] * barScale - 20);

        if(i == randomPivotIndex){
            scene->addEllipse(i * barWidth + barWidth / 4, height - 10, barWidth / 2, 10, QPen(QColor(139, 0, 0), 2)); // 深红椭圆
        }
    }
}


