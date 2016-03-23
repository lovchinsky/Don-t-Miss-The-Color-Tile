#include "GameScene.h"
#include "MainMenuScene.h"

#include "Definitions.h"

USING_NS_CC;

Scene* GameScene::createScene(Mode mode)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	layer->setMode(mode);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}
bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	gameState = 0;

	auto backgroundSprite = Sprite::create();
	backgroundSprite->retain();
	backgroundSprite->setTextureRect(cocos2d::Rect(0,0,visibleSize.width,visibleSize.height));
	backgroundSprite->setColor(cocos2d::Color3B(WHITE,WHITE,WHITE));
	backgroundSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);

	addChild(backgroundSprite);

	createListeners();
	createScore();
	createBorder();

	scheduleUpdate();

	return true;
}
void GameScene::setMode(Mode mode)
{
	currentMode = mode;

	if(currentMode == ARCADE)
	{
		createTimer();
	}
}
void GameScene::createListeners()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);

	auto squareMoveListener = EventListenerCustom::create("MOVE",CC_CALLBACK_0(GameScene::determine,this));
	auto squareHitListener = EventListenerCustom::create("HIT",CC_CALLBACK_0(GameScene::hit,this));
	auto squareMissListener = EventListenerCustom::create("MISS",CC_CALLBACK_0(GameScene::miss,this));

	auto borderShowListener = EventListenerCustom::create("BORDER_SHOW",CC_CALLBACK_0(GameScene::createStartAction,this));
	auto borderHideListener = EventListenerCustom::create("BORDER_HIDE",CC_CALLBACK_0(GameScene::createGameOverScene,this));

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(squareMoveListener, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(squareHitListener, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(squareMissListener, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(borderShowListener, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(borderHideListener, this);
}
void GameScene::createScore()
{
	score = 0;

	scoreLabel = Label::createWithTTF("Score: 0","Roboto-Light.ttf",MIDDLE_FONT_SIZE);
	scoreLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	scoreLabel->setPosition(visibleSize.width / 2,visibleSize.height - scoreLabel->getContentSize().height);

	addChild(scoreLabel,1000);
}
void GameScene::createBorder()
{
	border = new Border(this);
	border->showRight();
}
void GameScene::createStartAction()
{
	auto startLabel = Label::createWithTTF("Ready","Roboto-Light.ttf",MENU_FONT_SIZE);
	startLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	startLabel->setPosition(visibleSize.width / 2,visibleSize.height * 2 / 3);

	addChild(startLabel);

	auto startAction = Sequence::create(

		DelayTime::create(1.0f),CallFunc::create([startLabel]()
	{
		startLabel->setString("Go");
	}),
		DelayTime::create(0.5f),CallFunc::create([startLabel]()
	{
		startLabel->setVisible(false);
	}),
		DelayTime::create(0.5f),CallFunc::create(CC_CALLBACK_0(GameScene::createSquare,this)),CallFunc::create(CC_CALLBACK_0(GameScene::startGame,this)),nullptr);

	runAction(startAction);
}
void GameScene::createSquare()
{
	square = new Square(this);

	if(currentMode == SPEED || currentMode == CLASSIC)
	{
		square->setPosition(Vec2(visibleSize.width / 2, visibleSize.height + SQUARE_SIZE / 2));
	}
	else if(currentMode == ARCADE || currentMode == HARD)
	{
		square->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	}

	square->setColor();

	isSquareUpdate = true;
}
void GameScene::createTimer()
{
	time = ARCADE_TIME;

	timeLabel = Label::createWithTTF("Time: 60","Roboto-Light.ttf",MIDDLE_FONT_SIZE);
	timeLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	timeLabel->setPosition(BORDER_WIDTH * 3 / 2 + timeLabel->getContentSize().width / 2,visibleSize.height - timeLabel->getContentSize().height);

	addChild(timeLabel,1000);

	scoreLabel->setPositionX(visibleSize.width - BORDER_WIDTH * 3 / 2 - scoreLabel->getContentSize().width / 2);
}
void GameScene::startGame()
{
	if(currentMode == SPEED || currentMode == CLASSIC)
	{
		square->down();
	}
	else if(currentMode == ARCADE || currentMode == HARD)
	{
		border->move();

		if(currentMode == ARCADE)
		{
			schedule(schedule_selector(GameScene::tick),1.0f,ARCADE_TIME,0);
		}
	}

	gameState = 1;
}
void GameScene::overGame()
{
	border->hideRight();
}
void GameScene::updateSquare()
{
	square->setColor();

	if(currentMode == SPEED || currentMode == CLASSIC)
	{
		if(currentMode == SPEED)
		{
			square->increaseSpeed();
		}

		square->setPosition(Vec2(visibleSize.width / 2, visibleSize.height + SQUARE_SIZE / 2));
		square->down();
	}

	if(currentMode == ARCADE || currentMode == HARD)
	{
		if(currentMode == HARD)
		{
			border->increaseSpeed();
		}
		else if(currentMode == ARCADE)
		{
			square->fadeOut();
		}

		square->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	}

	isSquareUpdate = true;
}
void GameScene::update(float dt)
{
	if(gameState == 1)
	{
		if(currentMode == ARCADE || currentMode == HARD)
		{
			border->infinityScroll();
		}

		if(square->getPosition().y + SQUARE_SIZE / 2 < 0)
		{
			gameState = 2;

			createGameOverAction();
		}

		if (isTouchDown && isSquareUpdate)
		{
			if (initialTouchPosition - currentTouchPosition > visibleSize.width * SWIPE)
			{
				square->move(LEFT);

				isTouchDown = false;
				isSquareUpdate = false;
			}
			else if (initialTouchPosition - currentTouchPosition < - visibleSize.width * SWIPE)
			{
				square->move(RIGHT);

				isTouchDown = false;
				isSquareUpdate = false;
			}
		}
	}
	else if(gameState == 2)
	{
		if(currentMode == ARCADE || currentMode == HARD)
		{
			border->infinityScroll();
		}
	}
}
void GameScene::tick(float dt)
{
	time--;

	if(time == 0)
	{
		gameState = 2;

		createGameOverAction();
	}

	std::stringstream s;
	s << "Time: " << time;

	timeLabel->setString(s.str());
}
void GameScene::determine()
{
	if(square->getPosition().x < visibleSize.width / 2)
	{
		if(border->hitLeft(square->getPosition(),square->getColor()))
		{
			square->hit();
		}
		else
		{
			square->miss();
		}
	}
	else
	{
		if(border->hitRight(square->getPosition(),square->getColor()))
		{
			square->hit();
		}
		else
		{
			square->miss();
		}
	}
}
void GameScene::hit()
{
	score++;

	std::stringstream s;
	s << "Score: " << score;

	scoreLabel->setString(s.str());

	updateSquare();
}
void GameScene::miss()
{
	if(currentMode == ARCADE)
	{
		updateSquare();
	}
	else
	{
		gameState = 2;

		createGameOverAction();
	}
}
void GameScene::createGameOverAction()
{
	auto overLabel = Label::createWithTTF("Game Over","Roboto-Light.ttf",MENU_FONT_SIZE);
	overLabel->setColor(Color3B(BLACK,BLACK,BLACK));
	overLabel->setPosition(visibleSize.width / 2,visibleSize.height * 2 / 3);

	addChild(overLabel);

	auto startAction = Sequence::create(

		DelayTime::create(1.0f),CallFunc::create([overLabel]()
	{
		overLabel->setVisible(false);
	}),
		CallFunc::create(CC_CALLBACK_0(GameScene::overGame,this)),nullptr);

	runAction(startAction);
}
void GameScene::createGameOverScene()
{
	auto scene = GameOverScene::createScene(currentMode,score);
	Director::getInstance()->replaceScene(scene);
}
bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
	auto touchLocation = touch->getLocation();
	auto nodePosition = convertToNodeSpace(touchLocation);

	if(gameState == 1)
	{
		initialTouchPosition = nodePosition.x;
		currentTouchPosition = nodePosition.x;

		isTouchDown = true;
	}

	return true;
}
void GameScene::onTouchMoved(Touch *touch, Event *event)
{
	auto touchLocation = touch->getLocation();
	auto nodePosition = convertToNodeSpace(touchLocation);

	currentTouchPosition = nodePosition.x;
}
void GameScene::onTouchEnded(Touch *touch, Event *event)
{
	isTouchDown = false;
}
void GameScene::onTouchCancelled(Touch *touch, Event *event)
{
	onTouchEnded(touch, event);
}