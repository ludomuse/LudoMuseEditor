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
    void OpenModalDialog(const QString& a_sText);
    void SetEnabledPlayerField(int a_iPlayerID, bool a_bEnabled);
    QString m_sPreviousID1;
    QString m_sPreviousID2;
    QLabel* m_pPreviewTitle;
    QLabel* m_pPreviewTitle2;
    QLineEdit* m_pNewID;
    QLineEdit* m_pNewID2;
    QLineEdit* m_pSoundPath;
    QLineEdit* m_pSoundPath2;
    QCheckBox* m_pPlayer1CheckBox;
    QCheckBox* m_pPlayer2CheckBox;
    QCheckBox* m_pDashCheckBox;
    QCheckBox* m_pDashCheckBox2;
    QCheckBox* m_pSynchroCheckBox;
    QCheckBox* m_pSynchroCheckBox2;
    QComboBox* m_pComboBoxID;
    QComboBox* m_pComboBoxID2;
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
    // Signal emit when only one player box checked
    void addOneScene(const QString& a_sPreviousID, const QString& a_sNewID, int a_iPlayerID, int a_iTemplateNumber);
    // Signal emit when both player check box checked
    void addTwoScene(const QString& a_sPreviousIDP1, const QString& a_sNewIDP1,
                     const QString& a_sPreviousIDP2, const QString& a_sNewIDP2,
                     int a_iTemplateNumberP1);
    void validate(const QString& a_sPreviousID,const QString& a_sNewID, int a_iPlayerID, int a_iTemplateNumber);

};

#endif // CADDSCENEWIZARD_H
