#ifndef __START_LAYER_H__
#define __START_LAYER_H__

#include "cocos2d.h"

class StartLayer : public cocos2d::Layer
{
public:
    // 初始化方法
    virtual bool init();

    // 按钮点击回调
    void onNewGameClicked(cocos2d::Ref* sender);
    void onContinueGameClicked(cocos2d::Ref* sender);

    // 静态创建对象的方法
    CREATE_FUNC(StartLayer);
};

#endif // __START_LAYER_H__
