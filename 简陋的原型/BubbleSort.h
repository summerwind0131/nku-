#ifndef BUBBLESORT_H
#define BUBBLESORT_H
#include<QObject>
#include<QTimer>
#include"DataModel.h"
#include"SortAlgorithm.h"


class BubbleSort:public SortAlgorithm,DataModel,QObject
{

};


#endif // BUBBLESORT_H
