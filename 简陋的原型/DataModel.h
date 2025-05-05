#ifndef DATAMODEL_H
#define DATAMODEL_H
#include<QObject>
#include<vector>

class DataModel:public QObject{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent=nullptr);//构造函数
    void setData(const std::vector<int>&arr);  //设置数组信息，包括各个数，各个数的状态，比较次数，交换次数，操作次数
    const std::vector<int>&data()const {return m_data;}
    const std::vector<int>&state()const{return m_state;}
    int compareCount ()const{return m_compareCount; }
    int swapCount()const{return m_swapCount;}
    void swap(int i,int j);//交换函数，交换两个数的位置，并改变对应的状态，从而在图上体现
    void resetStates();//重新设置所有数的状态为没有交换
    void markSorted();//标记已经排好序的数
    void compare(int i,int j);//比较函数
signals:
    void dataChanged();
    void resetOccurred();
    void compare();
private:
    std::vector<int>m_data;
    std::vector<int>m_state;
    int m_compareCount=0;
    int m_swapCount=0;
};

#endif // DATAMODEL_H
