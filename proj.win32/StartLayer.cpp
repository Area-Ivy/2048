#include "StartLayer.h"
#include "GameLayer.h"
#include "cocos2d.h"
#include "GameScene.h" 

USING_NS_CC;

bool StartLayer::init()
{
    if (!Layer::init()) {
        return false;
    }

    // 获取屏幕尺寸
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建黑色背景
    auto background = LayerColor::create(Color4B(0, 0, 0, 180));  // 黑色背景，透明度180
    background->setPosition(Vec2::ZERO);  // 将背景层位置设置为原点
    this->addChild(background);

    // 创建“新游戏”按钮
    auto newGameItem = MenuItemLabel::create(Label::createWithTTF("New Game", "fonts/arial.ttf", 24),
        CC_CALLBACK_1(StartLayer::onNewGameClicked, this));
    newGameItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 50));

    // 创建“继续游戏”按钮
    auto continueGameItem = MenuItemLabel::create(Label::createWithTTF("Continue Game", "fonts/arial.ttf", 24),
        CC_CALLBACK_1(StartLayer::onContinueGameClicked, this));
    continueGameItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 50));

    // 将按钮放入菜单中
    auto menu = Menu::create(newGameItem, continueGameItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}

// 点击“新游戏”按钮后的处理函数
void StartLayer::onNewGameClicked(Ref* sender) {
    GameLayer::getInstance()->restartGame();
    // 隐藏当前 UI 层
    this->setVisible(false);
    log("reset game");
}

// 点击“继续游戏”按钮后的处理函数
void StartLayer::onContinueGameClicked(Ref* sender)
{
    // 继续游戏的逻辑，例如加载保存的游戏数据

    // 隐藏当前 UI 层
    this->setVisible(false);
    
}
