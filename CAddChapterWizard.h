#ifndef CADDCHAPTERWIZARD_H
#define CADDCHAPTERWIZARD_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class CAddChapterWizard : public QDialog
{
    Q_OBJECT
public:
    CAddChapterWizard();

private:
    QLabel mInformations;
    QLineEdit mChapterNameLineEdit;
    QPushButton mValidateButton;
    QPushButton mCancelButton;

private slots:
    void clickOnCancel();
    void clickOnValidate();
};

#endif // CADDCHAPTERWIZARD_H
