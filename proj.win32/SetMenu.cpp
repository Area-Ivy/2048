#include "SetMenu.h"
#include "Grid.h"
#include "GameTool.h"
#include "GameLayer.h"
#include "DataConf.h"
USING_NS_CC;
bool SetMenu::init()
{
    if (!Layer::init())
        return false;

    this->setContentSize(Size(300, 400));
    this->setPosition(Vec2(0, 0));

    this->refresh(); // ����ˢ�·���

    return true;
}

void SetMenu::backToMenu(cocos2d::Ref* sender)
{
    // ʵ�ַ����߼�������رյ�ǰ�˵�
    this->setVisible(false); // ���ز˵�

    // �����Ҫֱ��ˢ�¿ɼ�����
    this->refresh();
}
void SetMenu::classicCallFunc(Ref* ref)
{
	changeType(1);
}

void SetMenu::soldierCallFunc(Ref* ref)
{
	changeType(0);
}

void SetMenu::colorCallFunc(Ref* ref)
{
	changeType(2);
}

void SetMenu::soundCallFunc(Ref* ref)
{

}

void SetMenu::refresh()
{
    // �����ǰ�ڵ��е����а�
    this->removeAllChildren();

    // �ؽ�����
    auto bg = LayerColor::create(Color4B(30, 30, 30, 200), 300, 400);
    this->addChild(bg);

    auto draw = DrawNode::create();
    this->addChild(draw);

    // Title
    auto title = Label::createWithSystemFont(Grid::G2U("���а�"), "Arial", 30);
    title->setPosition(150, 370);
    this->addChild(title);

    // Divider
    draw->drawLine(Vec2(10, 350), Vec2(290, 350), Color4F(1, 1, 1, 1));

    // ��ȡ��ˢ�·�������
    auto scores = GameTool::getInstance()->getScores();
    std::sort(scores.rbegin(), scores.rend());

    // ��ӡ��־
    std::string logStr = "Scores: ";
    int yStart = 320;
    int offset = 40;

    for (size_t i = 0; i < scores.size(); ++i)
    {
        logStr += Value(scores[i]).asString();
        auto rankLabel = Label::createWithSystemFont(Value(scores[i]).asString(), "Arial", 16);
        rankLabel->setAnchorPoint(Vec2(0, 0.5f));
        rankLabel->setPosition(Vec2(50, yStart - i * offset));
        this->addChild(rankLabel);
        if (i != scores.size() - 1)
            logStr += ", ";
    }
    CCLOG("%s", logStr.c_str());

    // Back button
    auto backLabel = Label::createWithSystemFont(Grid::G2U("����"), "Arial", 16);
    auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(SetMenu::backToMenu, this));
    backItem->setPosition(Vec2(150, 30));

    auto menu = Menu::create(backItem, nullptr);
    menu->setPosition(0, 0);
    this->addChild(menu);
}

void SetMenu::changeType(int newType)
{
	auto type = Grid::getType();
	if (type == newType) // do not change the game model
		return;
	// do change, then save the game stat and restart new game model
	DataConf::getInstance()->dumpData(type);
	GameLayer::getInstance()->clearGrids();
	Grid::changeType(newType);
	this->setVisible(false);
}

