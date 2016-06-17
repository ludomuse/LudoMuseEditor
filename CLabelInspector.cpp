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
    // Construction de tout les champs swag
    QHBoxLayout* hLayoutId= new QHBoxLayout();
    QLineEdit* id = new QLineEdit(this);
    id->setPlaceholderText("id non définie");
    id->setText(QString(this->m_pLabel->GetID().c_str()));
    id->setAlignment(Qt::AlignLeft);
    QLabel* idTitle = new QLabel("ID :");
    idTitle->setAlignment(Qt::AlignRight);
    hLayoutId->addWidget(idTitle);
    hLayoutId->addWidget(id);
    QWidget* idContainer = new QWidget();
    idContainer->setLayout(hLayoutId);
    idContainer->setMaximumHeight(100);
    idContainer->setStyleSheet("border-bottom : 1px solid grey");

    QHBoxLayout* hLayoutText = new QHBoxLayout();
    m_pTextEdit = new QTextEdit(this);
    m_pTextEdit->setText(QString(this->m_pLabel->GetText().c_str()));
    QLabel* textTitle = new QLabel("Texte :");
    textTitle->setAlignment(Qt::AlignTop);
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
    textContainer->setStyleSheet("border-bottom : 1px solid grey");

    QVBoxLayout* vLayoutSize = new QVBoxLayout();
    QHBoxLayout* hLayoutHeight = new QHBoxLayout();
    QHBoxLayout* hLayoutWidth = new QHBoxLayout();
    QWidget* heightContainer = new QWidget();
    QWidget* widthContainer = new QWidget();
    QLabel* widthTitle = new QLabel("Width :");
    QLabel* heightTitle = new QLabel("Height :");
    QSlider*  widthSlider = new QSlider(Qt::Horizontal);
    QSlider*  heightSlider = new QSlider(Qt::Horizontal);
    widthSlider->setTickInterval(1);
    widthSlider->setMaximum(100);
    widthSlider->setMinimum(0);
    widthSlider->setSliderPosition(50);
    heightSlider->setTickInterval(1);
    heightSlider->setMaximum(100);
    heightSlider->setMinimum(0);
    heightSlider->setSliderPosition(50);
    hLayoutWidth->addWidget(widthTitle);
    hLayoutWidth->addWidget(widthSlider);
    hLayoutHeight->addWidget(heightTitle);
    hLayoutHeight->addWidget(heightSlider);
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
    pal.setColor(QPalette::Background, QColor(255,150,255,150));
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
