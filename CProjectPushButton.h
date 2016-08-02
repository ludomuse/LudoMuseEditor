#ifndef CPROJECTPUSHBUTTON_H
#define CPROJECTPUSHBUTTON_H

#include <QPushButton>
#include <QString>


class CProjectPushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit CProjectPushButton(QString a_sPath, QWidget *parent = 0);
    ~CProjectPushButton();

private:
    QString m_sPath;

public slots:
    void onClick(bool);

signals:
    void onClick(const QString& a_sPath);
};

#endif // CPROJECTPUSHBUTTON_H
