#ifndef MODE_SCENE_H
#define MODE_SCENE_H

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

enum Mode {CLASSIC,ARCADE,SPEED,HARD};

class ModeScene: public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(ModeScene);
    
private:
	cocos2d::Sprite* titleSprite;
	cocos2d::Sprite* classicSprite;
	cocos2d::Sprite* arcadeSprite;
	cocos2d::Sprite* speedSprite;
	cocos2d::Sprite* undefinedSprite;
	cocos2d::FiniteTimeAction* showAction;
	cocos2d::FiniteTimeAction* hideAction;

	void startGame(Mode mode);

	void classicOnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void arcadeOnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void speedOnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void undefinedOnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
};

#endif
