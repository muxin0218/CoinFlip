#include "playscene.h"
#include<QMenuBar>
#include<QPainter>
#include"mypushbutton.h"
#include"mycoin.h"
#include"dataconfig.h"
#include<QDebug>
#include<QPropertyAnimation>
#include<QSound>
#include <QMessageBox>
// PlayScene::PlayScene(QWidget *parent)
//     : QMainWindow{parent}
// {}
PlayScene::PlayScene(int levelNum){
    this->levelIndex=levelNum;
    this->setFixedSize(320,588);
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    QString levelStr=QString("Level %1").arg(levelIndex);
    this->setWindowTitle(levelStr);
    QMenuBar* bar=menuBar();
    setMenuBar(bar);
    QMenu* startMenu=bar->addMenu("开始");
    QAction* quitAction=startMenu->addAction("退出");
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //按钮音效
    QSound* backSound = new QSound(":/res/BackButtonSound.wav", this);

    //初始化胜利图片
    winLabel = new QLabel(this);
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0, tmpPix.width(), tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->move((this->width() - tmpPix.width())*0.5, -tmpPix.height());

    QString strUndoNormal = ":/res/Undo_Inactive.png";
    QString strRedoNormal = ":/res/Redo_Inactive.png";
    btnUndo = new MyPushButton(strUndoNormal);
    btnUndo->setParent(this);
    btnUndo->move(119, 506);
    btnRedo = new MyPushButton(strRedoNormal);
    btnRedo->setParent(this);
    btnRedo->move(170, 506);
    //Undo键
    connect(btnUndo, &MyPushButton::clicked, [=](){
        if(this->isProcessing) {
            return;
        }
        if(mUndoStack.isEmpty())
            return;
        backSound->play();
        QPoint p = mUndoStack.pop();
        //放入 Redo 栈
        mRedoStack.push(p);
        // 执行反转
        flipCoinOp(p.x(), p.y());
        //更新按钮状态
        updateUndoRedoButtons();
    });
    //Redo键
    connect(btnRedo,&MyPushButton::clicked,[=](){
        if(this->isProcessing) {
            return;
        }
        if(mRedoStack.isEmpty())
            return;
        backSound->play();
        QPoint p = mRedoStack.pop();
        mUndoStack.push(p);
        flipCoinOp(p.x(),p.y());
        updateUndoRedoButtons();
    });

    //返回键
    btn_back=new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    btn_back->setParent(this);
    btn_back->move(this->width()-btn_back->width(),this->height()-btn_back->height());
    connect(btn_back,&QPushButton::clicked,[=](){
        backSound->play();
        QTimer::singleShot(300, this, [=](){
            emit this->chooseSceneBack();
        });
    });

    //帮助键
    btn_help=new MyPushButton(":/res/HelpButton.png");
    btn_help->setParent(this);
    btn_help->move(1,this->height()-btn_help->height()-2);
    connect(btn_help,&QPushButton::clicked,[=](){

        backSound->play();
        if(this->isProcessing) {
            return;
        }
        int grid[6][6];
        for(int i=0;i<6;i++){
            for(int j=0;j<6;j++){
                grid[i][j]=coinBtn[i][j]->flag;
            }
        }
        std::vector<std::pair<int, int>> steps = solve(grid);
        if (steps.empty())
            return;
        std::sort(steps.begin(), steps.end(), [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
            if (a.second != b.second) {
                return a.second < b.second;
            }
            return a.first < b.first;
        });
        int ansX = steps.front().first;
        int ansY = steps.front().second;
        flipCoinOp(ansX, ansY);
        mUndoStack.push(QPoint(ansX, ansY));
        mRedoStack.clear();
        updateUndoRedoButtons();
    });


    if(this->levelIndex == 16) {
        this->isCustomMode = true;
        btnUndo->hide();
        btnRedo->hide();
        btn_help->hide();
        // 提示文字
        toolLabel = new QLabel(this);
        toolLabel->setText("拖拽硬币至棋盘");
        toolLabel->setFont(QFont("华文新魏", 15));
        toolLabel->adjustSize();
        toolLabel->move(52, 100);

        // 金币源
        toolGold = new DraggableCoin(this, 1);
        toolGold->move(112, 144);

        // 银币源
        toolSilver = new DraggableCoin(this, 0);
        toolSilver->move(162, 144);

        //开始按钮
        btnStartCustom = new MyPushButton(":/res/MenuSceneStartButton.png");
        btnStartCustom->setParent(this);
        btnStartCustom->move((this->width() - btnStartCustom->width())*0.5, this->height() - 100);

        connect(btnStartCustom, &MyPushButton::clicked, [=](){
            //检查是否填满
            bool isFull = true;
            for(int r=0; r<6; r++){
                for(int c=0; c<6; c++){
                    if(coinBtn[c][r]->coinState == -1){
                        isFull = false;
                        break;
                    }
                }
            }

            if(!isFull) {
                // 没填满，不可以开始
                backSound->play();
                QMessageBox::warning(this,"提示","棋盘还没摆满！\n请将所有方格填满后再开始挑战。");
                return;
            }
            QSound* startSound = new QSound(":/res/TapButtonSound.wav", this);
            startSound->play();

            this->isCustomMode = false;
            btnStartCustom->hide();
            toolLabel->hide();
            toolGold->hide();
            toolSilver->hide();
            btnUndo->show();
            btnRedo->show();
            btn_help->show();
            for(int r=0; r<6; r++){
                for(int c=0; c<6; c++){
                    gameArray[r][c] = coinBtn[r][c]->coinState;
                }
            }
        });
    }


    dataConfig config;
    for(int i=0;i<6;i++){
        for(int j=0;j<6;j++){
            this->gameArray[i][j]=config.mData[this->levelIndex][j][i];
        }
    }

    //显示金币背景
    for(int i=0;i<6;i++){
        for(int j=0;j<6;j++){
            QLabel* label=new QLabel;
            label->setGeometry(0,0,50,50);
            label->setPixmap(QPixmap(":/res/BoardNode.png"));
            label->setParent(this);
            label->move(10+i*50,200+j*50);

            //创建金币
            QString coinstr;
            if(this->levelIndex == 16) {
                coinstr = "";
            }
            else{
                if(gameArray[i][j]==1){
                    coinstr=":/res/Coin0001.png";
                }
                else{
                    coinstr=":/res/Coin0008.png";
                }
            }

            MyCoin* coin=new MyCoin(coinstr);
            if(this->levelIndex == 16) {
                coin->setEmpty();
                coin->allowDrop = true;
                connect(coin, &MyCoin::stateChangedWithDrag, [=](){
                    if(this->isCustomMode) {
                        QSound* placeSound = new QSound(":/res/TapButtonSound.wav", this);
                        placeSound->play();
                    }
                });
            }
            coin->setParent(this);
            coin->move(12+i*50,204+j*50);
            coin->posX=i;
            coin->posY=j;
            //1为正面，0为反面
            coin->flag=gameArray[i][j];
            coinBtn[i][j]=coin;
            //反转硬币
            connect(coin,&MyCoin::clicked,[=](){
                if(this->isProcessing) {
                    return;
                }
                if(this->isCustomMode) {
                    return;
                }
                mUndoStack.push(QPoint(i, j));
                mRedoStack.clear();
                flipCoinOp(i, j);
                updateUndoRedoButtons();
            });
        }
    }

}
void PlayScene::paintEvent(QPaintEvent*){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width()*0.6, pix.height()*0.6);
    painter.drawPixmap(10, 30, pix);
}
void PlayScene::updateUndoRedoButtons(){
    QString undoOn = ":/res/Undo_Active.png";
    QString undoOff = ":/res/Undo_Inactive.png";
    QString redoOn = ":/res/Redo_Active.png";
    QString redoOff = ":/res/Redo_Inactive.png";
    QPixmap pix;
    if(mUndoStack.isEmpty())
        btnUndo->updateImg(undoOff);
    else
        btnUndo->updateImg(undoOn);
    if(mRedoStack.isEmpty())
        btnRedo->updateImg(redoOff);
    else
        btnRedo->updateImg(redoOn);
}
void PlayScene::flipCoinOp(int i, int j){
    this->isProcessing = true;
    QSound *flipSound = new QSound(":/res/ConFlipSound.wav", this);
    QSound *winSound = new QSound(":/res/LevelWinSound.wav", this);
    flipSound->play();

    coinBtn[i][j]->changeFlag();
    gameArray[i][j] = gameArray[i][j] == 0 ? 1 : 0;
    QTimer::singleShot(300,this,[=](){
        //延时反转周围硬币
        if(i+1<6){
            coinBtn[i+1][j]->changeFlag();
            gameArray[i+1][j]=gameArray[i+1][j]==0 ? 1 : 0;
        }
        if(i-1>=0){
            coinBtn[i-1][j]->changeFlag();
            gameArray[i-1][j]=gameArray[i-1][j]==0 ? 1 : 0;
        }
        if(j+1<6){
            coinBtn[i][j+1]->changeFlag();
            gameArray[i][j+1]=gameArray[i][j+1]==0 ? 1 : 0;
        }
        if(j-1>=0){
            coinBtn[i][j-1]->changeFlag();
            gameArray[i][j-1]=gameArray[i][j-1]==0 ? 1 : 0;
        }
        this->isProcessing = false;
        this->isWin = true;
        for(int row = 0;row < 6;row++){
            for(int col = 0;col < 6;col++){
                if(coinBtn[row][col]->flag == false){
                    this->isWin = false;
                    break;
                }
            }
        }
        if(this->isWin == true){
            //禁用所有金币动画
            for(int row = 0;row < 6;row++){
                for(int col = 0;col < 6;col++){
                    coinBtn[row][col]->isWin = true;
                }
            }
            winSound->play();
            if(btnUndo) btnUndo->setEnabled(false);
            if(btnRedo) btnRedo->setEnabled(false);
            QPropertyAnimation* animation=new QPropertyAnimation(winLabel,"geometry");
            animation->setDuration(500);
            animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
            animation->setEndValue(QRect(winLabel->x(),winLabel->y()+182,winLabel->width(),winLabel->height()));
            animation->setEasingCurve(QEasingCurve::OutBounce);
            animation->start();
        }
    });
}

std::vector<std::pair<int, int>> PlayScene::solve(int grid[6][6]) {
    std::vector<Row> matrix(36);
    int N = 6;

    // 1. 构建增广矩阵
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            int i = r * 6 + c;
            // 系数矩阵 A
            matrix[i][i] = 1;
            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};
            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k];
                int nc = c + dc[k];
                if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                    matrix[i][nr*6+nc] = 1;
                }
            }

            // 结果向量 b (目标是全1，所以 b = 1 ^ current)
            matrix[i][36] = 1 ^ grid[r][c];
        }
    }

    // 2. 高斯消元
    int r = 0;
    for (int c = 0; c < 36 && r < 36; c++) {
        int pivot = r;
        while (pivot < 36 && !matrix[pivot][c]) pivot++;
        if (pivot == 36) continue;

        std::swap(matrix[r], matrix[pivot]);

        for (int i = 0; i < 36; i++) {
            if (i != r && matrix[i][c]) matrix[i] ^= matrix[r];
        }
        r++;
    }

    // 3. 解析结果
    std::vector<std::pair<int, int>> steps;
    for (int i = 0; i < 36; i++) {
        if (matrix[i][36]) {
            steps.push_back({i / N, i % N}); // 返回 (x, y)
        }
    }
    return steps;
}
