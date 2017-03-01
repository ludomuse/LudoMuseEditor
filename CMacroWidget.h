#ifndef CMACROWIDGET_H
#define CMACROWIDGET_H

#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPoint>
#include "CLineEdit.h"
#include "Classes/Engine/Include/CKernel.h"
#include "ETypes.h"

class CMacroWidget : public QWidget
{

    Q_OBJECT
public:
    CMacroWidget(QWidget* a_pParent = Q_NULLPTR);
    void Init();
    void SetKernel(LM::CKernel* a_pKernel);

private:
    QGridLayout* m_pImageLayout;
    QGridLayout* m_pSoundLayout;
    QGridLayout* m_pFontLayout;
    QLineEdit* m_pMacroName;
    QComboBox* m_pMacroType;

    QGridLayout* GetLayout(ETypes::Type a_eType);
    LM::CKernel* m_pKernel;

    void AddRow(ETypes::Type a_eType, const QString &a_sMacro);

public slots:
    void addRow();

signals:
    void macroModified();
    void deleteClicked();
};

class CDefineEdit : public CLineEdit
{
    Q_OBJECT
public :
    CDefineEdit(const QString &a_sMacro, ETypes::Type a_eType, QWidget* a_pParent);
    QString GetMacro();
    void SetKernel(LM::CKernel* a_pKernel);

private:
    QString m_sMacro;
    LM::CKernel* m_pKernel;

public slots:
    void removeDefine();
    void setDefine(const QString& a_sDef);
    void deleteIfUnused();

signals:
    void deleteConfirmed();
};

class CMacroLabel : public QLabel
{
    Q_OBJECT
public :
    CMacroLabel(const QString& a_sText, ETypes::Type a_sType, QWidget* a_pParent);
private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    ETypes::Type m_sType;
    QPoint m_pStartDragPos;

    LM::CKernel* m_pKernel;
};

//class CMacroLine : QObject
//{
//    Q_OBJECT

//public:
//    CMacroLine(CMacroLabel* a_pLabel, CDefineEdit* a_pDefine, QToolButton* a_pDelete, QObject* parent);

//private:
//    CMacroLabel* m_pLabel;
//    CDefineEdit* m_pDefine;
//    QToolButton* m_pDelete;

//private slots:
//    void deleteIfUnused();
//};

#endif //CMACROWIDGET_H
