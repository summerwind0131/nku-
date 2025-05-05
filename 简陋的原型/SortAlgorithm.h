#ifndef SORTALGORITHM_H
#define SORTALGORITHM_H
#include<QObject>
#include"DataModel.h"
class SortAlgorithm:public QObject{
    Q_OBJECT
public:
    explicit SortAlgorithm(DataModel *model,QObject *parent=nullptr);
    virtual void start (int interval)=0;
    virtual void pause()=0;
    virtual void resume()=0;
    virtual void step()=0;
    virtual void stop()=0;
signals:
    void highlightline(int);
    void compare(int ,int);
    void swapOccurred(int ,int);
    void finished();
protected:
    DataModel *m_model;

};

#endif // SORTALGORITHM_H
