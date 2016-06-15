#ifndef CTHREADCOCOS_H
#define CTHREADCOCOS_H

#include <QObject>

#include <Classes/Engine/Include/CKernel.h>


class CThreadCocos : public QObject
{
    Q_OBJECT
public:
    CThreadCocos();
    ~CThreadCocos();

public slots:
    void process();

signals:
    void finished();
    void error(QString err);
    void sendHWND(int);
    void sendKernel(LM::CKernel*);

private:
    bool m_bContinue;
    //LM::CKernel* m_oLudoMuseKernel;
};

#endif // CTHREADCOCOS_H
