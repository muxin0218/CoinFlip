#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include"mycoin.h"
#include<QStack>
#include<QPoint>
#include<mypushbutton.h>
#include<QLabel>
#include<bitset>
#include "draggablecoin.h"
class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    //explicit PlayScene(QWidget *parent = nullptr);
    PlayScene(int levelNum);
    int levelIndex;
    QLabel* winLabel = NULL;
    void paintEvent(QPaintEvent*);
    int gameArray[6][6];
    MyCoin* coinBtn[6][6];
    bool isWin;
    bool isProcessing = false;
    MyPushButton* btnUndo;
    MyPushButton* btnRedo;
    MyPushButton* btn_help;
    MyPushButton* btn_back;
    MyPushButton* btnStartCustom;
    QStack<QPoint> mUndoStack; // 撤回栈
    QStack<QPoint> mRedoStack; // 恢复栈
    void updateUndoRedoButtons();
    void flipCoinOp(int row, int col);
    using Row = std::bitset<37>;
    std::vector<std::pair<int, int>> solve(int grid[6][6]);
    bool isCustomMode=false;
    DraggableCoin* toolGold;
    DraggableCoin* toolSilver;
    QLabel* toolLabel; // 提示文字

signals:
    void chooseSceneBack();
};

#endif // PLAYSCENE_H
