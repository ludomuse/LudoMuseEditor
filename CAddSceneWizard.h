#ifndef CADDSCENEWIZARD_H
#define CADDSCENEWIZARD_H

#include <QDialog>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>

class CAddSceneWizard : public QDialog
{
    Q_OBJECT
public:
    CAddSceneWizard(int a_iActivePlayer,
                    const std::vector<std::string>& a_rSceneIDP1,
                    const std::vector<std::string>& a_rSceneIDP2,
                    QWidget* a_pParent = Q_NULLPTR,
                    QString a_sPreviousID1 = Q_NULLPTR,
                    QString a_sPreviousID2 = Q_NULLPTR);

private:
    void FillComboBox(int a_iPlayerID, const QString& activeSceneID);
    QString m_sPreviousID1;
    QString m_sPreviousID2;
    QLabel* m_pPreviewTitle;
    QLineEdit* m_pNewID;
    QCheckBox* m_pPlayer1CheckBox;
    QCheckBox* m_pPlayer2CheckBox;
    QComboBox* m_pComboBoxID;
    int m_iTemplateNumber;
    int m_iActivePlayer;
    const std::vector<std::string> m_rSceneIDP1;
    const std::vector<std::string> m_rSceneIDP2;

private slots:
    void clickOnBack(bool);
    void clickOnValidate(bool);
    void setTemplate1();
    void setTemplate2();
    void setTemplate3();
    void changeActivePlayer(); // Update previous id comboBox
signals:
    void validate(const QString& a_sPreviousID,const QString& a_sNewID, int a_iPlayerID, int a_iTemplateNumber);

};

#endif // CADDSCENEWIZARD_H
