#include"DataModel.h"

DataModel::DataModel(QObject *parent)
    : QObject(parent), m_compareCount(0), m_swapCount(0) {}
void DataModel::setData(const std::vector<int>&arr)
{
    m_data=arr;
    m_state.assign(arr.size(),0);
    m_compareCount=0;
    m_swapCount=0;
}
void DataModel::swap(int i,int j)
{

}
void DataModel::resetStates()
{

}
void DataModel::markSorted()
{

}
void DataModel::compare(int i,int j)
{

}
