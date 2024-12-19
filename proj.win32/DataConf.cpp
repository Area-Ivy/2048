#include "DataConf.h"
#include "GameTool.h"
#include "GameLayer.h"

static DataConf* instance;
DataConf* DataConf::getInstance()
{
	if (instance == nullptr) // 如果实例为空
		instance = create(); // 创建一个实例

	return instance; // 返回实例
}

bool DataConf::init()
{
	return true; // 初始化函数，目前总是返回 true
}

//void DataConf::loadMap()
//{
//	extern std::string map[16];
//	auto all = FileUtils::getInstance()->getValueMapFromFile("data.plist");
//	auto type = all.at("type").asInt();
//	log("the type is %d\n", type);
//
//	for(int i = 0; i < 16; i++)
//	{
//		if(type == 0)
//			map[i] = "";
//		else
//			map[i] = all.at(Value(type+i).asString()).asString();
//	}
//}

void DataConf::dumpData(int type)
{
	auto f = UserDefault::getInstance(); // 获取 UserDefault 实例
	f->setIntegerForKey(Value(type).asString().append("score").c_str(), GameTool::getInstance()->getScore()); // 存储当前分数
	f->setIntegerForKey(Value(type).asString().append("best_score").c_str(), GameTool::getInstance()->getBestScore()); // 存储最高分数
	f->setIntegerForKey("type", type); // 存储游戏类型
	f->setBoolForKey(Value(type).asString().append("exits").c_str(), true); // 存储游戏状态为存在
	GameTool::getInstance()->saveScoresToLocal(); // 保存分数到本地
	f->flush(); // 立即保存到本地

	Grid* temp;
	int value;
	for (int i = 0; i < 4; i++) // 遍历行
	{
		for (int j = 0; j < 4; j++) // 遍历列
		{
			temp = GameLayer::getInstance()->_grids[i][j]; // 获取当前格子
			if (temp == nullptr)
				value = -1;
			else
				value = temp->getScoreValue(); // 获取格子的分数值
			f->setIntegerForKey(Value(type * 3).asString().append(Value(100 + i * 4 + j).asString()).c_str(), value); // 存储当前格子分数值
			f->setIntegerForKey(Value(type * 3).asString().append(Value(i * 4 + j).asString()).c_str(), GameLayer::getInstance()->_lastGrids[i][j]); // 存储上一个格子状态
		}
	}
	f->flush(); // 立即保存到本地
}