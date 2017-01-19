#include "CLabelInspector.h"

// Include kernel to have ON_CC_THREAD macro
#include "Classes/Engine/Include/CKernel.h"

// Include QT
#include <QDebug>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>

#include "CAnchorWidget.h"
#include "CLineEdit.h"
#include "CPathWidget.h"
#include "CColorButton.h"

CLabelInspector::CLabelInspector(QWidget *parent):
    QWidget(parent)
{
    // Default constructor
}


CLabelInspector::CLabelInspector(LM::CLabelNode* a_pLabel, QWidget *parent):
    QWidget(parent),
    m_pLabel(a_pLabel),
    m_pTextEdit(Q_NULLPTR),

    m_sSavedText(a_pLabel->GetText()),
    m_iSavedHeight(a_pLabel->GetHeight()),
    m_iSavedWidth(a_pLabel->GetWidth()),
    m_iSavedAnchor(a_pLabel->GetAnchor()),
    m_sSavedFont(a_pLabel->GetFont()),
    m_iSavedFontSize(a_pLabel->GetFontSize()),
    m_sSavedColor(a_pLabel->GetColor())
{
    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

    // Construction de tout les champs swag
    QHBoxLayout* hLayoutId= new QHBoxLayout();
    QLineEdit* id = new QLineEdit(this);
    id->setPlaceholderText("id non définie");
    id->setText(QString(this->m_pLabel->GetID().c_str()));
    id->setAlignment(Qt::AlignLeft);
    QLabel* idTitle = new QLabel("ID :");
    idTitle->setAlignment(Qt::AlignRight);
    idTitle->setStyleSheet("QLabel{color : white;}");
    hLayoutId->addWidget(idTitle);
    hLayoutId->addWidget(id);
    QWidget* idContainer = new QWidget();
    idContainer->setLayout(hLayoutId);
    idContainer->setMaximumHeight(100);
    idContainer->setStyleSheet("border-bottom : 1px solid rgb(150,150,150)");

    // Champ de texte
    QHBoxLayout* hLayoutText = new QHBoxLayout();
    m_pTextEdit = new QTextEdit(this);
    m_pTextEdit->setText(QString(this->m_pLabel->GetText().c_str()));
    QLabel* textTitle = new QLabel("Texte :");
    textTitle->setAlignment(Qt::AlignTop);
    textTitle->setStyleSheet("QLabel{color : white;}");
    QWidget* textContainer = new QWidget();
    QPalette palTextContainer(palette());
    palTextContainer.setColor(QPalette::Background, QColor(255,255,255));
    m_pTextEdit->setAutoFillBackground(true);
    m_pTextEdit->setPalette(palTextContainer);
    m_pTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    textTitle->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    hLayoutText->addWidget(textTitle);
    hLayoutText->addWidget(m_pTextEdit);
    textContainer->setLayout(hLayoutText);
    textContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    textContainer->setStyleSheet("border-bottom : 1px solid rgb(150,150,150)");

    // Choix de la police
    QHBoxLayout* hLayoutPath= new QHBoxLayout();
    QLabel* pathTitle = new QLabel("Police :");
    pathTitle->setStyleSheet("QLabel{color : white;}");
    CPathWidget* pathWidget = new CPathWidget(QString::fromStdString(m_sSavedFont), ETypes::Font, this);
//    pathWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    hLayoutPath->addWidget(pathTitle);
    hLayoutPath->addWidget(pathWidget);
    QWidget* pathContainer = new QWidget();
    pathContainer->setLayout(hLayoutPath);
    pathContainer->setMaximumHeight(100);
//    pathContainer->setStyleSheet("border-bottom : 1px solid grey");
    pathContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    // Choix de la taille d'écriture
    QHBoxLayout* hLayoutFontSize= new QHBoxLayout();
    QLabel* fontSizeTitle = new QLabel("Taille :");
    fontSizeTitle->setStyleSheet("QLabel{color : white;}");
    QSpinBox* fontSizeWidget = new QSpinBox(this);
    fontSizeWidget->setRange(6, 100);
    fontSizeWidget->setValue(m_pLabel->GetFontSize());
    QLabel* colorTitle = new QLabel("Couleur :");
    colorTitle->setStyleSheet("QLabel{color : white;}");
    CColorButton* colorButton = new CColorButton(QString::fromStdString(m_pLabel->GetColor()), this);
    hLayoutFontSize->addWidget(fontSizeTitle);
    hLayoutFontSize->addWidget(fontSizeWidget);
    hLayoutFontSize->addWidget(colorTitle);
    hLayoutFontSize->addWidget(colorButton);
    QWidget* fontSizeContainer = new QWidget();
    fontSizeContainer->setLayout(hLayoutFontSize);
    fontSizeContainer->setMaximumHeight(100);
    fontSizeContainer->setStyleSheet("border-bottom : 1px solid grey");
    fontSizeContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    // Create anchor widget
    QLabel* anchorTitle = new QLabel("Ancrage");
    anchorTitle->setAlignment(Qt::AlignCenter);
    anchorTitle->setStyleSheet("QLabel{color : white;}");
    CAnchorWidget* anchorWidget = new CAnchorWidget(m_iSavedAnchor, this);
    QVBoxLayout* vLayoutAnchor = new QVBoxLayout();
    vLayoutAnchor->addWidget(anchorTitle);
    vLayoutAnchor->addWidget(anchorWidget);
    QWidget* anchorContainer = new QWidget(this);
    anchorContainer->setLayout(vLayoutAnchor);
    anchorContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Height and Width widget
    QVBoxLayout* vLayoutSize = new QVBoxLayout();
    QHBoxLayout* hLayoutHeight = new QHBoxLayout();
    QHBoxLayout* hLayoutWidth = new QHBoxLayout();
    QWidget* heightContainer = new QWidget();
    heightContainer->setObjectName("heightContainer");
    QWidget* widthContainer = new QWidget();
    widthContainer->setObjectName("widthContainer");
    QLabel* widthTitle = new QLabel("Largeur :");
    widthTitle->setStyleSheet("QLabel{color : white;}");
    QLabel* heightTitle = new QLabel("Hauteur :");
    heightTitle->setStyleSheet("QLabel{color : white;}");
    m_pWidthSlider = new QSlider(Qt::Horizontal);
    m_pHeightSlider = new QSlider(Qt::Horizontal);
    m_pWidthSlider->setTickInterval(1);
    m_pWidthSlider->setMaximum(100);
    m_pWidthSlider->setMinimum(0);
    m_pWidthSlider->setSliderPosition(m_pLabel->GetWidth());
    m_pHeightSlider->setTickInterval(1);
    m_pHeightSlider->setMaximum(100);
    m_pHeightSlider->setMinimum(0);
    m_pHeightSlider->setSliderPosition(m_pLabel->GetHeight());
    m_pWidthValue = new QLineEdit(QString::number(m_pLabel->GetWidth()));
    m_pWidthValue->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pWidthValue->setStyleSheet("QLineEdit{width : 25px;}");
    m_pHeightValue = new QLineEdit(QString::number(m_pLabel->GetHeight()));
    m_pHeightValue->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pHeightValue->setStyleSheet("QLineEdit{width : 25px;}");
    hLayoutWidth->addWidget(widthTitle);
    hLayoutWidth->addWidget(m_pWidthSlider);
    hLayoutWidth->addWidget(m_pWidthValue);
    hLayoutHeight->addWidget(heightTitle);
    hLayoutHeight->addWidget(m_pHeightSlider);
    hLayoutHeight->addWidget(m_pHeightValue);
    heightContainer->setLayout(hLayoutHeight);
    widthContainer->setLayout(hLayoutWidth);
    QWidget* sizeContainer = new QWidget();
    vLayoutSize->addWidget(heightContainer);
    vLayoutSize->addWidget(widthContainer);
    sizeContainer->setLayout(vLayoutSize);
    sizeContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QHBoxLayout* hLayoutButton = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("Ok");
    QPushButton* backButton = new QPushButton("Retour");
    hLayoutButton->addWidget(backButton);
    hLayoutButton->addWidget(okButton);
    QWidget* buttonContainer = new QWidget();
    buttonContainer->setLayout(hLayoutButton);
    buttonContainer->setMaximumHeight(100);

    QVBoxLayout* verticalLayout = new QVBoxLayout();
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(50,50,50,255));
    this->setLayout(verticalLayout);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->layout()->addWidget(idContainer);
    this->layout()->addWidget(textContainer);
    this->layout()->addWidget(pathContainer);
    this->layout()->addWidget(fontSizeContainer);
    this->layout()->addWidget(anchorContainer);
    this->layout()->addWidget(sizeContainer);
    this->layout()->addWidget(buttonContainer);

    // Connect SLOT and SIGNAL
    connect(m_pTextEdit, SIGNAL(textChanged()), this, SLOT(changeText()));
    connect(backButton, SIGNAL(clicked(bool)), this, SLOT(discardChanges()));
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(validateChanges()));

    connect(anchorWidget, SIGNAL(anchorChanged(int)), this, SLOT(changeAnchor(int)));
    connect(pathWidget, SIGNAL(pathChanged(QString)), this, SLOT(changePath(QString)));
    connect(fontSizeWidget, SIGNAL(valueChanged(int)), this, SLOT(changeFontSize(int)));
    connect(colorButton, SIGNAL(colorChanged(QString)), this, SLOT(changeColor(QString)));

    // Connect slider and text value
    connect(m_pHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(heightSliderChange(int)));
    connect(m_pWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(widthSliderChange(int)));
    connect(m_pHeightValue, SIGNAL(textChanged(QString)), this, SLOT(heightTextChange(QString)));
    connect(m_pWidthValue, SIGNAL(textChanged(QString)), this, SLOT(widthTextChange(QString)));

}

void CLabelInspector::validateChanges()
{
    m_sSavedText = m_pLabel->GetText();
    m_iSavedHeight = m_pLabel->GetHeight();
    m_iSavedWidth = m_pLabel->GetWidth();
    m_iSavedAnchor = m_pLabel->GetAnchor();
    m_sSavedFont = m_pLabel->GetFont();
    m_iSavedFontSize = m_pLabel->GetFontSize();
    m_sSavedColor = m_pLabel->GetColor();
    emit modifyLabel(m_pLabel);
    emit closeInspector();
}

void CLabelInspector::discardChanges()
{
    emit closeInspector();
}

void CLabelInspector::changeText()
{
    QString content = this->m_pTextEdit->toPlainText();
    m_pLabel->SetText(content.toStdString());
//    ON_CC_THREAD(LM::CLabelNode::SetText, this->m_pLabel, content.toStdString());
}

void CLabelInspector::changePath(const QString& a_sPath)
{
    m_pLabel->SetFont(a_sPath.toStdString());
}

void CLabelInspector::changeFontSize(int a_iSize)
{
    m_pLabel->SetFontSize(a_iSize);
}

void CLabelInspector::changeColor(const QString &a_sColor)
{
    m_pLabel->SetColor(a_sColor.toStdString());
}

void CLabelInspector::changeAnchor(int a_iAnchorID)
{
//    ON_CC_THREAD(LM::CLabelNode::ChangeAnchor, this->m_pLabel, a_iAnchorID);
    m_pLabel->ChangeAnchor(a_iAnchorID);
}

void CLabelInspector::heightSliderChange(int a_iValue)
{
    this->m_pHeightValue->setText(QString::number(a_iValue));
    this->m_pLabel->SetHeight(a_iValue);
}

void CLabelInspector::widthSliderChange(int a_iValue)
{
    this->m_pWidthValue->setText(QString::number(a_iValue));
    this->m_pLabel->SetWidth(a_iValue);
}

void CLabelInspector::heightTextChange(const QString& a_rText)
{
    // Try convert string to int
    bool succes;
    int value = a_rText.toInt(&succes);
    if(succes)
    {
        m_pHeightSlider->setSliderPosition(value);
        m_pLabel->SetHeight(value);
    }
}

void CLabelInspector::widthTextChange(const QString& a_rText)
{
    // Try convert string to int
    bool succes;
    int value = a_rText.toInt(&succes);
    if(succes)
    {
        m_pWidthSlider->setSliderPosition(value);
        m_pLabel->SetWidth(value);
    }
}

void CLabelInspector::closeEvent (QCloseEvent *event)
{
//    ON_CC_THREAD(LM::CLabelNode::SetText, this->m_pLabel, m_sSavedText);
    m_pLabel->SetText(m_sSavedText);
    m_pLabel->SetWidth(m_iSavedWidth);
    m_pLabel->SetHeight(m_iSavedHeight);
//    ON_CC_THREAD(LM::CLabelNode::ChangeAnchor, m_pLabel, m_iSavedAnchor);
    m_pLabel->ChangeAnchor(m_iSavedAnchor);
//    discardChanges();
    m_pLabel->SetFont(m_sSavedFont);
    m_pLabel->SetFontSize(m_iSavedFontSize);
    m_pLabel->SetColor(m_sSavedColor);
    QWidget::closeEvent(event);
}
