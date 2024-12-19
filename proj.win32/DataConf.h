#pragma once
#include "cocos2d.h"

USING_NS_CC;

class DataConf :
	public Ref
{
public:
	static std::string* map;

	CREATE_FUNC(DataConf);
	static DataConf* getInstance();
	virtual bool init();

	void dumpData(int type);
};
