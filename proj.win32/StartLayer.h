#ifndef __START_LAYER_H__
#define __START_LAYER_H__

#include "cocos2d.h"

class StartLayer : public cocos2d::Layer
{
public:
    // ��ʼ������
    virtual bool init();

    // ��ť����ص�
    void onNewGameClicked(cocos2d::Ref* sender);
    void onContinueGameClicked(cocos2d::Ref* sender);

    // ��̬��������ķ���
    CREATE_FUNC(StartLayer);
};

#endif // __START_LAYER_H__
