#include "GameLayer.h"
#include "GameTool.h"
#include "DataConf.h"
#include "AudioEngine.h"
using namespace cocos2d::experimental;

GameLayer* GameLayer::_instance = nullptr;
int isMove = 0; // ����0ʱ����ʾ�ƶ��˵�ǰ�����ֲ��֣������Ҫ�Զ�������һ�εĲ��ֽ��, ���ƶ���Ϻ���RandGenGrid�н����Զ���0
GameLayer* GameLayer::getInstance()
{
    if (_instance == nullptr)
        _instance = create();
    return _instance;
}

// ��ʼ��
bool GameLayer::init()
{
    do {
        CC_BREAK_IF(!Layer::init());

        this->setContentSize(Size(300, 300));
        this->setPosition(10, 65);
        // ��ʼ�� ����
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
    // ���ظ���
    loadGrids(2);
}
// ��ʼ������
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

// ���ظ��ӷ���
void GameLayer::loadGrids(int type)
{
    _isOver = false;
    auto f = UserDefault::getInstance();
    // ���XML�ļ������ڻ���ָ�����͵���Ϸ���ݲ����ڣ��ͳ�ʼ������
    if (!f->isXMLFileExist() || !f->getBoolForKey(Value(type).asString().append("exits").c_str(), false))
        return initGrids();

    int value = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // ��UserDefault�л�ȡָ��λ�õ���ֵ
            value = f->getIntegerForKey(Value(type * 3).asString().append(Value(100 + i * 4 + j).asString()).c_str());
            if (value == -1)
                _grids[i][j] = nullptr;
            else
            {
                _grids[i][j] = Grid::create();
                _grids[i][j]->initValue(value, i, j);
                this->addChild(_grids[i][j]);
            }
            // ��¼��һ���������ֵ
            _lastGrids[i][j] = f->getIntegerForKey(Value(type * 3).asString().append(Value(i * 4 + j).asString()).c_str());
        }
    }
}

//�������
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

#define EMPTY -1 // ��ʾlastGrids�б�������ϴεĲ���
#define DOUBLE_EMPTY -2 // ��ʾ_lastGrids�б�����ǳ�ʼ���Ľ��
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

// ���¿�ʼ��Ϸ
void GameLayer::restartGame()
{
    _isOver = false;
    GameTool::getInstance()->resetScore();
    clearGrids();
    clearLastGrids();
    randGenGrid();
    randGenGrid();
}

// ����
void GameLayer::undoGame()
{
    if (_lastGrids[0][0] != DOUBLE_EMPTY) // �ǳ�ʼ��ʱ�����ܵ���undo
    {
        clearGrids();
        recoverLastGrids();
    }
}

void GameLayer::saveLastGrids()
{
    // ����ƶ�����������1���򲻱�����һ������״̬
    if (++isMove != 1)
        return;

    // ������ǰ���񣬱���ÿ�����ӵ���ֵ��_lastGrids������
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
    // ������һ������״̬���飬������ֵ�ָ�ÿ�����ӵ�״̬
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            if (_lastGrids[row][column] > EMPTY)
            {
                // �����һ������״̬��Ϊ�գ��򴴽��µĸ��Ӳ���ʼ����ֵ
                _grids[row][column] = Grid::create();
                _grids[row][column]->initValue(_lastGrids[row][column], row, column);
                this->addChild(_grids[row][column]);
            }
            else
            {
                // �����һ������״̬Ϊ�գ�����Ϊnullptr
                _grids[row][column] = nullptr;
            }
        }
    }
}

void GameLayer::randGenGrid()
{
    isMove = 0; // �����ƶ�����Ϊ0�����Ϊ���¿�ʼ�ж��Ƿ��ƶ�
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
    // ���˳���Ϸ����ʱ����������
    /*log("eixt%d", DataConf::getInstance()->getModelType());*/
    DataConf::getInstance()->dumpData(Grid::getType());
}

bool GameLayer::onTouchBegan(Touch* touch, Event* event)
{
    if (_isOver) // �����Ϸ�����������¿�ʼ��Ϸ
    {
        auto fail = this->getChildByName("fail");
        fail->setVisible(false);
        restartGame();
        return false;
    }

    _begin = touch->getLocation();
    auto rect = Rect(this->getPosition().x, this->getPosition().y, this->getContentSize().width, this->getContentSize().height);
    return rect.containsPoint(_begin); // ֻ���ڵ����Ϸ�����ڲ���Ӧ�����¼�
}
void GameLayer::onTouchMoved(Touch* touch, Event* event)
{
}
void GameLayer::onTouchEnded(Touch* touch, Event* event)
{
    _end = touch->getLocation();
    auto value = _end - _begin;

    // �жϺ����ƶ����������ƶ�
    if (abs(value.x) > abs(value.y)) // �����ƶ�
    {
        if (value.x - 5 > 0) // �����ƶ�
            moveToRight();
        else if (value.x + 5 < 0) // �����ƶ�
            moveToLeft();
        else
            return;
    }
    else // �����ƶ�
    {
        if (value.y - 5 > 0) // �����ƶ�
            moveToTop();
        else if (value.y + 5 < 0) // �����ƶ�
            moveToBottom();
        else
            return;
    }

    if (isMove > 0) // ������ƶ����������µĸ���
        randGenGrid();

    // �ж���Ϸ�Ƿ����
    _isOver = ifOver();
}

bool GameLayer::ifOver()
{
    // �����ڿո�������������ڵ� grid ������ͬʱ����ʾ��Ϸû�н���
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            // �����ǰ grid Ϊ�գ�����Ϸδ����
            if (_grids[row][column] == nullptr)
                return false;

            // �����ǰ grid ���Ҳ���²�� grid ������ͬ������Ϸδ����
            if (_grids[row][column]->compareTo(_grids[row][column + 1]) || _grids[row][column]->compareTo(_grids[row + 1][column]))
                return false;
        }
    }

    for (int column = 0; column < 3; column++)
    {
        // ���������һ�еĵ�ǰ grid Ϊ�ջ������Ҳ� grid ������ͬ������Ϸδ����
        if (_grids[3][column] == nullptr || _grids[3][column]->compareTo(_grids[3][column + 1]))
            return false;
    }

    for (int row = 0; row < 3; row++)
    {
        // ������Ҳ�һ�еĵ�ǰ grid Ϊ�ջ������²� grid ������ͬ������Ϸδ����
        if (_grids[row][3] == nullptr || _grids[row][3]->compareTo(_grids[row + 1][3]))
            return false;
    }

    // game over and show the fail scene
    auto fail = this->getChildByName("fail");
    if (fail == nullptr)
    {
        // ����һ����͸���Ļ�ɫͼ�㣬����ӡ�Game Over!����ǩ
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
            // �����������ķǿ� grid ��������
            while (--column >= 0)
                if (_grids[row][column] != nullptr)
                    return column;
            break;
        case Direction::RIGHT:
            // ���Ҳ�������ķǿ� grid ��������
            while (++column <= 3)
                if (_grids[row][column] != nullptr)
                    return column;
            break;
        case Direction::UP:
            // ���ϲ�������ķǿ� grid ��������
            while (++row <= 3)
                if (_grids[row][column] != nullptr)
                    return row;
            break;
        case Direction::DOWN:
            // ���²�������ķǿ� grid ��������
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
        // ֱ���ƶ���Ŀ��λ��
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

    // �ж��Ƿ���Ժϲ���������
    if (_grids[row][column]->compareTo(_grids[preRow][preCol]))
    {
        auto value = _grids[row][column]->getScoreValue();
        // �ƶ������ԭλ��
        moveAndClear(row, column, targetRow, targetColumn);
        moveAndClear(preRow, preCol, targetRow, targetColumn);
        // �ƶ������ºϲ��������
        moveAndUpdate(++value, targetRow, targetColumn);
        AudioEngine::play2d("sound_click.mp3", false); // �ڶ������� true ��ʾѭ������
        GameTool::getInstance()->updateScore(pow(2, ++value));
        return preColumnOrRow;
    }

    // ֱ���ƶ���Ŀ��λ��
    moveOnly(row, column, targetRow, targetColumn);
    return preColumnOrRow + tag;
}bool GameLayer::moveToRight()
{
    // Ŀ����������ʼΪ 3�����Ҳࣩ
    auto target = 3;
    for (int row = 0; row < 4; row++)
    {
        // ÿ�ν����µ�һ�У����½�Ŀ����������Ϊ 3
        target = 3;
        for (int column = 3; column >= 0; column--)
        {
            // �����ǰ grid Ϊ�գ�����
            if (_grids[row][column] == nullptr)
                continue;
            // ���������ƶ���ǰ grid���ƶ���õ��µ�������
            column = moveGrid(Direction::LEFT, row, column, row, target);
            // ����ƶ�ʧ�ܣ����� ERRORINDEX�����˳���ǰ�е�ѭ��
            if (column == ERRORINDEX)
                break;
            // ����Ŀ����������������ƶ�һ��
            target--;
        }
    }
    // ������ƶ�������isMove > 0�������� true�����򷵻� false
    return isMove > 0;
}

bool GameLayer::moveToLeft()
{
    // Ŀ����������ʼΪ 0������ࣩ
    auto target = 0;
    for (int row = 0; row < 4; row++)
    {
        // ÿ�ν����µ�һ�У����½�Ŀ����������Ϊ 0
        target = 0;
        for (int column = 0; column < 4; column++)
        {
            // �����ǰ grid Ϊ�գ�����
            if (_grids[row][column] == nullptr)
                continue;
            // ���������ƶ���ǰ grid���ƶ���õ��µ�������
            column = moveGrid(Direction::RIGHT, row, column, row, target);
            // ����ƶ�ʧ�ܣ����� ERRORINDEX�����˳���ǰ�е�ѭ��
            if (column == ERRORINDEX)
                break;
            // ����Ŀ�������������Ҳ��ƶ�һ��
            target++;
        }
    }
    // ������ƶ�������isMove > 0�������� true�����򷵻� false
    return isMove > 0;
}

bool GameLayer::moveToTop()
{
    // Ŀ����������ʼΪ 3�����Ϸ���
    int target = 3;
    for (int column = 0; column < 4; column++)
    {
        // ÿ�ν����µ�һ�У����½�Ŀ����������Ϊ 3
        target = 3;
        for (int row = 3; row >= 0; row--)
        {
            // �����ǰ grid Ϊ�գ�����
            if (_grids[row][column] == nullptr)
                continue;
            // ���������ƶ���ǰ grid���ƶ���õ��µ�������
            row = moveGrid(Direction::DOWN, row, column, target, column);
            // ����ƶ�ʧ�ܣ����� ERRORINDEX�����˳���ǰ�е�ѭ��
            if (row == ERRORINDEX)
                break;
            // ����Ŀ�������������·��ƶ�һ��
            target--;
        }
    }
    // ������ƶ�������isMove > 0�������� true�����򷵻� false
    return isMove > 0;
}

bool GameLayer::moveToBottom()
{
    // Ŀ����������ʼΪ 0�����·���
    int target = 0;
    for (int column = 0; column < 4; column++)
    {
        // ÿ�ν����µ�һ�У����½�Ŀ����������Ϊ 0
        target = 0;
        for (int row = 0; row < 4; row++)
        {
            // �����ǰ grid Ϊ�գ�����
            if (_grids[row][column] == nullptr)
                continue;
            // ���������ƶ���ǰ grid���ƶ���õ��µ�������
            row = moveGrid(Direction::UP, row, column, target, column);
            // ����ƶ�ʧ�ܣ����� ERRORINDEX�����˳���ǰ�е�ѭ��
            if (row == ERRORINDEX)
                break;
            // ����Ŀ�������������Ϸ��ƶ�һ��
            target++;
        }
    }
    // ������ƶ�������isMove > 0�������� true�����򷵻� false
    return isMove > 0;
}

// ���ƶ� grid�������кϲ�����������
void GameLayer::moveOnly(int row, int column, int targetRow, int targetColumn)
{
    // �����ǰλ�ú�Ŀ��λ����ͬ����ֱ�ӷ���
    if (row == targetRow && column == targetColumn)
        return;

    // ���ڵ�һ���ƶ�ʱ��������һ�εĲ���
    saveLastGrids();

    // �����ƶ��������ƶ���Ŀ��λ�ã�����ע�͵��Ĵ���ʹ���� MoveTo �����࣬���Ը���ʵ�����ѡ���Ƿ�ʹ�ã�
    //auto action = MoveTo::create(0.1f, Vec2(73*targetColumn + 8, 73*targetRow + 8));
    // ���� grid �� moveOnly ���������ƶ�
    _grids[row][column]->moveOnly(targetRow, targetColumn);
    // ����Ŀ��λ�õ� grid ָ��
    _grids[targetRow][targetColumn] = _grids[row][column];
    // ����ǰλ�õ� grid ָ����Ϊ��
    _grids[row][column] = nullptr;
}

// �����µ� grid ���ƶ���Ŀ��λ�ã�ͬʱ������ֵ
void GameLayer::moveAndUpdate(int value, int targetRow, int targetColumn)
{
    // �����µ���ʱ grid
    auto temp = Grid::create();
    // ��ʼ����ʱ grid ��ֵ����������������
    temp->initValue(value, targetRow, targetColumn);
    // ����Ŀ��λ�õ� grid ָ��Ϊ�´�������ʱ grid
    _grids[targetRow][targetColumn] = temp;
    // ���µ� grid ��ӵ���ǰ����
    this->addChild(_grids[targetRow][targetColumn]);
    // ������ʱ grid �� moveAndUpdate ���������ƶ��͸�����ʾ
    temp->moveAndUpdate();
}

// �ƶ� grid �����ƶ������ԭλ�õ� grid
void GameLayer::moveAndClear(int row, int column, int targetRow, int targetColumn)
{
    // ������һ�εĲ���
    saveLastGrids();

    // ���� grid �� moveAndClear ���������ƶ������ԭλ��
    _grids[row][column]->moveAndClear(targetRow, targetColumn);
    // ��ԭλ�õ� grid ָ����Ϊ��
    _grids[row][column] = nullptr;
}