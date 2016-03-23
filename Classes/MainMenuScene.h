#ifndef MAIN_MENU_SCENE_H
#define MAIN_MENU_SCENE_H

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class MainMenuScene: public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);
    
private:
	cocos2d::Sprite* titleSprite;
	cocos2d::Sprite* playSprite;
	cocos2d::Sprite* highScoresSprite;
	cocos2d::Sprite* rateSprite;
	cocos2d::Sprite* moreGamesSprite;
	cocos2d::FiniteTimeAction* showAction;
	cocos2d::FiniteTimeAction* hideAction;

	void playOnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void highScoresOnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void rateOnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void moreGamesOnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
};

#endif // __MAIN_MENU_SCENE_H__
