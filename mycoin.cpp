#include "mycoin.h"

// MyCoin::MyCoin(QWidget *parent)
//     : QWidget{parent}
// {}
MyCoin::MyCoin(QString btnimg){
    QPixmap pix;
    bool ret=pix.load(btnimg);
    if(!ret){
        pix.load(":/res/Coin0001.png");
    }
    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    if(ret) {
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(), pix.height()));
    }
    this->setAcceptDrops(true);
    this->coinState = 1;
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);
    connect(timer1,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str=QString(":/res/Coin000%1").arg(min++);
        pix.load(str);
        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        if(min>max){
            min=1;
            isAnimation=false;
            timer1->stop();
        }
    });
    connect(timer2,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str=QString(":/res/Coin000%1").arg(max--);
        pix.load(str);
        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        if(max<min){
            max=8;
            isAnimation=false;
            timer2->stop();
        }
    });
}

void MyCoin::changeFlag(){
    if(flag){
        timer1->start(30);
    }
    else{
        timer2->start(30);
    }
    isAnimation=true;
    flag=!flag;
}
void MyCoin::mousePressEvent(QMouseEvent* e){
    if(isAnimation || this->isWin){
        return;
    }
    else{
        QPushButton::mousePressEvent(e);
    }
}

void MyCoin::setEmpty() {
    this->coinState = -1;
    this->setIcon(QIcon());
    this->setStyleSheet("QPushButton{border:0px; background-color: transparent;}");
}

// 设置为金币或银币
void MyCoin::setType(int type) {
    this->coinState = type;
    this->flag = (type == 1);
    QString imgPath = (type == 1) ? ":/res/Coin0001.png" : ":/res/Coin0008.png";
    QPixmap pix;
    pix.load(imgPath);
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(), pix.height()));
    this->setStyleSheet("QPushButton{border:0px;}");
}

// 拖拽进入
void MyCoin::dragEnterEvent(QDragEnterEvent *event) {
    if (this->allowDrop && event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

// 拖拽放下
void MyCoin::dropEvent(QDropEvent *event) {
    QString typeStr = event->mimeData()->text();
    int newType = typeStr.toInt(); // 获取拖过来的类型 0 或 1
    // 改变自身状态
    setType(newType);
    // 发送信号给 PlayScene 改数据
    emit stateChangedWithDrag(newType);

    event->acceptProposedAction();
}
