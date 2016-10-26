#ifndef CTHUMBNAILSLOADERTHREAD_H
#define CTHUMBNAILSLOADERTHREAD_H

#include <QThread>
#include <QList>
#include <QMutex>
#include <QWaitCondition>
#include "CThumbnailWidget.h"

class CThumbnailsLoaderThread : public QThread
{
    Q_OBJECT

public:
    CThumbnailsLoaderThread();
    void SetThumbnailList(QList<CThumbnailWidget*>* a_pThumbnailList);
    void AddThumbnailList(QList<CThumbnailWidget*>* a_pThumbnailList);
    //    void LoadThumbnail(CThumbnailWidget* a_pThumbnail);
    void AddThumbnail(CThumbnailWidget* a_pThumbnail);
    void RemoveThumbnail(CThumbnailWidget* a_pThumbnail);
    void Reset();

protected:
    virtual void run() override;

private:
    QList<CThumbnailWidget*>* m_pThumbnailList;
    QMutex* m_pMutex;
    QWaitCondition *m_pWriteList;
    QWaitCondition *m_pReadList;

    //public:
    //    virtual void run() override;
    //    void SetThumbnailList(const std::vector<std::string>);

    //private:
    //    std::vector<std::string> m_pThumbnailsID;

    //signals:
    //    void loadCapture(const std::string);
};

#endif // CTHREADCOCOS_H
