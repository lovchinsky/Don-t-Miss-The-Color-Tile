#include "Square.h"
#include "Definitions.h"

USING_NS_CC;

Square::Square(Layer *layer)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	sprite = Sprite::create();
	sprite->retain();
	sprite->setTextureRect(Rect(0,0,SQUARE_SIZE,SQUARE_SIZE));

	downAction = RepeatForever::create(MoveBy::create(MOVE_DOWN_TIME,Vec2(0,MOVE_DOWN_DX)));
	downAction->retain();

	auto move = MoveBy::create(SQUARE_MOVE_DT,Vec2(visibleSize.width / 2 - SQUARE_SIZE,0));
	auto hit = MoveBy::create((SQUARE_SIZE * SQUARE_MOVE_DT / (visibleSize.width / 2 - SQUARE_SIZE)) / SQUARE_EASE_RATE,Vec2(SQUARE_SIZE,0));

	auto delayUpdate = DelayTime::create(SQUARE_UPDATE_DT);

	auto callbackMove = CallFunc::create([](){
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("MOVE");
	});

	auto callbackHit = CallFunc::create([](){
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("HIT");
	});
	
	auto callbackMiss = CallFunc::create([](){
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("MISS");
	});

	moveLeftAction = Sequence::create(EaseIn::create(move->reverse(),SQUARE_EASE_RATE),callbackMove,nullptr);
	moveLeftAction->retain();

	moveRightAction = Sequence::create(EaseIn::create(move->clone(),SQUARE_EASE_RATE),callbackMove,nullptr);
	moveRightAction->retain();

	hitLeftAction = Sequence::create(hit->reverse(),delayUpdate->clone(),callbackHit->clone(),nullptr);
	hitLeftAction->retain();

	hitRightAction = Sequence::create(hit->clone(),delayUpdate->clone(),callbackHit->clone(),nullptr);
	hitRightAction->retain();

	missLeftAction = Sequence::create(Spawn::createWithTwoActions(EaseOut::create(move->clone(),SQUARE_EASE_RATE),FadeOut::create(SQUARE_MOVE_DT)),delayUpdate->clone(),callbackMiss->clone(),nullptr);
	missLeftAction->retain();

	missRightAction = Sequence::create(Spawn::createWithTwoActions(EaseOut::create(move->reverse(),SQUARE_EASE_RATE),FadeOut::create(SQUARE_MOVE_DT)),delayUpdate->clone(),callbackMiss->clone(),nullptr);
	missRightAction->retain();

	speed = 0;

	layer->addChild(sprite);
}
void Square::setPosition(Vec2 position)
{
	sprite->setPosition(position);
}
Vec2 Square::getPosition()
{
	return sprite->getPosition();
}
void Square::setColor()
{
	color = CCRANDOM_0_1() * NUMBER_OF_COLORS;

	switch (color)
	{
	case 0:
		sprite->setColor(Color3B(RED_R,RED_G,RED_B));
		break;
	case 1:
		sprite->setColor(Color3B(YELLOW_R,YELLOW_G,YELLOW_B));
		break;
	case 2:
		sprite->setColor(Color3B(GREEN_R,GREEN_G,GREEN_B));
		break;
	case 3:
		sprite->setColor(Color3B(BLUE_R,BLUE_G,BLUE_B));
		break;
	case 4:
		sprite->setColor(Color3B(PURLE_R,PURLE_G,PURLE_B));
		break;
	default:
		break;
	}
}
int Square::getColor()
{
	return color;
}
void Square::increaseSpeed()
{
	if(speed < MOVE_MAX_SPEED)
	{
		speed += INCREASE_MOVE_SPEED(speed);

		downAction = RepeatForever::create(MoveBy::create(MOVE_DOWN_TIME,Vec2(0,MOVE_DOWN_DX - MOVE_DOWN_TIME * speed)));
		downAction->retain();
	}
}
void Square::down()
{
	sprite->stopAllActions();
	sprite->runAction(downAction);
}
void Square::move(Direction direction)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("whoosh-in.ogg");

	sprite->stopAllActions();

	this->direction = direction;

	switch (direction)
	{
	case LEFT:
		sprite->runAction(moveLeftAction);
		break;
	case RIGHT:
		sprite->runAction(moveRightAction);
		break;
	default:
		break;
	}
}
void Square::hit()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hit.ogg");

	sprite->stopAllActions();

	switch (direction)
	{
	case LEFT:
		sprite->runAction(hitLeftAction);
		break;
	case RIGHT:
		sprite->runAction(hitRightAction);
		break;
	default:
		break;
	}
}
void Square::miss()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("whoosh-out.ogg");

	sprite->stopAllActions();

	switch (direction)
	{
	case LEFT:
		sprite->runAction(missLeftAction);
		break;
	case RIGHT:
		sprite->runAction(missRightAction);
		break;
	default:
		break;
	}
}
void Square::fadeOut()
{
	sprite->runAction(FadeIn::create(0));
}


