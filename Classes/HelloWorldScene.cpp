/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "GameOver.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    

    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    auto layer = HelloWorld::create();
    
    layer->setPhysicWorld(scene->getPhysicsWorld());
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Nhac-Chuong-Waiting-For-You-MONO.mp3");
    scene->addChild(layer);
    return scene;
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }


    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    auto background = Sprite::create("background.png");
    background->setAnchorPoint(Vec2(0, 0));
    background->setPosition(Vec2(0, 0));
    background->setScaleY(3);
    background->setScaleX(2.5);
    addChild(background);

    /*auto bg = Sprite::createWithSpriteFrameName("b87412badafc4f6cdd89705e7b5e2ee6yEuu02zpOykJPXnX.png");
    bg->setPosition(0, 0);
    this->addChild(bg);*/


    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT,1);
    edgeBody->setCollisionBitmask(2);
    edgeBody->setContactTestBitmask(true);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Vec2(visibleSize.width /2 + origin.x, visibleSize.height /2 + origin.y));
    edgeNode->setPhysicsBody(edgeBody);
    addChild(edgeNode);

    
    

    __String *tempScore = __String::createWithFormat("%i", score);
    label = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * 0.1);
    label->setPosition(Vec2(visibleSize.width / 9, visibleSize.height * 0.9));
    label->setColor(Color3B::RED);
    addChild(label);

    
    this->schedule(schedule_selector(HelloWorld::CreatePipe), 0.005 * visibleSize.width);
    this->scheduleUpdate();
    

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::OnTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::OnContactBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    bird = Sprite::create("bird.png");
    bird->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    auto birdPhysic = PhysicsBody::createBox(bird->getContentSize());
    birdPhysic->setContactTestBitmask(true);
    birdPhysic->setCollisionBitmask(4);
    bird->setPhysicsBody(birdPhysic);
    addChild(bird);

    return true;
}

bool HelloWorld::OnContactBegan(cocos2d::PhysicsContact& contact)
{
    PhysicsBody* a = contact.getShapeA()->getBody();
    PhysicsBody* b = contact.getShapeB()->getBody();

    if (a->getCollisionBitmask() == 4 && b->getCollisionBitmask() == 3 || a->getCollisionBitmask() == 3 && b->getCollisionBitmask() == 4)
    {
        ++score;
        __String* tempScore = __String::createWithFormat("%i", score);
        label->setString(tempScore->getCString());
    }
    else
    {
        if (a->getCollisionBitmask() == 4 && b->getCollisionBitmask() == 2 || a->getCollisionBitmask() == 2 && b->getCollisionBitmask() == 4)
        {
            //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("klonk.mp3");
            auto sqe = GameOver::createScene();
            Director::getInstance()->replaceScene(sqe);
            CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        }
    }
    return true;
}

void HelloWorld::stopFly(float dt)
{
    
    isFalling = true;
}

bool HelloWorld::OnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    isFalling = false;
    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("cartoonslip.mp3");
    this->scheduleOnce(schedule_selector(HelloWorld::stopFly), 0.25);
    return true;
}

void HelloWorld::update(float dt)
{
    

    Size visibleSize = Director::getInstance()->getVisibleSize();
    if (isFalling == true)
    {
        
        bird->setPositionX(visibleSize.width/2);
        bird->setPositionY(bird->getPositionY()
            - visibleSize.height * 0.008);

    }
    else
    {
        bird->setPositionX(visibleSize.width / 2);
        bird->setPositionY(bird->getPositionY()
            + visibleSize.height * 0.004);
    }
}

void HelloWorld::CreatePipe(float dt)
{
    if (CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying() == false)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Nhac-Chuong-Waiting-For-You-MONO.mp3");
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto topPipe = Sprite::create("bar.png");
    auto bottomPipe = Sprite::create("bar (1).png");

    auto topPhysic = PhysicsBody::createBox(Size(topPipe->getContentSize().width, topPipe->getContentSize().height-5));
    topPhysic->setCollisionBitmask(2);
    topPhysic->setContactTestBitmask(true);
    topPipe->setPhysicsBody(topPhysic);

    auto bottomPhysic = PhysicsBody::createBox(Size(bottomPipe->getContentSize().width, bottomPipe->getContentSize().height-31));
    bottomPhysic->setCollisionBitmask(2);
    bottomPhysic->setContactTestBitmask(true);
    bottomPipe->setPhysicsBody(bottomPhysic);

    topPhysic->setDynamic(false);
    bottomPhysic->setDynamic(false);

    auto random = CCRANDOM_0_1();
    if (random < 0.35)
    {
        random = 0.35;
    }
    else
    {
        if (random > 0.65)
        {
            random = 0.65;
        }
    }

    auto topPipePosition = (visibleSize.height * random) + (topPipe->getContentSize().height / 2);
    topPipe->setPosition(Vec2(visibleSize.width + topPipe->getContentSize().width, topPipePosition));
    auto random1 = CCRANDOM_0_1();
    bottomPipe->setPosition(Vec2(topPipe->getPositionX(), topPipePosition-((Sprite::create("bird.png")->getContentSize().height * 2) + (random1 * 40)) - topPipe->getContentSize().height));

    addChild(topPipe);
    addChild(bottomPipe);

   
   

    auto topMove = MoveBy::create(0.01 * visibleSize.width, Vec2(- visibleSize.width * speed,0));

    auto bottomMove = MoveBy::create(0.01 * visibleSize.width, Vec2(-visibleSize.width * speed, 0));

    topPipe->runAction(topMove);
    bottomPipe->runAction(bottomMove);

    auto pointNode = Node::create();
    pointNode->setPosition(Vec2(topPipe->getPositionX(), topPipe->getPositionY() - topPipe->getContentSize().height / 2 - (Sprite::create("bird.png")->getContentSize().height * 2 + (random1 * 40)) / 2));
    auto nodePhysic = PhysicsBody::createBox(Size(1, (Sprite::create("bird.png")->getContentSize().height * 2) + (random1 * 40)));

    nodePhysic->setCollisionBitmask(3);
    nodePhysic->setContactTestBitmask(true);
    nodePhysic->setDynamic(false);
    pointNode->setPhysicsBody(nodePhysic);
   
    auto nodeMove = MoveBy::create(0.01 * visibleSize.width, Vec2(-visibleSize.width * speed, 0));
    pointNode->runAction(nodeMove);
    addChild(pointNode);

    speed = speed * 1.05;

}


