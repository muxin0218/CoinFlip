#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include<QTimer>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyCoin(QWidget *parent = nullptr);
    MyCoin(QString btnimg);
    int posX;
    int posY;
    bool flag;
    int coinState = 1;
    void changeFlag();
    QTimer* timer1;
    QTimer* timer2;
    int min=1;
    int max=8;
    bool isAnimation=false;
    bool isWin=false;
    bool allowDrop = false;
    void mousePressEvent(QMouseEvent* e);
    void setEmpty();
    void setType(int type);
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

signals:
    void stateChangedWithDrag(int state);
};

#endif // MYCOIN_H
