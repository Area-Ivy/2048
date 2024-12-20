# 基于 Cocos2d-x 的2048小游戏

## 游戏功能

### 基础功能
- **基础方块**：方块以不同颜色代替传统的数字显示，生成的方块对应的数字为 2 和 4，随机出现在棋盘上的空位。
- **合成规则**：相同数字的方块在滑动时若相撞，则合成一个新的方块，数值为原方块之和。
- **权值计算**：每合成一个新数字，玩家得分为新方块的数字值。

### 音效与音乐
- **背景音乐**：支持背景音乐，以营造舒适的游戏氛围。
- **音效**：方块合成时提供音效。

### 基本界面
- **开始界面**：包含游戏的启动界面，可选择继续游戏或新游戏。
- **游戏界面**：包含游戏界面，显示当前分数和最高分。
- **撤回功能**：支持撤回上一步操作。
- **退出功能**：支持退出游戏。

### 状态恢复
- **恢复功能**：支持中途退出时记录当前状态，下次进入时可以选择继续上一次游戏或重新开始。

### 排行榜
- **排行榜**：支持本地排行榜。

## 方块颜色对应关系

| 数字  | 颜色名称            | RGB 颜色值    | HEX 颜色值 |
|-------|---------------------|---------------|------------|
| 2     | 淡黄色             | (247, 213, 97) | #F7D561    |
| 4     | 浅绿色             | (166, 232, 103)| #A6E867    |
| 8     | 绿色               | (87, 212, 154) | #57D49A    |
| 16    | 青绿色             | (19, 181, 177) | #13B5B1    |
| 32    | 蓝色               | (68, 138, 202) | #448ACA    |
| 64    | 紫色               | (200, 97, 234) | #C863EA    |
| 128   | 粉红色             | (225, 115, 181)| #E173B5    |
| 256   | 鲜艳的粉红色       | (238, 100, 141)| #EE648D    |
| 512   | 橙色               | (243, 157, 79) | #F39D4F    |
| 1024  | 橙色               | (245, 124, 78) | #F57C4E    |
| 2048  | 鲜红色             | (246, 76, 20)  | #F64C14    |

