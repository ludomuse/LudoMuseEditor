#ifndef CPATHWIDGET_H
#define CPATHWIDGET_H

#include <QWidget>
#include "CLineEdit.h"
#include "ETypes.h"

class CPathWidget : public QWidget
{
    Q_OBJECT

public:
//    CPathWidget(const QString &a_sFirstValue, const QString& a_sExtensions, QWidget* a_pParent = Q_NULLPTR);
    CPathWidget(const QString &a_sFirstValue, ETypes::Type a_eType, QWidget* a_pParent = Q_NULLPTR);

    void SetPath(const QString& a_sNewPath);

private:
    QString m_sExtensions;

private slots:
    void openPathFileDialog();
    void newPathSelected(QString a_sPath);
    void newDirectoryEntered(QString a_sPath);

signals:
    void pathChanged(QString a_sNewPath);
};

#endif // CPATHWIDGET_H
