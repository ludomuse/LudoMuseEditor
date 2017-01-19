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
#include "ETypes.h"

class CMacroWidget : public QWidget
{

    Q_OBJECT
public:
    CMacroWidget(QWidget* a_pParent = Q_NULLPTR);
    void Init();

private:
    QGridLayout* m_pImageLayout;
    QGridLayout* m_pSoundLayout;
    QGridLayout* m_pFontLayout;
    QLineEdit* m_pMacroName;
    QComboBox* m_pMacroType;

    QGridLayout* GetLayout(ETypes::Type a_eType);

    void AddRow(ETypes::Type a_eType, const QString &a_sMacro);

public slots:
    void addRow();

signals:
    void macroModified();
};

class CDefineEdit : public CLineEdit
{
    Q_OBJECT
public :
    CDefineEdit(const QString &a_sMacro, ETypes::Type a_eType, QWidget* a_pParent);
    QString GetMacro();
private:
    QString m_sMacro;
public slots:
    void removeDefine();
    void setDefine(const QString& a_sDef);
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
};

#endif //CMACROWIDGET_H
