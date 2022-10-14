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

#include "MenuGameScene.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* MenuGameScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = MenuGameScene::create();

    

    scene->addChild(layer);
    return scene;
}


// on "init" you need to initialize your instance
bool MenuGameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
   
    auto bgmenu = Sprite::create("menu.png");
    bgmenu->setPosition(visibleSize.width /2,visibleSize.height/2);
    bgmenu->setScale(2);
    this->addChild(bgmenu);

    auto play = MenuItemImage::create("play (1).png", "play.png", CC_CALLBACK_1(MenuGameScene::goToHelloWorld, this));
    play->setPosition(origin.x + visibleSize.width / 2 , origin.y + visibleSize.height / 2);
    play->setScale(0.18);

    auto menu = Menu::create(play, NULL);
    menu->setPosition(Vec2(0, 0));
    this->addChild(menu);
    return true;
}

void MenuGameScene::goToHelloWorld(Ref* sender) {
    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("squeak_HgdJzB5.mp3");
    auto gameScene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(gameScene);
}





