#pragma once

#include "cocos2d.h"

USING_NS_CC;

class SetMenu :public Layer
{
public:
	static SetMenu* getInstance();
	CREATE_FUNC(SetMenu);
	// 添加回调方法声明
	void backToMenu(cocos2d::Ref* sender);
	virtual bool init();
	void refresh();
private:
	void classicCallFunc(Ref* ref);
	void colorCallFunc(Ref* ref);
	void soldierCallFunc(Ref* ref);
	void soundCallFunc(Ref* ref);
	void changeType(int newType);
};