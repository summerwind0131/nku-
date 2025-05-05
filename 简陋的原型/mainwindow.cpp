#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QTime>
#include<QTimer>
#include<iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,scene(new QGraphicsScene(this))
    ,currentStep(0)
    ,timer(new QTimer(this))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    setupCode();//设置代码
    connect(timer,&QTimer::timeout,this,&MainWindow::playNextStep);
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(on_spinBox_valueChanged(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    srand(QTime::currentTime().msec());
   isPaused=false;
   //后续根据速度再添加
    timer->start(300);

}


void MainWindow::on_endButton_clicked()
{
   //直接结束，清空图形，清空datasize，data,step等变量
    step.clear();
    data.clear();
    datasize=0;
    currentStep=0;
    scene->clear();

}


void MainWindow::on_pauseButton_clicked()
{
   //暂停即可
    isPaused=true;
   timer->stop();
}


void MainWindow::on_pushButton_4_clicked()
{
  //继续开始即可
    if(isPaused)
    {
        isPaused=false;
        timer->start(300);
    }
}


void MainWindow::on_createButton_clicked()
{
    //根据datasize
    //根据spinBox传来的数据n，创建一个大小为m的数组vector，即data
    //data.append(rand()%200+10);

    //此处先随便规定个datasize
    scene->clear();
    step.clear();
    data.clear();
    currentStep=0;
    datasize=20;

    for(int i=0;i<datasize;i++)
    {
        data.append(rand()%200+20);
    }
    for(int i=0;i<datasize;i++)
    {
        std::cout<<data[i]<<std::endl;
    }
    std::cout<<"qiguai";

    //依据algorithmchoice产生步骤，后续再添加
    switch (AlgorithmChoice) {
    case 1:
    {
        Step step0({data,-1,-1,-1});
        generateBubbleSortSteps();
        for(int i=0;i<datasize;i++)
        {
            std::cout<<data[i]<<std::endl;
        }
        std::cout<<"zaikan"<<std::endl;
        drawBars(step0.arrayState,step0.index1,step0.index2,step0.index3);
        currentStep=0;
        playNextStep();
        break;
    }
    case 2:
        generateMergeSortSteps();
        break;
    default:
        break;
    }




}


void MainWindow::on_spinBox_textChanged(const QString &arg1)
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


void MainWindow::on_comboBoxAlgo_activated(int index)
{
  //接收用户选择的算法  对应着AlgorithmChoice
}


void MainWindow::on_Sliderspeed_actionTriggered(int action)
{
  //接收用户调节排序的速度  对应sliderspeed
}


void MainWindow::on_plainTextEdit_blockCountChanged(int newBlockCount)
{

}


void MainWindow::on_graphicsView_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint)
{

}


void MainWindow::generateBubbleSortSteps()
{
    //根据datasize，data产生对应的步骤
    //生成的步骤要包含：正在比较的？已经比较好的？以及正在执行的伪代码行数
    //此处先想一个简单的，复杂的以后再想
    //此处的step中，第一个维度数据，第二个维度代码行数，第三个维度正在比较的两个数，第四个维度已经比较完的
    step.clear();
    int n=data.size();
    for(int i=0;i<n;i++)
    {
        step.push_back({data,1,-1,-1,i});
        for(int j=0;j<n-i-1;j++)
        {
            step.push_back({data,2,j+1,j+1,i});
            if(data[j]>data[j+1])
            {
                std::swap(data[j],data[j+1]);
                step.push_back({data,3,j,j+1,i});
            }
        }
    }

}

void MainWindow::generateMergeSortSteps()
{
    step.clear();
}




void MainWindow::playNextStep()
{
    if(isPaused||currentStep>=step.size())
    {
        timer->stop();
        return;
    }
    Step step1=step[currentStep];
    drawBars(step1.arrayState,step1.index1,step1.index2,step1.index3);
    highlightCodeLine(step1.currentline);
    ++currentStep;

}





//绘画部分

void MainWindow::drawBars(QVector<int>&data,int h1,int h2,int h3)//画数据
{
    //根据step来画，currentstep,step里面的h1,h2,h3画不同颜色
    scene->clear();
    int n=datasize;
    int width=ui->graphicsView->width();
    int height=ui->graphicsView->height();
    int barWidth=width/n;
    for(int i=0;i<n;i++)
    {
        QRectF rect(i*barWidth,height-data[i],barWidth-2,data[i]);
        QColor color;
        if(i==h1||i==h2)
        {
            color=Qt::red;//如果正在排序，变成红色
        }
        else if(i>n-h3||i==n-h3)
        {
            color=Qt::green;//已经排完的，全都是绿色
        }
        else
        {
            color=Qt::blue;//其他都是蓝色
        }
        scene->addRect(rect,QPen(Qt::black),QBrush(color));
    }


}

void MainWindow::highlightCodeLine(int line)//高亮代码行
{
    //根据currrentline来高亮对应的伪代码
}

void MainWindow::setupCode()
{
    switch (AlgorithmChoice) {
    case 1:
    {
        QStringList lines=
        {
            "1.for i from 0 to n-1",
            "2.    for j from 0 to n-i-1",
            "3.        if array[j]>array[j+1]",
            "4.           swap(array[j],array[j+1])"
        };
        ui->plainTextEdit->setPlainText(lines.join("\n"));
        break;
    }
    default:
        break;
    }

}


void MainWindow::on_spinBox_valueChanged(int value)
{
    datasize = value;
    // 进行更新逻辑，例如初始化数据数组、刷新可视化
}
