# 吃播大作战

Qt6 小游戏 —— 网红良子的吃播挑战！

## 玩法

- **← →** 或 **A D** 控制良子左右移动
- 接住美食加分，躲避蔬菜和减肥药
- 累计 **250 分** 游戏胜利！

## 食物与分数

| 食物 | 分数 |
|------|------|
| 泡面 | +10 |
| 焖子 | +15 |
| 板面 | +20 |
| 可乐 | +5 |

## 陷阱

| 陷阱 | 效果 |
|------|------|
| 蔬菜 | -10 分 |
| 减肥药 | -1 命 |

## 连击

- 连续吃到 3 个焖子 → 屏幕弹出「我爱吃焖子！」
- 连续吃到 3 个板面 → 屏幕弹出「板面！」

## 构建

```bash
mkdir build && cd build
cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=<Qt6路径> ..
cmake --build .
./EatingBattle.exe
```

## 技术栈

- Qt 6.11.0 (QGraphicsView / QGraphicsScene)
- C++17 + CMake
- MinGW 13.1.0
