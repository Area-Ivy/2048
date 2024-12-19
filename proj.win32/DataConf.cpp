#include "DataConf.h"
#include "GameTool.h"
#include "GameLayer.h"

static DataConf* instance;
DataConf* DataConf::getInstance()
{
	if (instance == nullptr) // ���ʵ��Ϊ��
		instance = create(); // ����һ��ʵ��

	return instance; // ����ʵ��
}

bool DataConf::init()
{
	return true; // ��ʼ��������Ŀǰ���Ƿ��� true
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
	auto f = UserDefault::getInstance(); // ��ȡ UserDefault ʵ��
	f->setIntegerForKey(Value(type).asString().append("score").c_str(), GameTool::getInstance()->getScore()); // �洢��ǰ����
	f->setIntegerForKey(Value(type).asString().append("best_score").c_str(), GameTool::getInstance()->getBestScore()); // �洢��߷���
	f->setIntegerForKey("type", type); // �洢��Ϸ����
	f->setBoolForKey(Value(type).asString().append("exits").c_str(), true); // �洢��Ϸ״̬Ϊ����
	GameTool::getInstance()->saveScoresToLocal(); // �������������
	f->flush(); // �������浽����

	Grid* temp;
	int value;
	for (int i = 0; i < 4; i++) // ������
	{
		for (int j = 0; j < 4; j++) // ������
		{
			temp = GameLayer::getInstance()->_grids[i][j]; // ��ȡ��ǰ����
			if (temp == nullptr)
				value = -1;
			else
				value = temp->getScoreValue(); // ��ȡ���ӵķ���ֵ
			f->setIntegerForKey(Value(type * 3).asString().append(Value(100 + i * 4 + j).asString()).c_str(), value); // �洢��ǰ���ӷ���ֵ
			f->setIntegerForKey(Value(type * 3).asString().append(Value(i * 4 + j).asString()).c_str(), GameLayer::getInstance()->_lastGrids[i][j]); // �洢��һ������״̬
		}
	}
	f->flush(); // �������浽����
}