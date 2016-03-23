#ifndef BORDER_H
#define BORDER_H

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <vector>

class Border
{
public:
    Border(cocos2d::Layer *layer);

	void showRight();
	void hideRight();

	bool hitLeft(cocos2d::Vec2 position,int color);
	bool hitRight(cocos2d::Vec2 position,int color);

	void move();
	void infinityScroll();
	void increaseSpeed();

private:
	unsigned int speed;

	cocos2d::Size visibleSize;

	void createSprite(cocos2d::Sprite* sprite, int backward);
	cocos2d::Sprite* spriteLeft;
	cocos2d::Sprite* spriteRight;
	cocos2d::Action* downAction;
	cocos2d::Action* showRightAction;
	cocos2d::Action* hideRightAction;
};

#endif