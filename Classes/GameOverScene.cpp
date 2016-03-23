#include "GameOverScene.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "Definitions.h"

#include "NativeUtils.h"

USING_NS_CC;

Scene* GameOverScene::createScene(Mode mode, unsigned int score)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOverScene::create();

	layer->setMode(mode);
	layer->setScore(score);

	// add layer as a child to scene
	scene->addChild(layer);

	return scene;
}
void GameOverScene::setMode(Mode mode)
{
	currentMode = mode;

	switch (currentMode)
	{
	case CLASSIC:
		titleLabel->setString("Classic");
		break;
	case ARCADE:
		titleLabel->setString("Arcade Mode");
		break;
	case SPEED:
		titleLabel->setString("Speed Mode");
		break;
	case HARD:
		titleLabel->setString("Hard Mode");
		break;
	default:
		break;
	}

	titleLabel->setPosition(BORDER_WIDTH + titleLabel->getContentSize().width / 2,BORDER_HEIGHT / 2);
}
void GameOverScene::setScore(unsigned int score)
{
	currentScore = score;

	auto userDefault = UserDefault::getInstance();

	switch (currentMode)
	{
	case CLASSIC:
		highScore = userDefault->getIntegerForKey("HIGHSCORE_CLASSIC", 0);
		break;
	case ARCADE:
		highScore = userDefault->getIntegerForKey("HIGHSCORE_ARCADE", 0);
		break;
	case SPEED:
		highScore = userDefault->getIntegerForKey("HIGHSCORE_SPEED", 0);
		break;
	case HARD:
		highScore = userDefault->getIntegerForKey("HIGHSCORE_UNDEFINED", 0);
		break;
	default:
		break;
	}

	if (score > highScore)
	{
		highScore = score;

		switch (currentMode)
		{
		case CLASSIC:
			userDefault->setIntegerForKey("HIGHSCORE_CLASSIC", highScore);
			break;
		case ARCADE:
			userDefault->setIntegerForKey("HIGHSCORE_ARCADE", highScore);
			break;
		case SPEED:
			userDefault->setIntegerForKey("HIGHSCORE_SPEED", highScore);
			break;
		case HARD:
			userDefault->setIntegerForKey("HIGHSCORE_UNDEFINED", highScore);
			break;
		default:
			break;
		}
		
		userDefault->flush();

		highScoreLabel->setColor(Color3B(RED_R,RED_G,RED_B));

		if(NativeUtils::isSignedIn())
		{
			switch (currentMode)
			{
			case CLASSIC:
				NativeUtils::submitScore(LEADERBOARD_CLASSIC,highScore);
				break;
			case ARCADE:
				NativeUtils::submitScore(LEADERBOARD_ARCADE,highScore);
				break;
			case SPEED:
				NativeUtils::submitScore(LEADERBOARD_SPEED,highScore);
				break;
			case HARD:
				NativeUtils::submitScore(LEADERBOARD_UNDEFINED,highScore);
				break;
			default:
				break;
			}
		}
	}

	std::stringstream s;
	s << "Score: " << currentScore;

	scoreLabel->setString(s.str());

	s.str("");
	s.clear();
	s << "Best: " << highScore;

	highScoreLabel->setString(s.str());

	scoreLabel->setPosition(BORDER_WIDTH * 2 + scoreLabel->getContentSize().width / 2,BORDER_HEIGHT / 2);
	highScoreLabel->setPosition(BORDER_WIDTH * 2 + highScoreLabel->getContentSize().width / 2,BORDER_HEIGHT / 2);
}
bool GameOverScene::init()
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

	titleLabel = Label::createWithTTF("","Roboto-Light.ttf",MENU_FONT_SIZE);
	titleLabel->setColor(Color3B(WHITE,WHITE,WHITE));

	scoreSprite = Sprite::create();
	scoreSprite->retain();
	scoreSprite->setTextureRect(Rect(0,0,visibleSize.width,BORDER_HEIGHT));
	scoreSprite->setColor(Color3B(WHITE,WHITE,WHITE));
	scoreSprite->setPosition(visibleSize.width + visibleSize.width / 2, BORDER_HEIGHT / 2 + BORDER_HEIGHT * 3);

	scoreLabel = Label::createWithTTF("Score: 0","Roboto-Light.ttf",MENU_FONT_SIZE);
	scoreLabel->setColor(Color3B(GREY,GREY,GREY));

	highScoreSprite = Sprite::create();
	highScoreSprite->retain();
	highScoreSprite->setTextureRect(Rect(0,0,visibleSize.width,BORDER_HEIGHT));
	highScoreSprite->setColor(Color3B(WHITE,WHITE,WHITE));
	highScoreSprite->setPosition(visibleSize.width + visibleSize.width / 2, BORDER_HEIGHT / 2 + BORDER_HEIGHT * 2);

	highScoreLabel = Label::createWithTTF("Best: 0","Roboto-Light.ttf",MENU_FONT_SIZE);
	highScoreLabel->setColor(Color3B(GREY,GREY,GREY));

	replaySprite = Sprite::create();
	replaySprite->retain();
	replaySprite->setTextureRect(Rect(0,0,visibleSize.width,BORDER_HEIGHT));
	replaySprite->setColor(Color3B(WHITE,WHITE,WHITE));
	replaySprite->setPosition(visibleSize.width + visibleSize.width / 2, BORDER_HEIGHT / 2 + BORDER_HEIGHT);

	auto replayLabel = Label::createWithTTF("Replay","Roboto-Light.ttf",MENU_FONT_SIZE);
	replayLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	replayLabel->setPosition(BORDER_WIDTH * 2 + replayLabel->getContentSize().width / 2,BORDER_HEIGHT / 2);

	mainMenuSprite = Sprite::create();
	mainMenuSprite->retain();
	mainMenuSprite->setTextureRect(Rect(0,0,visibleSize.width,BORDER_HEIGHT));
	mainMenuSprite->setColor(Color3B(WHITE,WHITE,WHITE));
	mainMenuSprite->setPosition(visibleSize.width + visibleSize.width / 2, BORDER_HEIGHT / 2);

	auto mainMenuLabel = Label::createWithTTF("Main Menu","Roboto-Light.ttf",MENU_FONT_SIZE);
	mainMenuLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	mainMenuLabel->setPosition(BORDER_WIDTH * 2 + mainMenuLabel->getContentSize().width / 2, BORDER_HEIGHT / 2);

	addChild(backgroundSprite);
	addChild(titleSprite);
	titleSprite->addChild(titleLabel);
	addChild(scoreSprite);
	scoreSprite->addChild(scoreLabel);
	addChild(highScoreSprite);
	highScoreSprite->addChild(highScoreLabel);
	addChild(replaySprite);
	replaySprite->addChild(replayLabel);
	addChild(mainMenuSprite);
	mainMenuSprite->addChild(mainMenuLabel);

	auto border = new Border(this);

	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameOverScene::onKeyReleased, this);

	setKeypadEnabled(true);

	auto touchSpriteListener = EventListenerTouchOneByOne::create();

	touchSpriteListener->setSwallowTouches(true);
	touchSpriteListener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		if(replaySprite->getBoundingBox().containsPoint(touch->getLocation()))
		{
			replayOnTouchBegan(touch,event);

			return true;
		}
		else if(mainMenuSprite->getBoundingBox().containsPoint(touch->getLocation()))
		{
			mainMenuOnTouchBegan(touch,event);

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

	scoreSprite->runAction(showAction->clone());
	highScoreSprite->runAction(showAction->clone());
	replaySprite->runAction(showAction->clone());
	mainMenuSprite->runAction(showAction->clone());
	titleSprite->runAction(showAction->clone());

	NativeUtils::showAd();

	return true;
}
void GameOverScene::replayGame(Mode mode)
{
	auto callback = CallFunc::create([mode]()
	{
		auto scene = GameScene::createScene(mode);
		Director::getInstance()->replaceScene(scene);
	});

	auto sequence = Sequence::create(hideAction->clone(),DelayTime::create(MENU_DELAY),callback,nullptr);

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menu-out.ogg");

	scoreSprite->runAction(hideAction->clone());
	highScoreSprite->runAction(hideAction->clone());
	replaySprite->runAction(hideAction->clone());
	mainMenuSprite->runAction(hideAction->clone());
	titleSprite->runAction(sequence);
}
void GameOverScene::replayOnTouchBegan(Touch* touch, Event* event)
{
	replayGame(currentMode);
}
void GameOverScene::mainMenuOnTouchBegan(Touch* touch, Event* event)
{
	auto callback = CallFunc::create([]()
	{
		auto scene = MainMenuScene::createScene();
		Director::getInstance()->replaceScene(scene);
	});

	auto sequence = Sequence::create(hideAction->clone(),DelayTime::create(MENU_DELAY),callback,nullptr);

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("menu-out.ogg");

	scoreSprite->runAction(hideAction->clone());
	highScoreSprite->runAction(hideAction->clone());
	replaySprite->runAction(hideAction->clone());
	mainMenuSprite->runAction(hideAction->clone());
	titleSprite->runAction(sequence);
}
void GameOverScene::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
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

		scoreSprite->runAction(hideAction->clone());
		highScoreSprite->runAction(hideAction->clone());
		replaySprite->runAction(hideAction->clone());
		mainMenuSprite->runAction(hideAction->clone());
		titleSprite->runAction(sequence);
	}
}




