#pragma once
#include "cocos2d.h"

USING_NS_CC;

class GameTool :
	public cocos2d::Layer
{
public:
	static GameTool* getInstance();

	virtual bool init();
	virtual bool initScore();
	void loadScore(int type);
	void updateScore(int addScore);
	void resetScore();

	void setScore(int score);
	void saveScoresToLocal();
	int getScore();
	void setBestScore(int bestScore);
	int getBestScore();
	const std::vector<int>& getScores() const; // 获取当前分数数组
	std::vector<int> _scores;
	void loadScoresFromLocal();
private:
	CREATE_FUNC(GameTool);
	void updateBestScore();
	
	Label* scoreLabel;
	Label* bestLabel;

	int _score;
	int _bestScore;
	static GameTool* _instance;
};