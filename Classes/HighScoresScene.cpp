#include "HighScoresScene.h"
#include "MainMenuScene.h"
#include "Border.h"
#include "Definitions.h"

#include "NativeUtils.h"

USING_NS_CC;

Scene* HighScoresScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HighScoresScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	return scene;
}

bool HighScoresScene::init()
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

	auto titleLabel = Label::createWithTTF("High Scores","Roboto-Light.ttf",MENU_FONT_SIZE);
	titleLabel->setColor(Color3B(WHITE,WHITE,WHITE));
	titleLabel->setPosition(BORDER_WIDTH + titleLabel->getContentSize().width / 2,BORDER_HEIGHT / 2);

	classicSprite = Sprite::create();
	classicSprite->retain();
	classicSprite->setTextureRect(Rect(0,0,visibleSize.width,BORDER_HEIGHT));
	classicSprite->setColor(Color3B(WHITE,WHITE,WHITE));
	classicSprite->setPosition(visibleSize.width + visibleSize.width / 2, BORDER_HEIGHT / 2 + BORDER_HEIGHT * 3);

	auto classicLabel = Label::createWithTTF("Classic","Roboto-Light.ttf",MENU_FONT_SIZE);
	classicLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	classicLabel->setPosition(BORDER_WIDTH * 2 + classicLabel->getContentSize().width / 2,BORDER_HEIGHT / 2);

	arcadeSprite = Sprite::create();
	arcadeSprite->retain();
	arcadeSprite->setTextureRect(Rect(0,0,visibleSize.width,BORDER_HEIGHT));
	arcadeSprite->setColor(Color3B(WHITE,WHITE,WHITE));
	arcadeSprite->setPosition(visibleSize.width + visibleSize.width / 2, BORDER_HEIGHT / 2 + BORDER_HEIGHT * 2);

	auto arcadeLabel = Label::createWithTTF("Arcade Mode","Roboto-Light.ttf",MENU_FONT_SIZE);
	arcadeLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	arcadeLabel->setPosition(BORDER_WIDTH * 2 + arcadeLabel->getContentSize().width / 2,BORDER_HEIGHT / 2);

	speedSprite = Sprite::create();
	speedSprite->retain();
	speedSprite->setTextureRect(Rect(0,0,visibleSize.width,BORDER_HEIGHT));
	speedSprite->setColor(Color3B(WHITE,WHITE,WHITE));
	speedSprite->setPosition(visibleSize.width + visibleSize.width / 2, BORDER_HEIGHT / 2 + BORDER_HEIGHT);

	auto speedLabel = Label::createWithTTF("Speed Mode","Roboto-Light.ttf",MENU_FONT_SIZE);
	speedLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	speedLabel->setPosition(BORDER_WIDTH * 2 + speedLabel->getContentSize().width / 2,BORDER_HEIGHT / 2);

	undefinedSprite = Sprite::create();
	undefinedSprite->retain();
	undefinedSprite->setTextureRect(Rect(0,0,visibleSize.width,BORDER_HEIGHT));
	undefinedSprite->setColor(Color3B(WHITE,WHITE,WHITE));
	undefinedSprite->setPosition(visibleSize.width + visibleSize.width / 2, BORDER_HEIGHT / 2);

	auto undefinedLabel = Label::createWithTTF("Hard Mode","Roboto-Light.ttf",MENU_FONT_SIZE);
	undefinedLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	undefinedLabel->setPosition(BORDER_WIDTH * 2 + undefinedLabel->getContentSize().width / 2, BORDER_HEIGHT / 2);

	addChild(backgroundSprite);
	addChild(titleSprite);
	titleSprite->addChild(titleLabel);
	addChild(classicSprite);
	classicSprite->addChild(classicLabel);
	addChild(arcadeSprite);
	arcadeSprite->addChild(arcadeLabel);
	addChild(speedSprite);
	speedSprite->addChild(speedLabel);
	addChild(undefinedSprite);
	undefinedSprite->addChild(undefinedLabel);

	auto border = new Border(this);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(HighScoresScene::onKeyReleased, this);

	setKeypadEnabled(true);

	auto touchSpriteListener = EventListenerTouchOneByOne::create();

	touchSpriteListener->setSwallowTouches(true);
	touchSpriteListener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		if(classicSprite->getBoundingBox().containsPoint(touch->getLocation()))
		{
			classicOnTouchBegan(touch,event);

			return true;
		}
		else if(arcadeSprite->getBoundingBox().containsPoint(touch->getLocation()))
		{
			arcadeOnTouchBegan(touch,event);

			return true;
		}
		else if(speedSprite->getBoundingBox().containsPoint(touch->getLocation()))
		{
			speedOnTouchBegan(touch,event);

			return true;
		}
		else if(undefinedSprite->getBoundingBox().containsPoint(touch->getLocation()))
		{
			undefinedOnTouchBegan(touch,event);

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

	classicSprite->runAction(showAction->clone());
	arcadeSprite->runAction(showAction->clone());
	speedSprite->runAction(showAction->clone());
	undefinedSprite->runAction(showAction->clone());
	titleSprite->runAction(showAction->clone());

	return true;
}
void HighScoresScene::classicOnTouchBegan(Touch* touch, Event* event)
{
	if(!NativeUtils::isSignedIn())
	{
		NativeUtils::signIn();
	}
	else
	{
		int highScore = UserDefault::getInstance()->getIntegerForKey("HIGHSCORE_CLASSIC", 0);

		if(highScore > 0)
		{
			NativeUtils::submitScore(LEADERBOARD_CLASSIC,highScore);
		}

		NativeUtils::showLeaderboard(LEADERBOARD_CLASSIC);
	}
}
void HighScoresScene::arcadeOnTouchBegan(Touch* touch, Event* event)
{
	if(!NativeUtils::isSignedIn())
	{
		NativeUtils::signIn();
	}
	else
	{
		int highScore = UserDefault::getInstance()->getIntegerForKey("HIGHSCORE_ARCADE", 0);

		if(highScore > 0)
		{
			NativeUtils::submitScore(LEADERBOARD_ARCADE,highScore);
		}

		NativeUtils::showLeaderboard(LEADERBOARD_ARCADE);
	}
}
void HighScoresScene::speedOnTouchBegan(Touch* touch, Event* event)
{
	if(!NativeUtils::isSignedIn())
	{
		NativeUtils::signIn();
	}
	else
	{
		int highScore = UserDefault::getInstance()->getIntegerForKey("HIGHSCORE_SPEED", 0);

		if(highScore > 0)
		{
			NativeUtils::submitScore(LEADERBOARD_SPEED,highScore);
		}

		NativeUtils::showLeaderboard(LEADERBOARD_SPEED);
	}
}
void HighScoresScene::undefinedOnTouchBegan(Touch* touch, Event* event)
{
	if(!NativeUtils::isSignedIn())
	{
		NativeUtils::signIn();
	}
	else
	{
		int highScore = UserDefault::getInstance()->getIntegerForKey("HIGHSCORE_UNDEFINED", 0);

		if(highScore > 0)
		{
			NativeUtils::submitScore(LEADERBOARD_UNDEFINED,highScore);
		}

		NativeUtils::showLeaderboard(LEADERBOARD_UNDEFINED);
	}
}
void HighScoresScene::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) 
	{
		auto callback = CallFunc::create([]()
		{
			auto scene = MainMenuScene::createScene();
			Director::getInstance()->replaceScene(scene);
		});

		auto sequence = Sequence::create(hideAction->clone(),DelayTime::create(MENU_DELAY),callback,nullptr);

		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menu-out.ogg");

		classicSprite->runAction(hideAction->clone());
		arcadeSprite->runAction(hideAction->clone());
		speedSprite->runAction(hideAction->clone());
		undefinedSprite->runAction(hideAction->clone());
		titleSprite->runAction(sequence);
	}
}



