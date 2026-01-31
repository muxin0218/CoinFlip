#include "mainscene.h"
#include "ui_mainscene.h"
#include<QPainter>
#include"mypushbutton.h"
#include<QTimer>
#include<QSound>
MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
    //配置主场景，1. 固定大小;2.设置图标； 3.设置标题
    setFixedSize(320,588);
    setWindowIcon(QIcon(":/res/Coin0001.png"));
    setWindowTitle("CoinFlip");

    connect(ui->actionquit,&QAction::triggered,[=](){
        this->close();
    });

    MyPushButton *startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move((this->width() - startBtn->width()) * 0.5, this->height() * 0.7);

    chooseScene=new ChooseLevelScene;
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,[=](){
        this->setGeometry(chooseScene->geometry());
        chooseScene->hide();
        this->show();
    });
    QSound* startSound = new QSound(":/res/TapButtonSound.wav", this);
    //startSound->setLoops(-1);//-1为无限循环
    connect(startBtn,&MyPushButton::clicked,[=](){
        startSound->play();
        startBtn->zoom_s();
        startBtn->zoom_w();
        QTimer::singleShot(300,this,[=](){
            this->hide();
            //进入选择关卡的场景
            chooseScene->setGeometry(this->geometry());
            chooseScene->show();
        });
    });
}

MainScene::~MainScene()
{
    delete ui;
}

void MainScene::paintEvent(QPaintEvent*){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/MenuSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    pix.load(":/res/Title.png");
    pix=pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix);


}
