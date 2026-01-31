A CoinFlip game created by Qt 5.15
基于传智教育《QT从入门到实战》课程的教学案例进行创新：
1. 将原来的4x4网格扩展为6x6 网格，增加了解谜的复杂度。

2. 新增帮助键。使用AI设计算法，引入高斯消元法求解线性方程组。点击 "HELP" 按钮，算法会实时计算当前局面的最优解，并自动演示一步操作。将 6x6 棋盘抽象为 36 个方程的 GF(2) 线性方程组，确保 100%可解。

3. 支持无限步数的撤销和重做。

4. 支持自定义关卡。玩家可从顶部工具栏拖拽金币/银币到棋盘任意位置。


游戏截图
<p align="center">
  <img src="https://github.com/user-attachments/assets/a2201257-55f8-4ed5-9f7f-a4d41ff5b7de" alt="游戏主菜单" width="30%" />
  &nbsp;
  <img src="https://github.com/user-attachments/assets/268c9416-2660-41db-8f57-2fc44b4f40d5" alt="关卡选择" width="30%" />
  &nbsp;
  <img src="https://github.com/user-attachments/assets/0c176e49-370f-4f1d-b460-8b21a6bb771f" alt="游戏实机画面" width="30%" />
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

更新日志
V1.1
全面更换了游戏背景图片（主菜单、关卡选择及游戏场景），视觉风格更加清新统一。