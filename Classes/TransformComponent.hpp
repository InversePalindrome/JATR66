/*
Copyright (c) 2018 Inverse Palindrome
Apophis - TransformComponent.hpp
InversePalindrome.com
*/


#pragma once

#include <Box2D/Common/b2Math.h>

#include <pugixml.hpp>


class TransformComponent
{
public:
	explicit TransformComponent(const pugi::xml_node& componentNode);
	
	b2Vec2 getPosition() const;
	void setPosition(const b2Vec2& position);

	float getAngle() const;
	void setAngle(float angle);

private:
	b2Vec2 position;
	float angle;
};