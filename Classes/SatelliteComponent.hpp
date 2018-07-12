/*
Copyright (c) 2018 Inverse Palindrome
Apophis - SatelliteComponent.hpp
InversePalindrome.com
*/


#pragma once

#include <pugixml.hpp>


class SatelliteComponent
{
public:
	explicit SatelliteComponent(const pugi::xml_node& componentNode);

	int getPrimaryID() const;
	void setPrimaryID(int primaryID);

	std::int8_t getOrbitDirection() const;
	void setOrbitDirection(std::int8_t orbitDirection);

private:
	int primaryID;
	std::int8_t orbitDirection;
};