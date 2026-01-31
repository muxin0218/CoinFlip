#include "draggablecoin.h"

DraggableCoin::DraggableCoin(QWidget *parent, bool flag) : QLabel(parent)
{
    this->flag = flag;

    QString imgPath = flag ? ":/res/Coin0001.png" : ":/res/Coin0008.png";
    QPixmap pix;
    pix.load(imgPath);
    this->setPixmap(pix);
    this->setFixedSize(50, 50);
}

void DraggableCoin::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if(this->pixmap() == nullptr) return;
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData();
        // 将硬币类型 (1或0) 作为文本传递
        mimeData->setText(QString::number(this->flag));
        drag->setMimeData(mimeData);
        // 设置拖拽时的跟手图片
        drag->setPixmap(*this->pixmap());
        drag->setHotSpot(event->pos()); // 鼠标在图片的位置
        // 开始拖拽
        drag->exec(Qt::CopyAction | Qt::MoveAction);
    }
}
