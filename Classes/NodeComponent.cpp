/*
Copyright (c) 2018 Inverse Palindrome
JATR66 - NodeComponent.cpp
InversePalindrome.com
*/


#include "NodeComponent.hpp"


NodeComponent::NodeComponent(cocos2d::Node* node) :
	node(node)
{
}

cocos2d::Node* NodeComponent::getNode()
{
	return node;
}

cocos2d::Node* NodeComponent::getNode() const
{
	return node;
}

void NodeComponent::addChild(cocos2d::Node* node)
{
	node->addChild(node);
}

void NodeComponent::removeFromParent()
{
	node->removeFromParent();
}

cocos2d::Vec2 NodeComponent::getPosition() const
{
	return node->getPosition();
}

void NodeComponent::setPosition(const cocos2d::Vec2& position)
{
	node->setPosition(position);
}

float NodeComponent::getRotation() const
{
	return node->getRotation();
}

void NodeComponent::setRotation(float rotation)
{
	node->setRotation(rotation);
}