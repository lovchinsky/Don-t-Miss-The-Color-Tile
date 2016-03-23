#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <string>

#include "cocos2d.h"
#include "GameOverScene.h"
#include "MainMenuScene.h"
#include "ModeScene.h"
#include "Border.h"
#include "Square.h"

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(Mode mode);

	void setMode(Mode mode);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
private:
	void createBorder();
	void createScore();
	void createListeners();

	void createStartAction();
	void createSquare();

	void createTimer();

	void createGameOverAction();
	void createGameOverScene();

	void updateSquare();
	void addScore();

	void determine();
	void hit();
	void miss();

	void startGame();
	void overGame();

    void update(float dt);
	void tick(float dt);

	bool onContactBegin(cocos2d::PhysicsContact &contact);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);
	
    unsigned int score;
	unsigned int gameState;

	int time;

	bool isSquareUpdate; //говнокод:)

	bool isTouchDown;
	float initialTouchPosition;
	float currentTouchPosition;

	cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

	cocos2d::Timer* timer;

	Border* border;
	Square* square;

	cocos2d::Label* scoreLabel;
	cocos2d::Label* timeLabel;

	Mode currentMode;
};

#endif
