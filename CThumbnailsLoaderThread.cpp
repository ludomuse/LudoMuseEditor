#include "CThumbnailsLoaderThread.h"
#include "CThumbnailWidget.h"
#include <QDebug>

CThumbnailsLoaderThread::CThumbnailsLoaderThread():
    m_pThumbnailList(new QList<CThumbnailWidget*>()),
    m_pMutex(new QMutex()),
    m_pWriteList(new QWaitCondition()),
    m_pReadList(new QWaitCondition())
{
}

void CThumbnailsLoaderThread::run() {
    while (true)
    {
        m_pMutex->lock();
        while (m_pThumbnailList->isEmpty())
        {
            m_pReadList->wait(m_pMutex);
        }
        m_pThumbnailList->first()->UpdateThumbnail();
        m_pThumbnailList->pop_front();
        m_pMutex->unlock();
    }
    //    QMutexLocker lock(m_pMutex);
    //    m_pThumbnailList->first()->UpdateThumbnail();
    //    m_pThumbnailList->pop_front();
    //    QList<CThumbnailWidget*>::iterator i;
    //    for (i = m_pThumbnailList->begin(); i != m_pThumbnailList->end(); ++i)
    //    {
    //        if (*i)
    //        {
    //            (*i)->UpdateThumbnail();
    //        }
    //    }
}

void CThumbnailsLoaderThread::SetThumbnailList(QList<CThumbnailWidget*>* a_pThumbnailList)
{
    m_pMutex->lock();
    m_pThumbnailList = new QList<CThumbnailWidget*>(*a_pThumbnailList);
    m_pMutex->unlock();
    m_pReadList->wakeOne();
}

void CThumbnailsLoaderThread::AddThumbnailList(QList<CThumbnailWidget*>* a_pThumbnailList)
{
    m_pMutex->lock();
    m_pThumbnailList = new QList<CThumbnailWidget*>(*m_pThumbnailList + *a_pThumbnailList);
    m_pMutex->unlock();
    m_pReadList->wakeOne();
}

void CThumbnailsLoaderThread::AddThumbnail(CThumbnailWidget *a_pThumbnail)
{
    m_pMutex->lock();
    m_pThumbnailList->push_back(a_pThumbnail);
    m_pMutex->unlock();
    m_pReadList->wakeOne();
}

void CThumbnailsLoaderThread::Reset()
{
    m_pMutex->lock();
    m_pThumbnailList->clear();
    m_pMutex->unlock();
}

void CThumbnailsLoaderThread::RemoveThumbnail(CThumbnailWidget *a_pThumbnail)
{
    m_pMutex->lock();
    m_pThumbnailList->removeAll(a_pThumbnail);
    m_pMutex->unlock();
}

//void CThumbnailsLoaderThread::run() {
//    std::vector<std::string>::iterator i;
//    for (i = m_pThumbnailsID.begin(); i != m_pThumbnailsID.end(); ++i)
//    {
//        qDebug() << QString::fromStdString(*i);
//        emit loadCapture(QString::fromStdString(*i));
//    }
//}

//void CThumbnailsLoaderThread::SetThumbnailList(const std::vector<std::string> a_vSceneList)
//{
//    m_pThumbnailsID = a_vSceneList;
//}
