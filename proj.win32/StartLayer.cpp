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

    // ��ȡ��Ļ�ߴ�
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ������ɫ����
    auto background = LayerColor::create(Color4B(0, 0, 0, 180));  // ��ɫ������͸����180
    background->setPosition(Vec2::ZERO);  // ��������λ������Ϊԭ��
    this->addChild(background);

    // ����������Ϸ����ť
    auto newGameItem = MenuItemLabel::create(Label::createWithTTF("New Game", "fonts/arial.ttf", 24),
        CC_CALLBACK_1(StartLayer::onNewGameClicked, this));
    newGameItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 50));

    // ������������Ϸ����ť
    auto continueGameItem = MenuItemLabel::create(Label::createWithTTF("Continue Game", "fonts/arial.ttf", 24),
        CC_CALLBACK_1(StartLayer::onContinueGameClicked, this));
    continueGameItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 50));

    // ����ť����˵���
    auto menu = Menu::create(newGameItem, continueGameItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}

// ���������Ϸ����ť��Ĵ�����
void StartLayer::onNewGameClicked(Ref* sender) {
    GameLayer::getInstance()->restartGame();
    // ���ص�ǰ UI ��
    this->setVisible(false);
    log("reset game");
}

// �����������Ϸ����ť��Ĵ�����
void StartLayer::onContinueGameClicked(Ref* sender)
{
    // ������Ϸ���߼���������ر������Ϸ����

    // ���ص�ǰ UI ��
    this->setVisible(false);

    // ���������Ӽ�����Ϸ���߼�
    // ����ֱ�ӽ�����Ϸ����
    //Director::getInstance()->replaceScene(GameScene::createScene());
}
