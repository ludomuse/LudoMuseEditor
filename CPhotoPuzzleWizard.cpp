#include "CPhotoPuzzleWizard.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QDrag>
#include <QMouseEvent>
#include <QMimeData>


CPhotoPuzzleWizard::CPhotoPuzzleWizard(QWidget* parent):
    ui(new Ui_Dialog)
{
    ui->setupUi(this);
    setModal(true);
    setAcceptDrops(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(clickOnValidate(bool)));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(clickOnCancel(bool)));

    connect(ui->ToFillColSpinner, SIGNAL(valueChanged(int)), this, SLOT(GenerateGrids()));
    connect(ui->ToFillRowsSpinner, SIGNAL(valueChanged(int)), this, SLOT(GenerateGrids()));
    connect(ui->PiecesColSpinner, SIGNAL(valueChanged(int)), this, SLOT(GenerateGrids()));
    connect(ui->PiecesRowsSpinner, SIGNAL(valueChanged(int)), this, SLOT(GenerateGrids()));

    GenerateGrids();
}


void CPhotoPuzzleWizard::ClearLayout(QLayout* layout, bool deleteWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                delete widget;
        }
        if (QLayout* childLayout = item->layout())
            ClearLayout(childLayout, deleteWidgets);
        delete item;
    }
}


void CPhotoPuzzleWizard::GenerateGrids()
{
    ClearLayout(ui->ToFillGridLayout, true);
    ClearLayout(ui->PiecesGridLayout, true);
    m_vPiecesGridItems.clear();
    m_vToFillGridItems.clear();

    ui->PiecesColSpinner->setMinimum(1);
    ui->PiecesRowsSpinner->setMinimum(1);
    ui->PiecesColSpinner->setMaximum(ui->ToFillColSpinner->value());
    ui->PiecesRowsSpinner->setMaximum(ui->ToFillRowsSpinner->value());

    ui->ToFillColSpinner->setMinimum(1);
    ui->ToFillRowsSpinner->setMinimum(1);
    ui->ToFillColSpinner->setMaximum(5);
    ui->ToFillRowsSpinner->setMaximum(5);

    for (int row = 0; row < ui->ToFillRowsSpinner->value(); ++row)
    {
        for (int col = 0; col < ui->ToFillColSpinner->value(); ++col)
        {
            QLabel* label = new QLabel(this);
            label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            label->setMinimumHeight(20);
            label->setMinimumWidth(20);
            label->setText(QString::number(row * ui->ToFillColSpinner->value() + col + 1));

            if (row >= m_vToFillGridItems.size())
            {
                m_vToFillGridItems.append(QVector<QLabel*>());
            }

            m_vToFillGridItems[row].append(label);

            ui->ToFillGridLayout->addWidget(label, row, col);
        }
    }

    for (int row = 0; row < ui->PiecesRowsSpinner->value(); ++row)
    {
        for (int col = 0; col < ui->PiecesColSpinner->value(); ++col)
        {
            QComboBox* combo = new QComboBox(this);
            combo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

            combo->setAcceptDrops(true);

            for (int i = 0; i < m_vToFillGridItems.size() * m_vToFillGridItems.at(0).size(); ++i)
            {
                combo->addItem(QString::number(i + 1));
            }

            if (row >= m_vPiecesGridItems.size())
            {
                m_vPiecesGridItems.append(QVector<QComboBox*>());
            }

            m_vPiecesGridItems[row].append(combo);

            ui->PiecesGridLayout->addWidget(combo, row, col);
        }
    }
}


void CPhotoPuzzleWizard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {

        QLabel* label = dynamic_cast<QLabel*>(childAt(event->pos()));
        if (label && label->text() != "_")
        {
            QDrag* drag = new QDrag(this);
            QMimeData* mimeData = new QMimeData;
            mimeData->setText(label->text());
            drag->setMimeData(mimeData);

            Qt::DropAction dropAction = drag->exec();
        }
    }
}


void CPhotoPuzzleWizard::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
        event->acceptProposedAction();
}

void CPhotoPuzzleWizard::dropEvent(QDropEvent *event)
{
    QComboBox* combo = dynamic_cast<QComboBox*>(childAt(event->pos()));
    if (combo)
    {
        combo->setCurrentIndex(event->mimeData()->text().toInt() - 1);

        event->acceptProposedAction();
    }
}


void CPhotoPuzzleWizard::clickOnCancel(bool)
{
    qDebug() << "cancel";
    close();
}

void CPhotoPuzzleWizard::clickOnValidate(bool)
{
    qDebug()<< "Wizard ok";


}
