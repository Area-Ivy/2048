#include "GameScene.h"
#include "GameLayer.h"
#include "GameMenuLayer.h"
#include "GameTool.h"
#include "SetMenu.h"
#include "AudioEngine.h"
#include "StartLayer.h"

using namespace cocos2d::experimental;
USING_NS_CC;

// 创建并返回 GameScene 场景对象
Scene* GameScene::createScene()
{
	auto scene = Scene::create(); // 创建一个新的场景对象
	auto layer = GameScene::getInstance(); // 获取 GameScene 单例
	scene->addChild(layer); // 将 GameScene 单例作为子节点添加到场景中
	return scene; // 返回场景对象
}

// 静态变量，存储 GameScene 单例
static GameScene* _gameScene;

// 获取 GameScene 单例对象，如果不存在则创建
GameScene* GameScene::getInstance()
{
	if (!_gameScene) // 如果单例对象为空
		_gameScene = GameScene::create(); // 创建新的 GameScene 对象
	return _gameScene; // 返回单例对象
}

// 初始化方法
bool GameScene::init()
{
	if (!Layer::init()) // 检查父类 Layer 初始化是否成功
		return false; // 如果失败，返回 false
	
	// 播放背景音乐，参数 true 表示循环播放
	AudioEngine::play2d("game_bg.mp3", true);
	// 在游戏场景中添加 StartLayer UI 层

	// 添加背景颜色层，设置为浅灰色 (242, 242, 242) 不透明
	this->addChild(LayerColor::create(Color4B(242, 242, 242, 255))); 

	// 获取并添加工具层(GameTool)
	auto tool = GameTool::getInstance();
	this->addChild(tool);

	// 获取并添加菜单按钮层(GameMenuLayer)
	auto menuBtnLayer = GameMenuLayer::getInstance();
	this->addChild(menuBtnLayer);

	// 获取并添加游戏层(GameLayer)
	auto gameLayer = GameLayer::getInstance();
	this->addChild(gameLayer);

	// 创建设置菜单层(SetMenu)，默认不可见
	auto setLayer = SetMenu::create();
	setLayer->setName("setlayer"); // 设置层的名称为 "setlayer"
	setLayer->setVisible(false); // 设置为不可见
	this->addChild(setLayer);
	auto startLayer = StartLayer::create();
	this->addChild(startLayer);

	return true; // 初始化成功，返回 true
}
