#include "GameLayer.h"
#include "GameTool.h"
#include "DataConf.h"
#include "AudioEngine.h"
using namespace cocos2d::experimental;

GameLayer* GameLayer::_instance = nullptr;
int isMove = 0; // 大于0时，表示移动了当前的数字布局，因此需要自动保存上一次的布局结果, 在移动完毕后在RandGenGrid中进行自动清0
GameLayer* GameLayer::getInstance()
{
    if (_instance == nullptr)
        _instance = create();
    return _instance;
}

// 初始化
bool GameLayer::init()
{
    do {
        CC_BREAK_IF(!Layer::init());

        this->setContentSize(Size(300, 300));
        this->setPosition(10, 65);
        // 初始化 背景
        initBg();
    } while (0);
    return true;
}

void GameLayer::initBg()
{

    // init the rand seed
    struct timeval now;
    gettimeofday(&now, NULL);
    srand((unsigned int)(now.tv_sec * 1000 + now.tv_usec / 1000));

    auto bg = LayerColor::create(Color4B(186, 172, 159, 255), 300, 300);
    this->addChild(bg);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            auto cellBg = LayerColor::create(Color4B(213, 205, 194, 255), 65, 65);
            cellBg->setPosition(73 * j + 8, 73 * i + 8);
            this->addChild(cellBg);
        }
    }
    // 加载格子
    loadGrids(2);
}
// 初始化格子
void GameLayer::initGrids()
{
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            _grids[row][column] = nullptr;
        }
    }
    clearLastGrids();
    randGenGrid();
    randGenGrid();
}

// 加载格子方法
void GameLayer::loadGrids(int type)
{
    _isOver = false;
    auto f = UserDefault::getInstance();
    // 如果XML文件不存在或者指定类型的游戏数据不存在，就初始化网格
    if (!f->isXMLFileExist() || !f->getBoolForKey(Value(type).asString().append("exits").c_str(), false))
        return initGrids();

    int value = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // 从UserDefault中获取指定位置的数值
            value = f->getIntegerForKey(Value(type * 3).asString().append(Value(100 + i * 4 + j).asString()).c_str());
            if (value == -1)
                _grids[i][j] = nullptr;
            else
            {
                _grids[i][j] = Grid::create();
                _grids[i][j]->initValue(value, i, j);
                this->addChild(_grids[i][j]);
            }
            // 记录上一个网格的数值
            _lastGrids[i][j] = f->getIntegerForKey(Value(type * 3).asString().append(Value(i * 4 + j).asString()).c_str());
        }
    }
}

//清理格子
void GameLayer::clearGrids()
{
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            if (_grids[row][column] != nullptr)
                _grids[row][column]->removeFromParent();
            _grids[row][column] = nullptr;
        }
    }
}

#define EMPTY -1 // 表示lastGrids中保存的是上次的布局
#define DOUBLE_EMPTY -2 // 表示_lastGrids中保存的是初始化的结果
void GameLayer::clearLastGrids()
{
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            _lastGrids[row][column] = DOUBLE_EMPTY;
        }
    }
}

// 重新开始游戏
void GameLayer::restartGame()
{
    _isOver = false;
    GameTool::getInstance()->resetScore();
    clearGrids();
    clearLastGrids();
    randGenGrid();
    randGenGrid();
}

// 撤销
void GameLayer::undoGame()
{
    if (_lastGrids[0][0] != DOUBLE_EMPTY) // 非初始化时，才能调用undo
    {
        clearGrids();
        recoverLastGrids();
    }
}

void GameLayer::saveLastGrids()
{
    // 如果移动次数不等于1，则不保存上一个网格状态
    if (++isMove != 1)
        return;

    // 遍历当前网格，保存每个格子的数值到_lastGrids数组中
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            if (_grids[row][column] != nullptr)
                _lastGrids[row][column] = _grids[row][column]->getScoreValue();
            else
                _lastGrids[row][column] = EMPTY;
        }
    }
}

void GameLayer::recoverLastGrids()
{
    // 遍历上一个网格状态数组，根据数值恢复每个格子的状态
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            if (_lastGrids[row][column] > EMPTY)
            {
                // 如果上一个网格状态不为空，则创建新的格子并初始化数值
                _grids[row][column] = Grid::create();
                _grids[row][column]->initValue(_lastGrids[row][column], row, column);
                this->addChild(_grids[row][column]);
            }
            else
            {
                // 如果上一个网格状态为空，则置为nullptr
                _grids[row][column] = nullptr;
            }
        }
    }
}

void GameLayer::randGenGrid()
{
    isMove = 0; // 重置移动次数为0，标记为从新开始判断是否移动
    int value, row, column;
    do {
        value = CCRANDOM_0_1() * 16;
        row = value / 4;
        column = value % 4;
    } while (_grids[row][column] != nullptr);

    auto item = Grid::create();
    item->initValue((int)(CCRANDOM_0_1() * 2), row, column);
    _grids[row][column] = item;
    item->initAction();
    this->addChild(item);
}

void GameLayer::onEnter()
{
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameLayer::onExit()
{
    Layer::onExit();
    // 在退出游戏界面时，保存数据
    /*log("eixt%d", DataConf::getInstance()->getModelType());*/
    DataConf::getInstance()->dumpData(Grid::getType());
}

bool GameLayer::onTouchBegan(Touch* touch, Event* event)
{
    if (_isOver) // 如果游戏结束，则重新开始游戏
    {
        auto fail = this->getChildByName("fail");
        fail->setVisible(false);
        restartGame();
        return false;
    }

    _begin = touch->getLocation();
    auto rect = Rect(this->getPosition().x, this->getPosition().y, this->getContentSize().width, this->getContentSize().height);
    return rect.containsPoint(_begin); // 只有在点击游戏区域内才响应触摸事件
}
void GameLayer::onTouchMoved(Touch* touch, Event* event)
{
}
void GameLayer::onTouchEnded(Touch* touch, Event* event)
{
    _end = touch->getLocation();
    auto value = _end - _begin;

    // 判断横向移动还是纵向移动
    if (abs(value.x) > abs(value.y)) // 左右移动
    {
        if (value.x - 5 > 0) // 向右移动
            moveToRight();
        else if (value.x + 5 < 0) // 向左移动
            moveToLeft();
        else
            return;
    }
    else // 上下移动
    {
        if (value.y - 5 > 0) // 向上移动
            moveToTop();
        else if (value.y + 5 < 0) // 向下移动
            moveToBottom();
        else
            return;
    }

    if (isMove > 0) // 如果有移动，则生成新的格子
        randGenGrid();

    // 判断游戏是否结束
    _isOver = ifOver();
}

bool GameLayer::ifOver()
{
    // 当存在空格或者有两个相邻的 grid 数字相同时，表示游戏没有结束
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            // 如果当前 grid 为空，则游戏未结束
            if (_grids[row][column] == nullptr)
                return false;

            // 如果当前 grid 与右侧或下侧的 grid 数字相同，则游戏未结束
            if (_grids[row][column]->compareTo(_grids[row][column + 1]) || _grids[row][column]->compareTo(_grids[row + 1][column]))
                return false;
        }
    }

    for (int column = 0; column < 3; column++)
    {
        // 如果最下面一行的当前 grid 为空或者与右侧 grid 数字相同，则游戏未结束
        if (_grids[3][column] == nullptr || _grids[3][column]->compareTo(_grids[3][column + 1]))
            return false;
    }

    for (int row = 0; row < 3; row++)
    {
        // 如果最右侧一列的当前 grid 为空或者与下侧 grid 数字相同，则游戏未结束
        if (_grids[row][3] == nullptr || _grids[row][3]->compareTo(_grids[row + 1][3]))
            return false;
    }

    // game over and show the fail scene
    auto fail = this->getChildByName("fail");
    if (fail == nullptr)
    {
        // 创建一个半透明的灰色图层，并添加“Game Over!”标签
        fail = LayerColor::create(Color4B(144, 144, 144, 144), 300, 300);
        auto flabel = Label::create("Game Over!", "Bold", 50);
        flabel->setPosition(150, 150);
        fail->addChild(flabel);
        fail->setName("fail");
        this->addChild(fail);
    }
    fail->setVisible(true);
    return true;
}

#define ERRORINDEX -1
int GameLayer::getPreGridIndex(const int direction, int row, int column)
{
    switch (direction)
    {
        case Direction::LEFT:
            // 向左查找最近的非空 grid 的列索引
            while (--column >= 0)
                if (_grids[row][column] != nullptr)
                    return column;
            break;
        case Direction::RIGHT:
            // 向右查找最近的非空 grid 的列索引
            while (++column <= 3)
                if (_grids[row][column] != nullptr)
                    return column;
            break;
        case Direction::UP:
            // 向上查找最近的非空 grid 的行索引
            while (++row <= 3)
                if (_grids[row][column] != nullptr)
                    return row;
            break;
        case Direction::DOWN:
            // 向下查找最近的非空 grid 的行索引
            while (--row >= 0)
                if (_grids[row][column] != nullptr)
                    return row;
            break;
    }
    return ERRORINDEX;
}

// move the grid to target place and return the pre_grid index
int GameLayer::moveGrid(const int direction, int row, int column, int targetRow, int targetColumn)
{
    int addScore = 0;

    int preColumnOrRow = getPreGridIndex(direction, row, column);
    if (preColumnOrRow == ERRORINDEX)
    {
        // 直接移动到目标位置
        moveOnly(row, column, targetRow, targetColumn);
        return ERRORINDEX;
    }

    int preRow, preCol;
    int tag = -1;
    switch (direction)
    {
        case Direction::LEFT:
            tag = 1;
        case Direction::RIGHT:
            preRow = row;
            preCol = preColumnOrRow;
            break;
        case Direction::DOWN:
            tag = 1;
        case Direction::UP:
            preRow = preColumnOrRow;
            preCol = column;
            break;
    }

    // 判断是否可以合并两个数字
    if (_grids[row][column]->compareTo(_grids[preRow][preCol]))
    {
        auto value = _grids[row][column]->getScoreValue();
        // 移动并清除原位置
        moveAndClear(row, column, targetRow, targetColumn);
        moveAndClear(preRow, preCol, targetRow, targetColumn);
        // 移动并更新合并后的数字
        moveAndUpdate(++value, targetRow, targetColumn);
        AudioEngine::play2d("sound_click.mp3", false); // 第二个参数 true 表示循环播放
        GameTool::getInstance()->updateScore(pow(2, ++value));
        return preColumnOrRow;
    }

    // 直接移动到目标位置
    moveOnly(row, column, targetRow, targetColumn);
    return preColumnOrRow + tag;
}bool GameLayer::moveToRight()
{
    // 目标列索引初始为 3（最右侧）
    auto target = 3;
    for (int row = 0; row < 4; row++)
    {
        // 每次进入新的一行，重新将目标列索引设为 3
        target = 3;
        for (int column = 3; column >= 0; column--)
        {
            // 如果当前 grid 为空，跳过
            if (_grids[row][column] == nullptr)
                continue;
            // 尝试向右移动当前 grid，移动后得到新的列索引
            column = moveGrid(Direction::LEFT, row, column, row, target);
            // 如果移动失败（返回 ERRORINDEX），退出当前行的循环
            if (column == ERRORINDEX)
                break;
            // 更新目标列索引，向左侧移动一格
            target--;
        }
    }
    // 如果有移动发生（isMove > 0），返回 true，否则返回 false
    return isMove > 0;
}

bool GameLayer::moveToLeft()
{
    // 目标列索引初始为 0（最左侧）
    auto target = 0;
    for (int row = 0; row < 4; row++)
    {
        // 每次进入新的一行，重新将目标列索引设为 0
        target = 0;
        for (int column = 0; column < 4; column++)
        {
            // 如果当前 grid 为空，跳过
            if (_grids[row][column] == nullptr)
                continue;
            // 尝试向左移动当前 grid，移动后得到新的列索引
            column = moveGrid(Direction::RIGHT, row, column, row, target);
            // 如果移动失败（返回 ERRORINDEX），退出当前行的循环
            if (column == ERRORINDEX)
                break;
            // 更新目标列索引，向右侧移动一格
            target++;
        }
    }
    // 如果有移动发生（isMove > 0），返回 true，否则返回 false
    return isMove > 0;
}

bool GameLayer::moveToTop()
{
    // 目标行索引初始为 3（最上方）
    int target = 3;
    for (int column = 0; column < 4; column++)
    {
        // 每次进入新的一列，重新将目标行索引设为 3
        target = 3;
        for (int row = 3; row >= 0; row--)
        {
            // 如果当前 grid 为空，跳过
            if (_grids[row][column] == nullptr)
                continue;
            // 尝试向上移动当前 grid，移动后得到新的行索引
            row = moveGrid(Direction::DOWN, row, column, target, column);
            // 如果移动失败（返回 ERRORINDEX），退出当前列的循环
            if (row == ERRORINDEX)
                break;
            // 更新目标行索引，向下方移动一格
            target--;
        }
    }
    // 如果有移动发生（isMove > 0），返回 true，否则返回 false
    return isMove > 0;
}

bool GameLayer::moveToBottom()
{
    // 目标行索引初始为 0（最下方）
    int target = 0;
    for (int column = 0; column < 4; column++)
    {
        // 每次进入新的一列，重新将目标行索引设为 0
        target = 0;
        for (int row = 0; row < 4; row++)
        {
            // 如果当前 grid 为空，跳过
            if (_grids[row][column] == nullptr)
                continue;
            // 尝试向下移动当前 grid，移动后得到新的行索引
            row = moveGrid(Direction::UP, row, column, target, column);
            // 如果移动失败（返回 ERRORINDEX），退出当前列的循环
            if (row == ERRORINDEX)
                break;
            // 更新目标行索引，向上方移动一格
            target++;
        }
    }
    // 如果有移动发生（isMove > 0），返回 true，否则返回 false
    return isMove > 0;
}

// 仅移动 grid，不进行合并或其他操作
void GameLayer::moveOnly(int row, int column, int targetRow, int targetColumn)
{
    // 如果当前位置和目标位置相同，则直接返回
    if (row == targetRow && column == targetColumn)
        return;

    // 仅在第一次移动时，保存上一次的布局
    saveLastGrids();

    // 调用 grid 的 moveOnly 方法进行移动
    _grids[row][column]->moveOnly(targetRow, targetColumn);
    // 更新目标位置的 grid 指针
    _grids[targetRow][targetColumn] = _grids[row][column];
    // 将当前位置的 grid 指针置为空
    _grids[row][column] = nullptr;
}

// 创建新的 grid 并移动到目标位置，同时更新其值
void GameLayer::moveAndUpdate(int value, int targetRow, int targetColumn)
{
    // 创建新的临时 grid
    auto temp = Grid::create();
    // 初始化临时 grid 的值、行索引和列索引
    temp->initValue(value, targetRow, targetColumn);
    // 更新目标位置的 grid 指针为新创建的临时 grid
    _grids[targetRow][targetColumn] = temp;
    // 将新的 grid 添加到当前层中
    this->addChild(_grids[targetRow][targetColumn]);
    // 调用临时 grid 的 moveAndUpdate 方法进行移动和更新显示
    temp->moveAndUpdate();
}

// 移动 grid 并在移动后清除原位置的 grid
void GameLayer::moveAndClear(int row, int column, int targetRow, int targetColumn)
{
    // 保存上一次的布局
    saveLastGrids();

    // 调用 grid 的 moveAndClear 方法进行移动和清除原位置
    _grids[row][column]->moveAndClear(targetRow, targetColumn);
    // 将原位置的 grid 指针置为空
    _grids[row][column] = nullptr;
}
