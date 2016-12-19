#include "CColorButton.h"

#include <QColorDialog>
#include <QDebug>

CColorButton::CColorButton(QString a_iFirstColor, QWidget *a_pParent):
    QPushButton(a_pParent),
    m_oColor(ParseColor(a_iFirstColor))
{
    if (a_iFirstColor == ""){
        a_iFirstColor = "255,255,255,255";
    }
    setStyleSheet("QPushButton{background-color : rgba(" + a_iFirstColor + ");"+
                  "border-style: outset;"+
                  "border-width: 2px;"+
                  "border-color: (200, 200, 200);}");

    connect(this, SIGNAL(clicked(bool)), this, SLOT(openColorDialog()));
}

void CColorButton::openColorDialog()
{
    QColorDialog* colorDialog = new QColorDialog(m_oColor);
    colorDialog->show();
    connect(colorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(changeColor(QColor)));
}

void CColorButton::changeColor(const QColor &a_oColor)
{
    m_oColor = a_oColor;
    QString color;
    color += QString::number(a_oColor.red())+","+QString::number(a_oColor.green())+","+QString::number(a_oColor.blue())+","+QString::number(a_oColor.alpha());
    setStyleSheet("QPushButton{background-color : rgba(" + color + ");"+
                  "border-style: outset;"+
                  "border-width: 2px;"+
                  "border-color: (200, 200, 200);}");
    emit colorChanged(color);
}

const QColor& CColorButton::ParseColor(const QString &a_sColor) const
{
    QStringList oRGBAValues = a_sColor.split(",");
    if (oRGBAValues.count() >= 4) // if color is not properly set, do nothing
    {
        return (QColor(oRGBAValues.at(0).toInt(), oRGBAValues.at(1).toInt(), oRGBAValues.at(2).toInt(), oRGBAValues.at(3).toInt()));
    }
    else
    {
        return QColor(255,255,255,255);
    }
}
