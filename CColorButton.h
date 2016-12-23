#ifndef CCOLORBUTTON_H
#define CCOLORBUTTON_H

#include <QWidget>
#include <QPushButton>

class CColorButton : public QPushButton
{
    Q_OBJECT
public:
    CColorButton(QString a_iFirstColor, QWidget* a_pParent = Q_NULLPTR);
    QColor ParseColor(const QString &a_sColor) const;

private slots:
    void changeColor(const QColor& a_oColor);
    void openColorDialog();

signals:
    void colorChanged(const QString& a_oColor);

private:
    QColor m_oColor;
};
#endif // CCOLORBUTTON_H
