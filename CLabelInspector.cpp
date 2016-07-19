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

CLabelInspector::CLabelInspector(QWidget *parent):
    QWidget(parent)
{
    // Default constructor
}


CLabelInspector::CLabelInspector(LM::CLabelNode* a_pLabel, QWidget *parent):
    QWidget(parent),
    m_pLabel(a_pLabel),
    m_pTextEdit(Q_NULLPTR)
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
//    QVBoxLayout* vLayoutSize = new QVBoxLayout();
//    QHBoxLayout* hLayoutHeight = new QHBoxLayout();
//    QHBoxLayout* hLayoutWidth = new QHBoxLayout();
//    QWidget* heightContainer = new QWidget();
//    QWidget* widthContainer = new QWidget();
//    QLabel* widthTitle = new QLabel("Largeur :");
//    widthTitle->setStyleSheet("QLabel{color : white;}");
//    QLabel* heightTitle = new QLabel("Hauteur :");
//    heightTitle->setStyleSheet("QLabel{color : white;}");
//    QSlider*  widthSlider = new QSlider(Qt::Horizontal);
//    QSlider*  heightSlider = new QSlider(Qt::Horizontal);
//    widthSlider->setTickInterval(1);
//    widthSlider->setMaximum(100);
//    widthSlider->setMinimum(0);
//    widthSlider->setSliderPosition(50);
//    heightSlider->setTickInterval(1);
//    heightSlider->setMaximum(100);
//    heightSlider->setMinimum(0);
//    heightSlider->setSliderPosition(50);
//    hLayoutWidth->addWidget(widthTitle);
//    hLayoutWidth->addWidget(widthSlider);
//    hLayoutHeight->addWidget(heightTitle);
//    hLayoutHeight->addWidget(heightSlider);
//    heightContainer->setLayout(hLayoutHeight);
//    widthContainer->setLayout(hLayoutWidth);
//    QWidget* sizeContainer = new QWidget();
//    vLayoutSize->addWidget(heightContainer);
//    vLayoutSize->addWidget(widthContainer);
//    sizeContainer->setLayout(vLayoutSize);
//    sizeContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);


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
    this->layout()->addWidget(sizeContainer);
    this->layout()->addWidget(buttonContainer);

    // Connect SLOT and SIGNAL
    connect(m_pTextEdit, SIGNAL(textChanged()), this, SLOT(changeText()));
    connect(backButton, SIGNAL(clicked(bool)), this, SLOT(closeInspectorSlot()));
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(closeInspectorSlot()));

    // Connect slider and text value
    connect(m_pHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(heightSliderChange(int)));
    connect(m_pWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(widthSliderChange(int)));
    connect(m_pHeightValue, SIGNAL(textChanged(QString)), this, SLOT(heightTextChange(QString)));
    connect(m_pWidthValue, SIGNAL(textChanged(QString)), this, SLOT(widthTextChange(QString)));
}

void CLabelInspector::closeInspectorSlot()
{
    emit closeInspector();
}

void CLabelInspector::changeText()
{
    QString content = this->m_pTextEdit->toPlainText();
    qDebug()<<"Changement de text"<<content;
    ON_CC_THREAD(LM::CLabelNode::SetText, this->m_pLabel, content.toStdString());
}

void CLabelInspector::heightSliderChange(int a_iValue)
{
    qDebug()<< "Change slider height value to:"<<a_iValue;
    this->m_pHeightValue->setText(QString::number(a_iValue));
    this->m_pLabel->SetHeight(a_iValue);
}

void CLabelInspector::widthSliderChange(int a_iValue)
{
    qDebug()<< "Change slider width value to:"<<a_iValue;
    this->m_pWidthValue->setText(QString::number(a_iValue));
    this->m_pLabel->SetWidth(a_iValue);
}

void CLabelInspector::heightTextChange(const QString& a_rText)
{
    qDebug()<< "Change height value to:"<<a_rText;
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
    qDebug()<< "Change width value to:"<<a_rText;
    // Try convert string to int
    bool succes;
    int value = a_rText.toInt(&succes);
    if(succes)
    {
        m_pWidthSlider->setSliderPosition(value);
        m_pLabel->SetWidth(value);
    }
}

