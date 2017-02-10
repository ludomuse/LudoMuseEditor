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
#include "CThumbnailsLoaderThread.h"
#include "CArchiver.h"
#include "CTimelineWidget.h"

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

    enum {PLAYER_1, PLAYER_2, BOTH_PLAYER};

private:

    /// \brief Populate both area scroll for each player time line
    void ProcessTree();

    /// \brief return playerId who have the scene matching the param id
    int ScreenIDToPlayerID(const QString &a_id);

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

    /// \Displays current scene
    void ShowCurrentScene();

private slots:
    void loadExistingProject(const QString& a_sProjectPath);

    void receiveLabel(LM::CLabelNode*);
    void receiveSprite(LM::CSpriteNode*);
    void receiveMenu(LM::CMenuNode*);
    /// \brief isNav store the fact that the scene is send after pression on
    /// prev or next button. Thus we can know if we need to update thumbnails or not!
    void receiveScene(LM::CSceneNode* a_pScene, bool a_bIsNav);
    void clearInspectorContainer();
    void clearSceneInspector();
    void goToNextScene();
    void goToPreviousScene();
    void goToDashBoard();
    void goToWaitingScreen();
    void changeScene();
    void launchEmulator();
    // slot for adding new template
    void addOneScene(const QString& a_sPreviousID, const QString& a_sNewID, int a_iPlayerID, CTemplate* a_pTemplate);
    void addSharedScene(const QString& a_sPreviousIDP1, const QString& a_sPreviousIDP2,
                        const QString& a_sNewID, CTemplate* a_pTemplate);
    void addTwoScene(const QString& a_sPreviousIDP1, const QString& a_sNewIDP1,
                     const QString& a_sPreviousIDP2, const QString& a_sNewIDP2,
                     CTemplate* a_pTemplate);
    void addGameScene(const QString& a_sPreviousIDP1, const QString& a_sNewIDP1,
                      const QString& a_sPreviousIDP2, const QString& a_sNewIDP2,
                      CTemplate* a_pTemplate, int a_iTemplateNumberP1, int a_iTemplateNumberP2);
    void deleteScene(QString a_sSceneID, bool a_bIsSync);
    void addingSceneFinished(const QString& a_sPrevSceneID, const QString& a_sSceneID, int a_iPlayerID);
    void addingSharedSceneFinished(const QString& a_sPrevSceneID1, const QString& a_sPrevSceneID2, const QString& a_sSceneID);
    void deletingSceneFinished(const QString a_sSceneID);
    // Saving slots
    void saveAs();
    void save();
    void exportProject(const QString& a_rDestination);
    // Temporary slot
    void launchAddSceneWizard();
//    void on_fileBrowser_clicked(const QModelIndex &index);
    void loadCapture(QString a_sScreenName);
    void saveCapture();
    void nodeModified(LM::CEntityNode* a_pNode);
    void nodeSoundModified(LM::CEntityNode* a_pNode, const QString& a_sEvent, const QString& a_sArgument);
    void nodeSoundRemoved(LM::CEntityNode* a_pNode);
    void reloadScene();
    void on_archiveButton_clicked();

private:
    Ui::CMainWindow *ui;
//    QFileSystemModel* m_pDirModel;
//    QFileSystemModel* m_pFileModel;
    LM::CKernel* m_pKernel;
    QProcess m_oProcessServer;
    QProcess m_oProcessClient;

    CTemplateManager* m_pTemplatesManager;

    QString m_sSaveName;
    CThumbnailsLoaderThread *m_pLoader;
    CArchiver m_oArchiver;
    CTimelineWidget* m_pTimeline;
};

#endif // CMAINWINDOW_H
