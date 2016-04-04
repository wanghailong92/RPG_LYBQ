//
//  SLGCharacter.cpp
//  RPG_LYBQ
//
//  Created by mac on 16-3-29.
//
//

#include "SLGCharacter.h"
#include "Animation_itools.h"
#include "SLGMapView.h"
#include "PathManager.h"
#include <iostream>
float SLGCharacter::SpeedRatio=128.0;
SLGCharacter*SLGCharacter::create(const std::string filename,SLGMapView*map)
{
    SLGCharacter *pRet = new(std::nothrow) SLGCharacter();
    if (pRet && pRet->init(filename,map))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool SLGCharacter::init(const std::string filename,SLGMapView*map){
    if (!Sprite::init()) {
        return false;
    }
    this->_speed=50;
    this->_moveRange=4;
    this->map=map;
    this->imageSource=filename;
    loading_animation();
    this->walk(SLGCharacter::Direction::right);
    this->_AttakType=SLGCharacter::Attack::type1;
    this->_HurtType=SLGCharacter::hurt::type1;
    return true;
}

Vector<FiniteTimeAction*> SLGCharacter::initActioninterval(std::vector<Vec2>vpn,float speed){
    Vector<FiniteTimeAction*>acv;
    //vpn.push_back(Vec2(this->getPositionX(),this->getPositionY()));
    std::cout<<vpn.size()<<std::endl;
    for(auto pi:vpn){
        std::cout<<"x:"<<pi.x<<"Y:"<<pi.y<<std::endl;
    }
    for(int i = vpn.size()-1;i>0;i--)
    {
        auto v1=vpn[i];
        auto v2=vpn[i-1];
        acv.pushBack(CallFuncN::create([=](Node* pSender){
            SLGCharacter*ms=static_cast<SLGCharacter*>(pSender);
            ms->change_animation(v1,v2);
            
        }));
        FiniteTimeAction*move=MoveTo::create(speed/SLGCharacter::SpeedRatio, v2);
        acv.pushBack(move);
        std::cout<<"MoveTo:x:"<<v2.x<<"Y:"<<v2.y<<std::endl;
     
        }

    return acv;
}
void SLGCharacter::change_animation(Vec2 v1,Vec2 v2){
    
    float absx=abs((float)(v2.x-v1.x));
    float absy=abs((float)(v2.y-v1.y));
    SLGCharacter::Direction temp=_direction;
    SLGCharacter::Direction temp2;
    if (absx>=absy&&v2.x>=v1.x) {
        temp2=SLGCharacter::Direction::right;
    }
    //左
    else if (absx>=absy&&v2.x<v1.x) {
        temp2=SLGCharacter::Direction::left;
        //CCLOG("left");
    }
    //上
    
    else if (absx<absy&&v2.y>=v1.y) {
        
        temp2=SLGCharacter::Direction::up;
        
        // CCLOG("up");
    }
    //下
    else if (absx<absy&&v2.y<v1.y) {
        temp2=SLGCharacter::Direction::down;
        //CCLOG("down");
    }
    if (temp2==temp) {
        return;
    }
    
    stopAction(animate);
    //CCLOG("in");
    if (absx>=absy&&v2.x>=v1.x) {
        _direction=SLGCharacter::Direction::right;
      
        // CCLOG("right");
    }
    //左
    else if (absx>=absy&&v2.x<v1.x) {
        _direction=SLGCharacter::Direction::left;
        //CCLOG("left");
    }
    //上
    
    else if (absx<absy&&v2.y>=v1.y) {
        _direction=SLGCharacter::Direction::up;
        // CCLOG("up");
    }
    //下
    else if (absx<absy&&v2.y<v1.y) {
        _direction=SLGCharacter::Direction::down;
        //CCLOG("down");
    }
      walk(_direction);
}


void SLGCharacter::goDestnation(std::vector<Vec2>p){
    auto vecOfAction=initActioninterval(p,_speed);
    this->runAction(Sequence::create(vecOfAction));

}
void SLGCharacter::drawMovieRange(){
    CCASSERT(_pathManager,"在使用前必须指定pathManager，通过setPathManager设定");
    std::vector<Vec2> temp=getAttackRange(_AttakType);
    auto gid = Gird::create(_tilex, _tiley);
    _pathManager->findRange(gid,_moveRange,temp);
}
void SLGCharacter::setPathManager(PathManager*p){
    p->retain();
    this->_pathManager=p;
}
void SLGCharacter::setTilePosition(int x,int y,bool goPos){
    this->_tilex=x;
    this->_tiley=y;
    if(goPos){
    Vec2 pos = this->map->tileCoordToPosition(Vec2(x,y))+Vec2(this->map->_tileMap->getTileSize()/2);
    this->setPosition(pos);
      }
}
Vec2 SLGCharacter::getTilePosition(int x,int y){
    return this->map->tileCoordToPosition(Vec2(x,y))+Vec2(this->map->_tileMap->getTileSize()/2);
}

void SLGCharacter::loading_animation(){
    AnimationCache* ac=AnimationCache::getInstance();
    Animation*rundown=itool->makeAnimationfrommixpicture(imageSource+"_move.png",4,4,1,1,4,1,0.5f,true,-1);
    ac->addAnimation(rundown,imageSource+"godown");
    Animation*goleft=itool->makeAnimationfrommixpicture(imageSource+"_move.png",4,4,1,2,4,2,0.5f,true,-1);
    ac->addAnimation(goleft,imageSource+"goleft");
    Animation*goright=itool->makeAnimationfrommixpicture(imageSource+"_move.png",4,4,1,3,4,3,0.5f,true,-1);
    ac->addAnimation(goright,imageSource+"goright");
    Animation*goup=itool->makeAnimationfrommixpicture(imageSource+"_move.png",4,4,1,4,4,4,0.5f,true,-1);
    ac->addAnimation(goup,imageSource+"goup");
    
    Animation*attackdown=itool->makeAnimationfrommixpicture(imageSource+"_attack.png",4,4,1,1,4,1,0.2f,true,1);
    ac->addAnimation(attackdown,imageSource+"Adown");
    Animation*attackleft=itool->makeAnimationfrommixpicture(imageSource+"_attack.png",4,4,1,2,4,2,0.2f,true,1);
    ac->addAnimation(attackleft,imageSource+"Aleft");
    Animation*attackright=itool->makeAnimationfrommixpicture(imageSource+"_attack.png",4,4,1,3,4,3,0.2f,true,1);
    ac->addAnimation(attackright,imageSource+"Aright");
    Animation*attackup=itool->makeAnimationfrommixpicture(imageSource+"_attack.png",4,4,1,4,4,4,0.2f,true,1);
    ac->addAnimation(attackup,imageSource+"Aup");
    
    Animation*diedown=itool->makeAnimationfrommixpicture(imageSource+"_die.png",3,4,1,1,3,1,0.2f,true,1);
    ac->addAnimation(diedown,imageSource+"diedown");
    Animation*dieft=itool->makeAnimationfrommixpicture(imageSource+"_die.png",3,4,1,2,3,2,0.2f,true,1);
    ac->addAnimation(dieft,imageSource+"dieleft");
    Animation*dieright=itool->makeAnimationfrommixpicture(imageSource+"_die.png",3,4,1,3,3,3,0.2f,true,1);
    ac->addAnimation(dieright,imageSource+"dieright");
    Animation*dieup=itool->makeAnimationfrommixpicture(imageSource+"_die.png",3,4,1,4,3,4,0.2f,true,1);
    ac->addAnimation(dieup,imageSource+"dieup");
}
std::string SLGCharacter::enumToString(SLGCharacter::Direction direction){
    switch (direction) {
        case SLGCharacter::Direction::left:
            return "left";
         
        case SLGCharacter::Direction::right:
            return "right";
          
        case SLGCharacter::Direction::up:
            return "up";
      
        case SLGCharacter::Direction::down:
            return "down";
       
    }
}

void SLGCharacter::walk(SLGCharacter::Direction direction){
    AnimationCache* ac=AnimationCache::getInstance();
    std::string godirection =imageSource+"go"+enumToString(direction);
    Animation *a=ac->getAnimation(godirection);
    Animate* animate=Animate::create(a);
    this->animate=animate;
    this->runAction(animate);
}
void SLGCharacter::attack(SLGCharacter::Direction direction){
    AnimationCache* ac=AnimationCache::getInstance();
    std::string Adirection =imageSource+"A"+enumToString(direction);
    Animation *a=ac->getAnimation(Adirection);
    Animate* animate=Animate::create(a);
    this->animate=animate;
    this->runAction(animate);
}
void SLGCharacter::dieAnimation(SLGCharacter::Direction direction){
    AnimationCache* ac=AnimationCache::getInstance();
    std::string diedirection =imageSource+"die"+enumToString(direction);
    Animation *a=ac->getAnimation(diedirection);
    Animate* animate=Animate::create(a);
    this->animate=animate;
    this->runAction(animate);
}

std::vector<Vec2> SLGCharacter::gethurtRange(SLGCharacter::hurt type){
    std::vector<Vec2> temp;
    switch (type) {
        case hurt::type1:
        {
            temp.push_back(Vec2(0,0));
            break;
        }
        case hurt::type2:
        {
            temp.push_back(Vec2(1,0));
            temp.push_back(Vec2(0,-1));
            temp.push_back(Vec2(-1,0));
            temp.push_back(Vec2(0,1));
            break;
        }
        default:
            break;
    }
    return temp;
    
}


std::vector<Vec2> SLGCharacter::getAttackRange(SLGCharacter::Attack type){
    std::vector<Vec2> temp;
    switch (type) {
        case Attack::type1:
        {
            temp.push_back(Vec2(1,0));
            temp.push_back(Vec2(0,-1));
            temp.push_back(Vec2(-1,0));
            temp.push_back(Vec2(0,1));
            break;
        }
        case Attack::type2:
        {
            temp.push_back(Vec2(1,0));
            temp.push_back(Vec2(1,-1));
            temp.push_back(Vec2(0,-1));
            temp.push_back(Vec2(-1,-1));
            temp.push_back(Vec2(-1,0));
            temp.push_back(Vec2(-1,1));
            temp.push_back(Vec2(0,1));
            temp.push_back(Vec2(1,1));
            break;
        }
        case Attack::type3:
        {
            temp.push_back(Vec2(-2,0));
            temp.push_back(Vec2(-1,-1));
            temp.push_back(Vec2(0,-2));
            temp.push_back(Vec2(1,-1));
            temp.push_back(Vec2(2,0));
            temp.push_back(Vec2(1,1));
            temp.push_back(Vec2(0,2));
            temp.push_back(Vec2(-1,1));
            break;
        }
            
            
        default:
            break;
    }
    return temp;
}

SLGCharacter::Attack SLGCharacter::AttakintToenum(int type){
    switch (type) {
        case 1:
            return SLGCharacter::Attack::type1;
        case 2:
            return SLGCharacter::Attack::type2;
        case 3:
            return SLGCharacter::Attack::type3;
    }
}

SLGCharacter::hurt SLGCharacter::HurtintToenum(int type){
    switch (type) {
        case 1:
            return SLGCharacter::hurt::type1;
        case 2:
            return SLGCharacter::hurt::type2;
       
    }

}


SLGCharacter::~SLGCharacter(){
    _pathManager->release();
}