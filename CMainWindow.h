#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

//#include <GL/glew.h>
//#include <WinSock2.h>
#include <QMainWindow>
#include <QWindow>
#include <QProcess>
#include <QFileSystemModel>

#include "CThumbnailWidget.h"
#include "CTemplateManager.h"
#include "CTemplate.h"

#include <Classes/Engine/Include/CKernel.h>
#include <Classes/Engine/Include/CLabelNode.h>
#include <Classes/Engine/Include/CSpriteNode.h>
#include <Classes/Engine/Include/CMenuNode.h>



namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

private:

    /// \brief Populate both area scroll for each player time line
    void ProcessTree();

    /// \brief return playerId who have the scene matching the param id
    int ScreenIDToPlayerID(const QString &a_id);

    /// \brief Add scene id to the time line by creating a new widget
    /// \param int player number
    /// \param @&id scene id in string type
    /// \returns Return widget associated with the scene id passed through param
    CThumbnailWidget* addSceneToTimeLine(const QString &id, int);

    /// \brief allow label inspection in the widget named : inspectorContainer
    /// \param CLabel the label which is actually under inspection/ modification
    void InspectLabel(LM::CLabelNode*);

    /// \brief allow sprite inspection in the widget named : inspectorContainer
    /// \param CSprite the sprite which is actually under inspection/ modification
    void InspectSprite(LM::CSpriteNode* a_pSprite);

    /// \brief update and allow scene inspection
    void InspectScene(LM::CSceneNode* a_pScene);

    /// \brief update and allow scene inspection
    void InspectMenuNode(LM::CMenuNode* a_pMenuNode);

    /// \brief St the name of the inspector
    /// \param a_rName desired name
    void setInspectorName(const QString& a_rName);

    /// \brief Produce json file base on current BehaviorTree
    /// and save in file name a_rFileName
    void produceJson(const QString& a_rFileame);

    /// \brief search and active a thumbnail by player id and scene id
//    void activeThumbnail(const QString& a_sSceneId, int a_iPlayerId);
    int FindThumbnailIndexByID(const QString& a_sSceneId, int a_iPlayerId);
    void InitiateThumbnails();



private slots:
    void loadExistingProject(const QString& a_sProjectPath);

    void receiveHWND(int);
    void receiveKernel(LM::CKernel*);
    void receiveLabel(LM::CLabelNode*);
    void receiveSprite(LM::CSpriteNode*);
    void receiveMenu(LM::CMenuNode*);
    /// \brief isNav store the fact that the scene is send after pression on
    /// prev or next button. Thus we can know if we need to update thumbnails or not!
    void receiveScene(LM::CSceneNode* a_pScene, bool a_bIsNav);
    void clearInspectorContainer();
    void goToSceneID(CThumbnailWidget* a_pClickedThumbnails);
    void goToNextScene();
    void goToPreviousScene();
//    void updateScene(CThumbnailWidget* a_pNewThumnail);
    void launchEmulator();
    // slot for adding new template
    void addOneScene(const QString& a_sPreviousID, const QString& a_sNewID, int a_iPlayerID, CTemplate* a_pTemplate);
    void addTwoScene(const QString& a_sPreviousIDP1, const QString& a_sNewIDP1,
                     const QString& a_sPreviousIDP2, const QString& a_sNewIDP2,
                     CTemplate* a_pTemplate);
    void addGameScene(const QString& a_sPreviousIDP1, const QString& a_sNewIDP1,
                      const QString& a_sPreviousIDP2, const QString& a_sNewIDP2,
                      CTemplate* a_pTemplate, int a_iTemplateNumberP1, int a_iTemplateNumberP2);
    void addingSceneFinished(std::string a_sSceneID, int a_iPlayerID);
    void deleteScene(QString a_sSceneID, bool a_bIsSync);
    void deletingSceneFinished();
    // Saving slots
    void saveAs();
    void save();
    // Temporary slot
    void launchAddSceneWizard();
    void on_fileBrowser_clicked(const QModelIndex &index);
    void SetCurrentThumbnailIndex(int a_iPlayerID, int a_iNewThumbnailIndex);
    int GetCurrentThumbnailIndex(int a_iPlayerID);
    QList<CThumbnailWidget*>* GetThumbnailList(int a_iPlayerID);
    int GetOtherPlayer(int a_iPlayerID);

private:
    Ui::CMainWindow *ui;
    QFileSystemModel* m_pDirModel;
    QFileSystemModel* m_pFileModel;
    LM::CKernel* m_pKernel;
    enum {PLAYER_1, PLAYER_2, BOTH_PLAYER};
    QProcess m_oProcessServer;
    QProcess m_oProcessClient;
    //DEPRECATED
    CThumbnailWidget* m_pCurrentThumbnailWidget1;
    CThumbnailWidget* m_pCurrentThumbnailWidget2;
    //REPLACED BY
    int m_iCurrentThumbnailIndex1;
    int m_iCurrentThumbnailIndex2;
    QList<CThumbnailWidget*>* m_pThumbnailList1;
    QList<CThumbnailWidget*>* m_pThumbnailList2;
    //
    CTemplateManager* m_pTemplatesManager;
    int m_iActivePlayer;
    QString m_sSaveName;

    int index;
};

#endif // CMAINWINDOW_H
