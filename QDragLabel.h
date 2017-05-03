#ifndef QDRAGLABEL_H
#define QDRAGLABEL_H

#include <QLabel>

class QDragLabel : public QLabel
{
    Q_OBJECT

public:
    QDragLabel(QWidget* parent = nullptr);

    void mousePressEvent(QMouseEvent* event) override;
};

#endif // QDRAGLABEL_H
