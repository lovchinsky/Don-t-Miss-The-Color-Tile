#ifndef SQUARE_H
#define SQUARE_H

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

enum Direction {LEFT,RIGHT};

class Square
{
public:
    Square(cocos2d::Layer *layer);
    
    void setPosition(cocos2d::Vec2 position);
	cocos2d::Vec2 getPosition();

	void setColor();
	int getColor();

	void increaseSpeed();
	void down();
	void move(Direction direction);
	void hit();
	void miss();
	void fadeOut();

private:
	unsigned int color;
	unsigned int speed;

	Direction direction;

	cocos2d::Sprite* sprite;
	cocos2d::Action* downAction;
	
	cocos2d::Action* moveLeftAction;
	cocos2d::Action* moveRightAction;
	cocos2d::Action* hitLeftAction;
	cocos2d::Action* hitRightAction;
	cocos2d::Action* missLeftAction;
	cocos2d::Action* missRightAction;
};

#endif