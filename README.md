# my2048
 For the final project of the first semester
 [思路来源](https://github.com/mevdschee/2048.c)
 [视频参考](https://www.bilibili.com/video/BV1GY4y1p78Z)

 ## 基本要求

1. 游戏画面绘制
2. 游戏规则实现
3. 游戏数据存储与读取
4. 玩家排行榜



## Part1：游戏画面绘制

游戏画面是通过EasyX图形库实现的，包括以下函数：

1. **初始化窗口**`initWindows`：
   - 设置背景颜色，初始化棋盘。
   - 随机生成两个初始数字。
   - 方格坐标确定
2. **绘制棋盘**`drawBoard`：
   - 每个格子根据其数字设置不同颜色。
   - 使用`solidrectangle`绘制无边框矩形，`outtextxy`绘制数字。
3. **双缓冲绘图**：
   - 使用`BeginBatchDraw`和`EndBatchDraw`减少闪屏。

4. **绘制棋盘尺寸计算**



## Part2：游戏规则实现

​	在2048的实现中，游戏规则的核心包括以下几个方面：数字生成、数字合并、画面更新，以及游戏胜负状态的判断。

### 数字生成

游戏的核心是随机生成数字并放置在棋盘上。

1. **随机数生成函数`randomNums`**：
   - 使用时间作为种子，确保每次运行都生成不同的随机数。
   - 80%概率生成数字2，20%概率生成数字4。
2. **随机位置分配函数`randomPos`**：
   - 随机选取一个空位置放置数字。
   - 如果棋盘已满，函数不会陷入死循环。
3. **数字召唤逻辑`numSummon`**：
   - 在玩家每次有效操作后，调用`randomPos`生成一个新数字。

### 数字移动

在2048中，玩家通过上下左右滑动操作，使相邻且相同的数字合并，并将分数加到总分中。

- **遍历方向**：根据玩家输入的方向（上、下、左、右），对棋盘进行不同方向的遍历。
- **压缩格子**：将非零数字移动到目标方向上，形成“紧凑排列”的效果。
- **合并数字**：对于排列相邻且相同的数字，将它们相加，并将结果放置在前一个位置，同时将后一个位置置为0。
- **再次压缩**：合并后再进行一次压缩，确保所有数字靠拢。

### 画面更新

玩家每次操作后，系统会自动更新画面状态，包括：

- **生成新数字**：
  - 在空白位置随机生成一个“2”或“4”。
  - 调用`randomPos`函数确保随机性，同时避免操作无效时生成新数字。
- **刷新显示**：
  - 每次更新画面后，重新绘制游戏界面，包括当前分数和棋盘内容。

### 游戏胜负判断

**胜利条件**：

- 如果棋盘中出现`2048`，调用`isGameWin`函数判断玩家是否获胜。
- 一旦获胜，可以弹出提示，玩家选择继续游戏或退出。

**失败条件**：

- 检查棋盘是否还有空位置。如果没有空位置，再判断是否存在相邻相同的数字。
- 如果两者都不满足，游戏结束。

## Part3：游戏数据存储与读取

使用了文件操作实现了游戏退出存档以及再次进入游戏的读档功能。

1. **存储游戏状态**`saveGameState`：
   - 把当前分数、棋盘数据写入文件`game_state.txt`。
2. **加载历史记录**`loadSaveData`：
   - 检查是否有未完成的游戏。
   - 提示玩家选择是否加载历史记录。



## Part4：玩家排行榜

使用了链表来存储玩家数据，支持增加删除与修改。

1. **链表管理**：
   - 每个玩家数据通过结构体`Player`表示。
   - 链表操作包括插入新记录和排序。
2. **冒泡排序`listSort`**：
   - 对链表按分数从高到低排序，确保排行榜显示最优成绩。
3. **显示排行榜`displayRankings`**：
   - 使用图形界面显示前10名玩家信息。
