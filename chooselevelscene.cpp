#include "chooselevelscene.h"
#include<QMenuBar>
#include<QPainter>
#include"mypushbutton.h"
#include<QLabel>
#include<QDebug>
#include<QSound>
ChooseLevelScene::ChooseLevelScene(QWidget *parent)
    : QMainWindow{parent}
{
    this->setFixedSize(320,588);
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    this->setWindowTitle("CoinFlip");
    QMenuBar* bar=menuBar();
    setMenuBar(bar);
    QMenu* startMenu=bar->addMenu("开始");
    QAction* quitAction=startMenu->addAction("退出");
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //按钮音效
    QSound *chooseSound = new QSound(":/res/TapButtonSound.wav", this);
    QSound *backSound = new QSound(":/res/BackButtonSound.wav", this);

    MyPushButton* btn_back=new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    btn_back->setParent(this);
    btn_back->move(this->width()-btn_back->width(),this->height()-btn_back->height());
    connect(btn_back,&QPushButton::clicked,[=](){
        backSound->play();
        emit this->chooseSceneBack();
    });

    for(int i=0;i<16;i++){
        MyPushButton* btn_menu=new MyPushButton(":/res/LevelIcon.png");
        btn_menu->setParent(this);
        btn_menu->move(25+i%4*70,130+i/4*70);
        connect(btn_menu,&MyPushButton::clicked,[=](){
            //qDebug()<<"选择的是第 "<<i+1<<" 关";
            chooseSound->play();
            this->hide();
            this->play=new PlayScene(i+1);
            play->setGeometry(this->geometry());
            play->show();
            connect(play,&PlayScene::chooseSceneBack,[=](){
                this->setGeometry(play->geometry());
                this->show();
                play->hide();
                delete play;
                play=NULL;
            });


        });
        QLabel* label=new QLabel;
        label->setParent(this);
        label->setFixedSize(btn_menu->width(),btn_menu->height());
        label->setText(QString::number(i+1));
        label->move(25+i%4*70,130+i/4*70);
        label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        label->setAttribute(Qt::WA_TransparentForMouseEvents);

    }

}
void ChooseLevelScene::paintEvent(QPaintEvent*){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    pix.load(":/res/Title.png");
    painter.drawPixmap((this->width()-pix.width())*0.5,30,pix.width(),pix.height(),pix);

}
