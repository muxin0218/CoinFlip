#ifndef DRAGGABLECOIN_H
#define DRAGGABLECOIN_H

#include <QLabel>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
class DraggableCoin : public QLabel
{
    Q_OBJECT
public:
    bool flag;
    DraggableCoin(QWidget *parent, bool flag);
    void mousePressEvent(QMouseEvent *event) override;

signals:
};

#endif // DRAGGABLECOIN_H
