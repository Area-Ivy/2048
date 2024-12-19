#include "Grid.h"
#include "GameTool.h"
#include "GameLayer.h"

std::string map[16] = { "0" };
std::string num[19] = { "2", "4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048", "4096", "8192", "16384", "32768", "65536",  "131072", "262144", "524288" };

static int type = 1;
// 改变游戏类型，更新相关状态并重新加载分数和网格布局
void Grid::changeType(int _type)
{
    type = _type;
    GameTool::getInstance()->loadScore(type);
    GameLayer::getInstance()->loadGrids(type);
}

// 获取当前游戏类型
int Grid::getType()
{
    return type;
}

// 将 GB2312 编码字符串转换为 UTF-8 编码字符串
char* Grid::G2U(const char* gb2312)
{
    int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr) delete[] wstr;
    return str;
}

// 初始化网格对象
bool Grid::init()
{
    if (!Layer::init())
        return false;

    // 加载地图数据
    loadMap();

    // 设置网格的大小
    this->setContentSize(Size(65, 65));
    // 初始值设为 0
    _value = 0;
    // 创建背景颜色层
    _bg = LayerColor::create(Color4B(213, 205, 194, 255), 65, 65);
    //_bg->setAnchorPoint(Vec2(32.5f, 32.5f));
    // 创建标签用于显示数字
    _label = Label::createWithSystemFont(map[_value], "Arial", 30);
    //_label = Label::createWithTTF(map[_value], "MarkerFelt.ttf", 30);
    // 设置标签的位置在网格中心
    _label->setPosition(32.5f, 32.5f);
    // 将背景和标签添加到网格层
    this->addChild(_bg);
    this->addChild(_label);
    // 更新背景颜色和标签显示
    updateBg();

    return true;
}

// 加载地图数据，根据游戏类型设置不同的显示内容
void Grid::loadMap()
{
    if (map[0].compare("0") == 0) // 首次加载时，根据游戏类型加载显示对象
    {
        type = UserDefault::getInstance()->getIntegerForKey("type", 0);
    }
    type = 2;
    for (int i = 0; i < 16; i++)
    {
        if (type == StateType::CLASSIC)
            map[i] = num[i];
        else if (type == StateType::COLOR)
            map[i] = "";
    }
}

// 判断当前网格与另一个网格的值是否相同
bool Grid::compareTo(Grid* grid)
{
    return grid != nullptr && _value == grid->getScoreValue();
}

// 初始化网格的值，用于创建新的网格
void Grid::initValue(int value)
{
    _value = value;
    updateBg();
}

// 初始化网格的值，并设置网格的行和列位置
void Grid::initValue(int value, int row, int column)
{
    setLocalPosition(row, column);
    initValue(value);
}

// 设置网格在游戏布局中的位置
void Grid::setLocalPosition(int row, int column)
{
    this->setPosition(column * 73 + 8, row * 73 + 8);
}

// 初始化网格的显示动画
void Grid::initAction()
{
    auto a1 = ScaleTo::create(0.3f, 1);
    auto a2 = FadeIn::create(1);
    auto a3 = Spawn::create(a1, a2, nullptr);
    setScale(0);
    runAction(a3);
}

// 仅移动网格到目标位置
void Grid::moveOnly(int targetRow, int targetColumn)
{
    runAction(MoveTo::create(0.1f, Vec2(73 * targetColumn + 8, 73 * targetRow + 8)));
}

// 移动网格到目标位置并清除当前网格
void Grid::moveAndClear(int targetRow, int targetColumn)
{
    auto a1 = MoveTo::create(0.1f, Vec2(73 * targetColumn + 8, 73 * targetRow + 8));
    auto a2 = FadeOut::create(0.1f);
    auto a3 = Spawn::create(a1, a2, nullptr);
    auto a4 = CallFunc::create([&] {
        //log("remove child");
        this->removeFromParent();
        });
    this->runAction(Sequence::create(a3, a4, nullptr));
}

// 移动网格并进行更新显示
void Grid::moveAndUpdate()
{
    this->setVisible(false);
    auto action01 = ScaleTo::create(0.1f, 1.1f);
    auto action02 = ScaleTo::create(0.1f, 1);
    auto action03 = FadeIn::create(0.1f);
    auto action04 = Sequence::create(action03, CallFunc::create([&] {
        //log("add child");
        this->setVisible(true);
        }), action01, action02, nullptr);
    this->runAction(action04);
}

// 更新网格的背景颜色和标签显示
void Grid::updateBg()
{
    _label->setString(map[_value]);
    Color3B color, fcolor = Color3B(255, 255, 255);

    switch (_value)
    {
        case 0:
            color = Color3B(247, 213, 97);
            fcolor = Color3B(120, 120, 120);
            break;
        case 1:
            color = Color3B(166, 232, 103);
            fcolor = Color3B(90, 90, 90);
            break;
        case 2:
            color = Color3B(87, 212, 154);
            break;
        case 3:
            color = Color3B(19, 181, 177);
            break;
        case 4:
            color = Color3B(68, 138, 202);
            break;
        case 5:
            color = Color3B(200, 97, 234);
            break;
        case 6:
            color = Color3B(225, 115, 181);
            break;
        case 7:
            color = Color3B(238, 100, 141);
            break;
        case 8:
            color = Color3B(243, 157, 79);
            break;
        case 9:
            color = Color3B(245, 124, 78);
            break;
        case 10:
            color = Color3B(246, 76, 20);
            break;
        case 11:
            color = Color3B(210, 210, 10); //Color3B(105, 84, 187);
            break;
        case 12:
            color = Color3B(190, 194, 22); // Color3B(50, 86, 204);
            break;
        case 13:
            color = Color3B(160, 160, 10);
            break;
        case 14:
            color = Color3B(50, 86, 204);// Color3B(24, 66, 149);
            break;
        default:
            color = Color3B(24, 66, 149);
            break;
    }
    _label->setColor(fcolor);
    _bg->setColor(color);

    if (type != 1)
        return;

    if (_value > 15)
        _label->setSystemFontSize(20);
    else if (_value > 12)
        _label->setSystemFontSize(24);
    else if (_value > 8)
        _label->setSystemFontSize(26);
}
