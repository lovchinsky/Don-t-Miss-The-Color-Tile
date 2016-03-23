#ifndef GAME_OVER_SCENE_H
#define GAME_OVER_SCENE_H

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ModeScene.h"

class GameOverScene: public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(Mode mode, unsigned int score);

	void setMode(Mode mode);
	void setScore(unsigned int score);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameOverScene);
    
private:
	unsigned int highScore;
	unsigned int currentScore;

	Mode currentMode;

	cocos2d::Label* titleLabel;
	cocos2d::Sprite* titleSprite;
	cocos2d::Sprite* scoreSprite;
	cocos2d::Sprite* highScoreSprite;
	cocos2d::Sprite* replaySprite;
	cocos2d::Sprite* mainMenuSprite;
	cocos2d::Label* scoreLabel;
	cocos2d::Label* highScoreLabel;
	cocos2d::FiniteTimeAction* showAction;
	cocos2d::FiniteTimeAction* hideAction;

	void replayGame(Mode mode);

	void replayOnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void mainMenuOnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
};

#endif
