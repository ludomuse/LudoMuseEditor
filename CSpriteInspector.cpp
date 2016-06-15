#include "CSpriteInspector.h"

// Include QT
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

CSpriteInspector::CSpriteInspector(QWidget *parent):
    QWidget(parent)
{
    // Default constructor
}


CSpriteInspector::CSpriteInspector(LM::CSpriteNode* a_pSprite, QWidget *parent):
    QWidget(parent),
    m_pSprite(a_pSprite)
{
    // Construction de tout les champs swag
    QLabel* text = new QLabel("Ici on aura l'id");
    QLabel* text2 = new QLabel("Ici ou par la le path");

    QVBoxLayout *verticalLayout = new QVBoxLayout();

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(100,100,255));

    this->setLayout(verticalLayout);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->layout()->addWidget(text);
    this->layout()->addWidget(text2);
}
