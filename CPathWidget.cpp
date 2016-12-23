#include "CPathWidget.h"

#include <QLayout>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QDebug>

#include "CLineEdit.h"
#include "CProjectManager.h"

CPathWidget::CPathWidget(const QString& a_sFirstValue, const QString &a_sExtensions, QWidget* a_pParent):
    QWidget(a_pParent)
{
    QHBoxLayout* pathLayout = new QHBoxLayout(this);
    QString sExtensions = a_sExtensions;
    sExtensions = sExtensions.remove(QRegExp("[().,*]"));
    QStringList lExtensions = sExtensions.split(" ");
    CLineEdit* path = new CLineEdit(lExtensions, this);
    path->setPlaceholderText("id non définie");
    path->setText(QString(a_sFirstValue));
    path->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    path->setAttribute(Qt::WA_TranslucentBackground, false);

    QPushButton* pathFileDialogButton = new QPushButton(this);
    pathFileDialogButton->setText("...");
    pathFileDialogButton->setMaximumWidth(30);
    pathFileDialogButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    pathLayout->addWidget(path);
    pathLayout->addWidget(pathFileDialogButton);
    this->setLayout(pathLayout);

//    connect(m_pPathField, SIGNAL(textChanged(QString)), this, SLOT(newPathDragged(QString)));
//    connect (this, SIGNAL(pathChanged(QString)), m_pPathField, SLOT(setText(QString)));
//    connect (m_pPathField, SIGNAL(textEdited(QString)), this, SLOT(newPathSelected(QString)));
    connect(this, SIGNAL(pathChanged(QString)), path, SLOT(setText(QString)));
    connect(path, SIGNAL(textEdited(QString)), this, SIGNAL(pathChanged(QString)));
    connect(pathFileDialogButton, SIGNAL(clicked(bool)), this, SLOT(openPathFileDialog()));
}

void CPathWidget::openPathFileDialog()
{
    QFileDialog* fileDialog = new QFileDialog();

    QDir currentDir = QDir(CProjectManager::Instance()->QGetProjectPath());

//    QStringList pathToImage = m_sPath.split("/");

//    for (int i = 0; i < pathToImage.length() - 1; ++i)
//    {
//        currentDir.cd(pathToImage[i]);
//    }
    fileDialog->setDirectory(currentDir);
//    fileDialog->selectFile(pathToImage[pathToImage.size() - 1]);
    fileDialog->setNameFilter(m_sExtensions);

    connect(fileDialog, SIGNAL(directoryEntered(QString)), this, SLOT(newDirectoryEntered(QString)));
    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(newPathSelected(QString)));
    fileDialog->show();
}

void CPathWidget::newPathSelected(QString a_sPath)
{
    QDir currentDir = QDir::currentPath();
    QString currentPath = currentDir.absolutePath();
    a_sPath.remove(currentPath + "/");
//    this->m_pPath->setText(a_sPath);
    qDebug()<<"etablish new path via file windonw -"<<a_sPath;
//    m_pPathField->setPath(a_sPath);
    emit pathChanged(a_sPath);
}

void CPathWidget::newDirectoryEntered(QString a_sPath)
{
    QString sProjectPath = CProjectManager::Instance()->QGetProjectPath();
    if (!a_sPath.contains(sProjectPath))
    {
        QFileDialog* fileDialog = dynamic_cast<QFileDialog*>(QObject::sender());
        if (fileDialog != Q_NULLPTR)
        {
            fileDialog->setDirectory(sProjectPath);
        }
    }
}

void CPathWidget::SetPath(const QString &a_sNewPath)
{
    emit pathChanged(a_sNewPath);
}
