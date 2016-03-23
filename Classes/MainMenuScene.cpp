#include "MainMenuScene.h"
#include "ModeScene.h"
#include "HighScoresScene.h"
#include "Border.h"
#include "Definitions.h"

#include "NativeUtils.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	return scene;
}

bool MainMenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create();
	backgroundSprite->retain();
	backgroundSprite->setTextureRect(Rect(0,0,visibleSize.width,visibleSize.height));
	backgroundSprite->setColor(Color3B(WHITE,WHITE,WHITE));
	backgroundSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);

	titleSprite = Sprite::create();
	titleSprite->retain();
	titleSprite->setTextureRect(Rect(0,0,visibleSize.width - BORDER_WIDTH,BORDER_HEIGHT));
	titleSprite->setColor(Color3B(BLACK,BLACK,BLACK));
	titleSprite->setPosition(visibleSize.width + visibleSize.width / 2 + BORDER_WIDTH / 2, BORDER_HEIGHT / 2 + BORDER_HEIGHT * 4);

	auto titleLabel = Label::createWithTTF("Main Menu","Roboto-Light.ttf",MENU_FONT_SIZE);
	titleLabel->setColor(Color3B(WHITE,WHITE,WHITE));
	titleLabel->setPosition(BORDER_WIDTH + titleLabel->getContentSize().width / 2,BORDER_HEIGHT / 2);

	playSprite = Sprite::create();
	playSprite->retain();
	playSprite->setTextureRect(Rect(0,0,visibleSize.width,BORDER_HEIGHT));
	playSprite->setColor(Color3B(WHITE,WHITE,WHITE));
	playSprite->setPosition(visibleSize.width + visibleSize.width / 2, BORDER_HEIGHT / 2 + BORDER_HEIGHT * 3);

	auto playLabel = Label::createWithTTF("Play","Roboto-Light.ttf",MENU_FONT_SIZE);
	playLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	playLabel->setPosition(BORDER_WIDTH * 2 + playLabel->getContentSize().width / 2,BORDER_HEIGHT / 2);

	highScoresSprite = Sprite::create();
	highScoresSprite->retain();
	highScoresSprite->setTextureRect(Rect(0,0,visibleSize.width,BORDER_HEIGHT));
	highScoresSprite->setColor(Color3B(WHITE,WHITE,WHITE));
	highScoresSprite->setPosition(visibleSize.width + visibleSize.width / 2, BORDER_HEIGHT / 2 + BORDER_HEIGHT * 2);

	auto highScoresLabel = Label::createWithTTF("High Scores","Roboto-Light.ttf",MENU_FONT_SIZE);
	highScoresLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	highScoresLabel->setPosition(BORDER_WIDTH * 2 + highScoresLabel->getContentSize().width / 2,BORDER_HEIGHT / 2);

	rateSprite = Sprite::create();
	rateSprite->retain();
	rateSprite->setTextureRect(Rect(0,0,visibleSize.width,BORDER_HEIGHT));
	rateSprite->setColor(Color3B(WHITE,WHITE,WHITE));
	rateSprite->setPosition(visibleSize.width + visibleSize.width / 2, BORDER_HEIGHT / 2 + BORDER_HEIGHT);

	auto rateLabel = Label::createWithTTF("Rate","Roboto-Light.ttf",MENU_FONT_SIZE);
	rateLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	rateLabel->setPosition(BORDER_WIDTH * 2 + rateLabel->getContentSize().width / 2,BORDER_HEIGHT / 2);

	moreGamesSprite = Sprite::create();
	moreGamesSprite->retain();
	moreGamesSprite->setTextureRect(Rect(0,0,visibleSize.width,BORDER_HEIGHT));
	moreGamesSprite->setColor(Color3B(WHITE,WHITE,WHITE));
	moreGamesSprite->setPosition(visibleSize.width + visibleSize.width / 2, BORDER_HEIGHT / 2);

	auto moreGamesLabel = Label::createWithTTF("More Games","Roboto-Light.ttf",MENU_FONT_SIZE);
	moreGamesLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	moreGamesLabel->setPosition(BORDER_WIDTH * 2 + moreGamesLabel->getContentSize().width / 2, BORDER_HEIGHT / 2);

	addChild(backgroundSprite);
	addChild(titleSprite);
	titleSprite->addChild(titleLabel);
	addChild(playSprite);
	playSprite->addChild(playLabel);
	addChild(highScoresSprite);
	highScoresSprite->addChild(highScoresLabel);
	addChild(rateSprite);
	rateSprite->addChild(rateLabel);
	addChild(moreGamesSprite);
	moreGamesSprite->addChild(moreGamesLabel);

	auto border = new Border(this);

	auto touchSpriteListener = EventListenerTouchOneByOne::create();
	touchSpriteListener->setSwallowTouches(true);
	
	touchSpriteListener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		if(playSprite->getBoundingBox().containsPoint(touch->getLocation()))
		{
			playOnTouchBegan(touch,event);

			return true;
		}
		else if(highScoresSprite->getBoundingBox().containsPoint(touch->getLocation()))
		{
			highScoresOnTouchBegan(touch,event);

			return true;
		}
		else if(rateSprite->getBoundingBox().containsPoint(touch->getLocation()))
		{
			rateOnTouchBegan(touch,event);

			return true;
		}
		else if(moreGamesSprite->getBoundingBox().containsPoint(touch->getLocation()))
		{
			moreGamesOnTouchBegan(touch,event);

			return true;
		}
		return false;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchSpriteListener,this);

	auto move = MoveBy::create(MENU_MOVE_DT,Vec2(visibleSize.width, 0));

	showAction = EaseExponentialOut::create(move->reverse());
	showAction->retain();

	hideAction = EaseExponentialIn::create(move->clone());
	hideAction->retain();

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menu-in.ogg");

	playSprite->runAction(showAction->clone());
	highScoresSprite->runAction(showAction->clone());
	rateSprite->runAction(showAction->clone());
	moreGamesSprite->runAction(showAction->clone());
	titleSprite->runAction(showAction->clone());

	return true;
}
void MainMenuScene::playOnTouchBegan(Touch* touch, Event* event)
{
	auto callback = CallFunc::create([]()
	{
		auto scene = ModeScene::createScene();
		Director::getInstance()->replaceScene(scene);
	});

	auto sequence = Sequence::create(hideAction->clone(),DelayTime::create(MENU_DELAY),callback,nullptr);

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menu-out.ogg");

	playSprite->runAction(hideAction->clone());
	highScoresSprite->runAction(hideAction->clone());
	rateSprite->runAction(hideAction->clone());
	moreGamesSprite->runAction(hideAction->clone());
	titleSprite->runAction(sequence);
}
void MainMenuScene::highScoresOnTouchBegan(Touch* touch, Event* event)
{
	auto callback = CallFunc::create([]()
	{
		auto scene = HighScoresScene::createScene();
		Director::getInstance()->replaceScene(scene);
	});

	auto sequence = Sequence::create(hideAction->clone(),DelayTime::create(MENU_DELAY),callback,nullptr);

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menu-out.ogg");

	playSprite->runAction(hideAction->clone());
	highScoresSprite->runAction(hideAction->clone());
	rateSprite->runAction(hideAction->clone());
	moreGamesSprite->runAction(hideAction->clone());
	titleSprite->runAction(sequence);
}
void MainMenuScene::rateOnTouchBegan(Touch* touch, Event* event)
{
	NativeUtils::goRate();
}
void MainMenuScene::moreGamesOnTouchBegan(Touch* touch, Event* event)
{
	NativeUtils::goMoreGames();
}




