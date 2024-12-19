#include "GameTool.h"
#include "DataConf.h"
USING_NS_CC;
// 静态变量，存储 GameTool 单例
GameTool* GameTool::_instance = nullptr;

// 获取 GameTool 单例对象，如果不存在则创建
GameTool* GameTool::getInstance()
{
	if (_instance == nullptr) // 如果单例为空
		_instance = create(); // 创建新的 GameTool 对象
	return _instance; // 返回单例
}

// 初始化方法
bool GameTool::init()
{
	do {
		// 初始化父类 Layer，如果失败则退出
		CC_BREAK_IF(!Layer::init());

		// 设置工具层的大小和位置
		this->setContentSize(Size(300, 60)); // 设置大小为 300x60
		this->setPosition(10, 410); // 设置位置为 (10, 410)

		// 创建一个图标层，颜色为透明黄，大小为 100x60
		auto layerIcon = LayerColor::create(Color4B(230, 230, 0, 0), 100, 60);
		
		// 创建一个显示 "2048" 的标签
		auto label = Label::createWithSystemFont("2048", "Arial", 36);
		label->setTextColor(Color4B(134, 134, 134, 255)); // 设置文字颜色为灰色
		label->setPosition(60, 30); // 设置标签位置
		layerIcon->addChild(label); // 将标签添加到图标层中
		this->addChild(layerIcon); // 将图标层添加到当前层中

		// 初始化分数显示
		initScore();
	} while (0); // 使用 do-while(0) 结构确保代码出错时能够退出
	return true; // 初始化成功返回 true
}

// 初始化分数相关 UI
bool GameTool::initScore()
{
	// 创建当前分数图标层，背景色为灰褐色，大小为 80x50
	auto scoreIcon = LayerColor::create(Color4B(186, 172, 159, 255), 80, 50);
	auto scoreTitleLabel = Label::createWithSystemFont("分数", "Arial", 16); // 创建分数标题标签
	scoreTitleLabel->setPosition(40, 35); // 设置分数标题位置
	scoreIcon->setPosition(Vec2(130, 5)); // 设置分数图标位置
	scoreIcon->addChild(scoreTitleLabel); // 将分数标题添加到图标层中

	// 创建当前分数字体标签
	scoreLabel = Label::createWithSystemFont(Value(_score).asString(), "Arial", 20);
	scoreLabel->setPosition(40, 10); // 设置分数标签位置
	scoreIcon->addChild(scoreLabel); // 将分数标签添加到图标层中
	this->addChild(scoreIcon); // 将图标层添加到当前层中

	// 创建最佳分数图标层，背景色为灰褐色，大小为 80x50
	auto bestIcon = LayerColor::create(Color4B(186, 172, 159, 255), 80, 50);
	auto bestTitleLabel = Label::createWithSystemFont("最高分", "Arial", 16); // 创建最佳分数标题标签
	bestTitleLabel->setPosition(40, 35); // 设置最佳分数标题位置
	bestIcon->setPosition(Vec2(220, 5)); // 设置最佳分数图标位置
	bestIcon->addChild(bestTitleLabel); // 将最佳分数标题添加到图标层中

	// 创建最佳分数字体标签
	bestLabel = Label::createWithSystemFont(Value(_bestScore).asString(), "Arial", 20);
	bestLabel->setPosition(40, 10); // 设置最佳分数标签位置
	bestIcon->addChild(bestLabel); // 将最佳分数标签添加到图标层中
	this->addChild(bestIcon); // 将最佳分数图标层添加到当前层中

	// 初次从本地文件加载分数
	loadScore(UserDefault::getInstance()->getIntegerForKey("type", 1));
	return true; // 初始化成功返回 true
}

// 从本地存储加载分数
void GameTool::loadScore(int type)
{
	// 加载当前分数
	setScore(UserDefault::getInstance()->getIntegerForKey(Value(type).asString().append("score").c_str(), 0));
	// 加载最佳分数
	setBestScore(UserDefault::getInstance()->getIntegerForKey(Value(type).asString().append("best_score").c_str(), 0));
	this->loadScoresFromLocal();
}

// 获取当前分数
int GameTool::getScore()
{
	return _score;
}

// 设置当前分数
void GameTool::setScore(int score)
{
	
	_score = score; // 更新分数
	scoreLabel->setString(Value(_score).asString()); // 更新分数标签的显示内容

}

// 获取最佳分数
int GameTool::getBestScore()
{
	return _bestScore;
}

void GameTool::loadScoresFromLocal()
{
	_scores.clear(); // 确保分数列表初始化为空

	// 从 UserDefault 中获取存储的分数字符串
	std::string serializedScores = UserDefault::getInstance()->getStringForKey("scores", "");

	if (!serializedScores.empty())
	{
		// 按逗号分割字符串
		std::stringstream ss(serializedScores);
		std::string score;
		while (std::getline(ss, score, ',')) // 使用逗号分隔符解析
		{
			// 转换为整数并添加到分数列表
			_scores.push_back(std::stoi(score));
		}
	}

}


void GameTool::saveScoresToLocal()
{
	std::string serializedScores;
	for (const int score : _scores)
	{
		serializedScores += Value(score).asString() + ","; // 使用逗号分隔
	}
	// 去掉最后一个逗号
	if (!serializedScores.empty() && serializedScores.back() == ',')
		serializedScores.pop_back();

	UserDefault::getInstance()->setStringForKey("scores", serializedScores);
}

const std::vector<int>& GameTool::getScores() const
{
	return _scores;
}

// 设置最佳分数
void GameTool::setBestScore(int bestScore)
{
	_bestScore = bestScore; // 更新最佳分数
	bestLabel->setString(Value(_bestScore).asString()); // 更新最佳分数标签的显示内容

}

// 更新分数并检测是否需要更新最佳分数
void GameTool::updateScore(int addScore)
{
	setScore(_score + addScore); // 增加当前分数
	updateBestScore(); // 更新最佳分数（如果需要）
}

// 重置当前分数
void GameTool::resetScore()
{
	_scores.push_back(_score); // 将新的分数追加到数组中
	// 保存到本地存储
	saveScoresToLocal();
	setScore(0); // 将当前分数设置为 0
}

// 更新最佳分数（如果当前分数更高）
void GameTool::updateBestScore()
{
	if (_score < _bestScore) // 如果当前分数小于最佳分数，则不更新
		return;

	setBestScore(_score); // 否则更新最佳分数
}
