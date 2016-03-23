#include "Border.h"
#include "Definitions.h"

USING_NS_CC;

Border::Border(Layer *layer)
{
	visibleSize = Director::getInstance()->getVisibleSize();

	spriteLeft = Sprite::create();
	spriteLeft->retain();
	spriteLeft->setPosition(BORDER_WIDTH / 2,0);
	createSprite(spriteLeft,0);

	spriteRight = Sprite::create();
	spriteRight->retain();
	spriteRight->setPosition(BORDER_WIDTH / 2 + visibleSize.width,0);
	createSprite(spriteRight,1);

	downAction = RepeatForever::create(MoveBy::create(MOVE_DOWN_TIME,Vec2(0,MOVE_DOWN_DX)));
	downAction->retain();

	auto move = MoveBy::create(BORDER_SHOW_DT,Vec2(BORDER_WIDTH,0));

	auto callbackShow = CallFunc::create([](){
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("BORDER_SHOW");
	});

	auto callbackHide = CallFunc::create([](){
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("BORDER_HIDE");
	});

	showRightAction = Sequence::create(EaseExponentialOut::create(move->reverse()),callbackShow,nullptr);
	showRightAction->retain();

	hideRightAction = Sequence::create(EaseExponentialIn::create(move->clone()),DelayTime::create(0.5f),callbackHide,nullptr);
	hideRightAction->retain();

	speed = 0;

	layer->addChild(spriteLeft,1000);
	layer->addChild(spriteRight,1000);
}
void Border::createSprite(Sprite* sprite, int backward)
{
	std::vector<Sprite*> sprites;
	std::vector<Color3B> colors;

	colors.push_back(Color3B(RED_R,RED_G,RED_B));
	colors.push_back(Color3B(YELLOW_R,YELLOW_G,YELLOW_B));
	colors.push_back(Color3B(GREEN_R,GREEN_G,GREEN_B));
	colors.push_back(Color3B(BLUE_R,BLUE_G,BLUE_B));
	colors.push_back(Color3B(PURLE_R,PURLE_G,PURLE_B));

	for(int i=0; i<NUMBER_OF_COLORS; i++)
	{
		sprites.push_back(Sprite::create());
		sprites.back()->retain();
		sprites.back()->setTag(sprites.size() - 1);
		sprites.back()->setTextureRect(Rect(0,0,BORDER_WIDTH,BORDER_HEIGHT));
		sprites.back()->setColor(colors.at(abs(NUMBER_OF_COLORS - 1 - i - (NUMBER_OF_COLORS - 1) * backward)));
		sprites.back()->setPosition(0,(i * 2 + 1) * (BORDER_HEIGHT) / 2);

		sprite->addChild(sprites.back());
	}

	Sprite* infinity = Sprite::create();
	infinity->retain();
	infinity->setTag(NUMBER_OF_COLORS);
	infinity->setTextureRect(Rect(0,0,BORDER_WIDTH,BORDER_HEIGHT));

	if(backward)
	{
		infinity->setPosition(0,sprites.back()->getPositionY() + BORDER_HEIGHT);
	}
	else
	{
		infinity->setPosition(0,sprites.front()->getPositionY() - BORDER_HEIGHT);
	}

	sprite->addChild(infinity);
}
bool Border::hitLeft(Vec2 position,int color)
{
	Node *node = spriteLeft->getChildByTag(abs((NUMBER_OF_COLORS - 1) - color));

	if(node->getPositionY() + BORDER_HEIGHT / 2 > position.y + SQUARE_SIZE / 2 && node->getPositionY() - BORDER_HEIGHT / 2 < position.y - SQUARE_SIZE / 2)
	{
		return true;
	}

	return false;
}
bool Border::hitRight(Vec2 position,int color)
{
	Node *node = spriteRight->getChildByTag(color);

	if(node->getPositionY() + BORDER_HEIGHT / 2 > position.y + SQUARE_SIZE / 2 && node->getPositionY() - BORDER_HEIGHT / 2 < position.y - SQUARE_SIZE / 2)
	{
		return true;
	}

	return false;
}
void Border::increaseSpeed()
{
	if(speed < MOVE_MAX_SPEED)
	{
		speed += INCREASE_MOVE_SPEED(speed);

		downAction = RepeatForever::create(MoveBy::create(MOVE_DOWN_TIME,Vec2(0,MOVE_DOWN_DX - MOVE_DOWN_TIME * speed)));
		downAction->retain();

		move();
	}
}
void Border::infinityScroll()
{
	Node* nodeLeftOld;
	Node* nodeLeftNew;
	Node* nodeRightOld;
	Node* nodeRightNew;

	for(int i=0; i<NUMBER_OF_COLORS; i++)
	{
		nodeLeftOld = spriteLeft->getChildByTag(i);
		nodeLeftNew = spriteLeft->getChildByTag(NUMBER_OF_COLORS);
		nodeRightOld = spriteRight->getChildByTag(i);
		nodeRightNew = spriteRight->getChildByTag(NUMBER_OF_COLORS);

		if(nodeLeftOld->getPositionY() + nodeLeftOld->getContentSize().height / 2 > visibleSize.height && nodeLeftOld->getColor() != nodeLeftNew->getColor())
		{
			nodeLeftNew->setColor(nodeLeftOld->getColor());
		}
		if(nodeRightOld->getPositionY() - nodeRightOld->getContentSize().height / 2 < 0 && nodeRightOld->getColor() != nodeRightNew->getColor())
		{
			nodeRightNew->setColor(nodeRightOld->getColor());
		}

		if(nodeLeftOld->getPositionY() - nodeLeftOld->getContentSize().height / 2 > visibleSize.height)
		{
			int tagOld = nodeLeftOld->getTag();
			nodeLeftOld->setTag(nodeLeftNew->getTag());
			nodeLeftNew->setTag(tagOld);

			nodeLeftOld->setPositionY(nodeLeftNew->getPositionY() - nodeLeftOld->getContentSize().height);
		}
		if(nodeRightOld->getPositionY() + nodeRightOld->getContentSize().height / 2 < 0)
		{
			int tagOld = nodeRightOld->getTag();
			nodeRightOld->setTag(nodeRightNew->getTag());
			nodeRightNew->setTag(tagOld);

			nodeRightOld->setPositionY(nodeRightNew->getPositionY() + nodeRightOld->getContentSize().height);
		}
	}
}
void Border::move()
{
	for(int i=0; i<NUMBER_OF_COLORS + 1; i++)
	{
		Node* nodeLeft = spriteLeft->getChildByTag(i);
		Node* nodeRight = spriteRight->getChildByTag(i);

		nodeLeft->stopAllActions();
		nodeRight->stopAllActions();

		nodeLeft->runAction(downAction->reverse());
		nodeRight->runAction(downAction->clone());
	}
}
void Border::showRight()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menu-in.ogg");

	spriteRight->stopAllActions();
	spriteRight->runAction(showRightAction);
}
void Border::hideRight()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menu-out.ogg");

	for(int i=0; i<NUMBER_OF_COLORS + 1; i++)
	{
		Node* nodeRight = spriteRight->getChildByTag(i);

		nodeRight->stopAllActions();
	}

	spriteRight->stopAllActions();
	spriteRight->runAction(hideRightAction);
}

