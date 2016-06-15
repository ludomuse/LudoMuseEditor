#include "CLabelInspector.h"

// Include QT
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

CLabelInspector::CLabelInspector(QWidget *parent):
    QWidget(parent)
{
    // Default constructor
}


CLabelInspector::CLabelInspector(LM::CLabelNode* a_pLabel, QWidget *parent):
    QWidget(parent),
    m_pLabel(a_pLabel)
{
    // Construction de tout les champs swag
    QHBoxLayout* hLayoutId= new QHBoxLayout();
    QLineEdit* id = new QLineEdit(this);
    id->setText(QString(this->m_pLabel->GetID().c_str()));
    id->setAlignment(Qt::AlignLeft);
    QLabel* idTitle = new QLabel("ID :");
    idTitle->setAlignment(Qt::AlignRight);
    hLayoutId->addWidget(idTitle);
    hLayoutId->addWidget(id);
    QWidget* idContainer = new QWidget();
    idContainer->setLayout(hLayoutId);
    idContainer->setMaximumHeight(100);

    QHBoxLayout* hLayoutText = new QHBoxLayout();
    QLabel* text = new QLabel(QString(this->m_pLabel->GetText().c_str()));
    QLabel* textTitle = new QLabel("Texte :");
    hLayoutText->addWidget(textTitle);
    hLayoutText->addWidget(text);
    QWidget* textContainer = new QWidget();
    QPalette palTextContainer(palette());
    palTextContainer.setColor(QPalette::Background, QColor(255,255,255));
    text->setAutoFillBackground(true);
    text->setPalette(palTextContainer);
    text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    textTitle->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    textContainer->setLayout(hLayoutText);
    textContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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
    pal.setColor(QPalette::Background, QColor(255,150,255));

    this->setLayout(verticalLayout);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->layout()->addWidget(idContainer);
    this->layout()->addWidget(textContainer);
    this->layout()->addWidget(buttonContainer);

}
