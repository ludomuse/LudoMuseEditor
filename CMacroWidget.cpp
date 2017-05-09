#include "CMacroWidget.h"

#include <map>
#include <iterator>
#include <QBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QApplication>
#include <QPainter>
#include <CFindFilenameVisitor.h>
#include <QMessageBox>
#include "LudoMuse_src/Classes/Engine/Include/CMacroManager.h"

CMacroWidget::CMacroWidget(QWidget *a_pParent):
    QWidget(a_pParent),
    m_pKernel(nullptr)
{

}

void CMacroWidget::SetKernel(LM::CKernel* a_pKernel)
{
    m_pKernel = a_pKernel;
}

void CMacroWidget::AddRow(ETypes::Type a_eType, const QString &a_sMacro/*, const QString &a_sDefine*/)
{
    QGridLayout* pGridLayout = GetLayout(a_eType);
    if (pGridLayout == Q_NULLPTR)
    {
        return;
    }
    int iRow = pGridLayout->rowCount();
    CMacroLabel* pMacro = new CMacroLabel(a_sMacro, a_eType, this);
    CDefineEdit* pDefine = new CDefineEdit(a_sMacro, a_eType, this);
//    pDefine->setPlaceholderText(tr("Chemin du fichier"));
    QToolButton* pDel = new QToolButton(this);
    pDel->setText("-");
    pMacro->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    pDefine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    pDel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    pGridLayout->addWidget(pMacro, iRow, 0);
    pGridLayout->addWidget(pDefine, iRow, 1);
    pGridLayout->addWidget(pDel, iRow, Qt::AlignRight);

    pDefine->SetKernel(m_pKernel);

    connect(pDel, SIGNAL(pressed()), pDefine, SLOT(deleteIfUnused()));
    connect(pDel, SIGNAL(pressed()), this, SIGNAL(deleteClicked()));
    connect(pDefine, SIGNAL(deleteConfirmed()), pDefine, SLOT(deleteLater()));
    connect(pDefine, SIGNAL(deleteConfirmed()), pMacro, SLOT(deleteLater()));
    connect(pDefine, SIGNAL(deleteConfirmed()), pDel, SLOT(deleteLater()));
    connect(pDefine, SIGNAL(deleteConfirmed()), pDefine, SLOT(removeDefine()));
    connect(pDefine, SIGNAL(textEdited(QString)), pDefine, SLOT(setDefine(QString)));
    connect(pDefine, SIGNAL(selectionChanged()), this, SIGNAL(macroModified()));
}


QGridLayout* CMacroWidget::GetLayout(ETypes::Type a_eType)
{
    switch (a_eType)
    {
    case ETypes::Type::Image:
        return m_pImageLayout;
    case ETypes::Type::Sound:
        return m_pSoundLayout;
    case ETypes::Type::Font:
        return m_pFontLayout;
    default:
        return Q_NULLPTR;
    }
}

void CMacroWidget::Init()
{
    QWidget* imageList = new QWidget(this);
    m_pImageLayout = new QGridLayout(imageList);
    m_pImageLayout->addWidget(new QLabel(tr("Images :")),0,0);
    m_pImageLayout->setColumnStretch(0, 2);
    m_pImageLayout->setColumnStretch(1, 3);
    imageList->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    QWidget* fontList = new QWidget(this);
    m_pFontLayout = new QGridLayout(fontList);
    m_pFontLayout->addWidget(new QLabel(tr("Polices :")),0,0);
    m_pFontLayout->setColumnStretch(0, 2);
    m_pFontLayout->setColumnStretch(1, 3);
    fontList->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    QWidget* soundList = new QWidget(this);
    m_pSoundLayout = new QGridLayout(soundList);
    m_pSoundLayout->addWidget(new QLabel(tr("Sons :")),0,0);
    m_pSoundLayout->setColumnStretch(0, 2);
    m_pSoundLayout->setColumnStretch(1, 3);
    soundList->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    QWidget* macroAdd = new QWidget(this);
    QHBoxLayout* addRowLayout = new QHBoxLayout(macroAdd);
    m_pMacroName = new QLineEdit(macroAdd);
    m_pMacroName->setPlaceholderText("Nom de la macro");
    m_pMacroType = new QComboBox(this);
    m_pMacroType->addItem("Image", QVariant("image"));
    m_pMacroType->addItem("Police", QVariant("font"));
    m_pMacroType->addItem("Son", QVariant("sound"));
    QToolButton* pAdd = new QToolButton(this);
    pAdd->setText("+");
    addRowLayout->addWidget(m_pMacroType);
    addRowLayout->addWidget(m_pMacroName);
    addRowLayout->addWidget(pAdd);
    connect(pAdd, SIGNAL(pressed()), this, SLOT(addRow()));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(macroAdd);
    mainLayout->addWidget(imageList);
    mainLayout->addWidget(fontList);
    mainLayout->addWidget(soundList);
    mainLayout->setAlignment(macroAdd, Qt::AlignTop);
    mainLayout->setAlignment(imageList, Qt::AlignTop);
    mainLayout->setAlignment(fontList, Qt::AlignTop);
    mainLayout->setAlignment(soundList, Qt::AlignTop);
    setLayout(mainLayout);

    std::map<std::string, std::string>::const_iterator itBegin;
    std::map<std::string, std::string>::const_iterator itEnd;
    LM::CMacroManager::Instance()->GetIterator(&itBegin, &itEnd);
    std::map<std::string, std::string>::const_iterator itr;
    for (itr = itBegin; itr != itEnd; itr ++)
    {
        QString sExtension = (QString::fromStdString((*itr).second).split('.')).last();
        AddRow(ETypes::TypeFromExtension(sExtension),
               QString::fromStdString((*itr).first));
    }
}

void CMacroWidget::addRow()
{
    QString sName = m_pMacroName->text();
    if (sName.isEmpty())
    {
        return;
    }
    else if (sName.at(0) != '#')
    {
        sName.insert(0, '#');
    }
    if (LM::CMacroManager::Instance()->HasDefinition(sName.toStdString()))
    {
        return;
    }
    QString sType = m_pMacroType->currentData().toString();
    AddRow(ETypes::TypeFromString(sType), sName);
}

CDefineEdit::CDefineEdit(const QString& a_sMacro, ETypes::Type a_eType, QWidget *a_pParent):
    CLineEdit(a_eType, a_pParent),
    m_sMacro(a_sMacro)
{
    if (LM::CMacroManager::Instance()->HasDefinition(m_sMacro.toStdString()))
    {
        setText(QString::fromStdString(LM::CMacroManager::Instance()->GetDefinition(m_sMacro.toStdString())));
    }
    else
    {
        LM::CMacroManager::Instance()->AddDefinition(m_sMacro.toStdString(), "");
    }
}

void CDefineEdit::SetKernel(LM::CKernel *a_pKernel)
{
    m_pKernel = a_pKernel;
}

QString CDefineEdit::GetMacro()
{
    return m_sMacro;
}

void CDefineEdit::removeDefine()
{
    LM::CMacroManager::Instance()->RemoveDefinition(m_sMacro.toStdString());
}

void CDefineEdit::setDefine(const QString& a_sDef)
{
    LM::CMacroManager::Instance()->AddDefinition(m_sMacro.toStdString(), a_sDef.toStdString());
}

void CDefineEdit::deleteIfUnused()
{
    qDebug() << "delete if unused";
    CFindFilenameVisitor* oVisitor = new CFindFilenameVisitor(m_sMacro);
    oVisitor->Traverse(m_pKernel->m_pBehaviorTree);
    QStringList* vScenes = oVisitor->GetScenesID();
    if (vScenes->empty())
    {
        emit deleteConfirmed();
    }
    else
    {
        QMessageBox msgBox;
        QString sScenes = QString(vScenes->join('\n'));
        msgBox.setText(tr("Suppression impossible. La macro est utilisée dans les scènes suivantes :\n")
                       + sScenes);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();
    }
}

CMacroLabel::CMacroLabel(const QString &a_sText, ETypes::Type a_sType, QWidget *a_pParent):
    QLabel(a_sText, a_pParent),
    m_sType(a_sType)
{
    setStyleSheet("CMacroLabel { border: 1px solid black }");
}

void CMacroLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_pStartDragPos = event->pos();
}

void CMacroLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
    {
        return;
    }
    if ((event->pos() - m_pStartDragPos).manhattanLength()
            < QApplication::startDragDistance())
    {
        return;
    }
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(this->text()+":"+ETypes::TypeToString(m_sType));
    drag->setMimeData(mimeData);
    QPixmap pixmap = grab();
    QPixmap temp(pixmap.size());
    temp.fill(Qt::transparent);
    QPainter p(&temp);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.drawPixmap(0, 0, pixmap);
    p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p.fillRect(temp.rect(), QColor(0, 0, 0, 150));
    p.end();
    pixmap = temp;

    drag->setPixmap(pixmap);
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
}

//CMacroLine::CMacroLine(CMacroLabel *a_pLabel, CDefineEdit *a_pDefine, QToolButton *a_pDelete, QObject *parent):
//    QObject(),
//    m_pLabel(a_pLabel),
//    m_pDefine(a_pDefine),
//    m_pDelete(a_pDelete)
//{
//    connect(m_pDelete, SIGNAL(clicked(bool)), this, SLOT(deleteIfUnused()));
//}

//void CMacroLine::deleteIfUnused()
//{

//}
