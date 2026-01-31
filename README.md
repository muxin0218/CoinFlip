A CoinFlip game created by Qt 5.15
基于传智教育《QT从入门到实战》课程的教学案例进行创新：
1. 将原来的4x4网格扩展为6x6 网格，增加了解谜的复杂度。

2. 新增帮助键。使用AI设计算法，引入高斯消元法求解线性方程组。点击 "HELP" 按钮，算法会实时计算当前局面的最优解，并自动演示一步操作。将 6x6 棋盘抽象为 36 个方程的 GF(2) 线性方程组，确保 100%可解。

3. 支持无限步数的撤销和重做。

4. 支持自定义关卡。玩家可从顶部工具栏拖拽金币/银币到棋盘任意位置。


游戏截图
<p align="center">
  <img src="https://github.com/user-attachments/assets/a351d9d6-e39a-4fe6-a24b-c6fdcedf7690" alt="游戏页面" width="30%" />
  &nbsp; &nbsp;
  <img src="https://github.com/user-attachments/assets/c86b1e3d-c2c1-4925-b0a2-75e696210f76" alt="自定义关卡" width="30%" />
</p>

技术栈 
框架：Qt 5 (C++)
核心逻辑：
QStack: 实现撤销/重做。
Vector&Bitset: 实现高斯消元求解器。
QPropertyAnimation: 实现金币翻转与按钮缩放动画。
QTimer: 解决多线程动画冲突与防抖动。
Drag&Drop Events: 实现自定义关卡的拖拽交互。


如何运行

1. 确保本地已安装Qt Creator 及Qt 5.x(MinGW 或 MSVC 编译器)。
2. 克隆本仓库：
   git clone https://github.com/muxin0218/CoinFlip.git
3. 使用Qt Creator打开CoinFlip.pro文件。
4. 点击构建并运行。