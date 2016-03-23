#include "SplashScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"

#include "NativeUtils.h"

USING_NS_CC;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool SplashScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("move.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("hit.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("menu-in.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("menu-out.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("whoosh-in.ogg");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("whoosh-out.ogg");

    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    this->scheduleOnce(schedule_selector(SplashScene::GoToMainMenuScene), DISPLAY_TIME_SPLASH_SCENE);

    auto backgroundSprite = Sprite::create();
	backgroundSprite->retain();
	backgroundSprite->setTextureRect(cocos2d::Rect(0,0,visibleSize.width,visibleSize.height));
	backgroundSprite->setColor(cocos2d::Color3B(WHITE,WHITE,WHITE));
	backgroundSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    
	this->addChild(backgroundSprite);
    
    return true;
}

void SplashScene::GoToMainMenuScene(float dt)
{
    auto scene = MainMenuScene::createScene();
    
    Director::getInstance()->replaceScene(scene);
}



