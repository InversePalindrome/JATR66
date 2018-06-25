/*
Copyright (c) 2018 Inverse Palindrome
Apophis  - PlayerSystem.cpp
InversePalindrome.com
*/


#pragma once

#include "Tags.hpp"
#include "Events.hpp"
#include "Constants.hpp"
#include "AppSettings.hpp"
#include "PlayerSystem.hpp"
#include "SteeringBehaviors.hpp"

#include <cocos/base/CCEventDispatcher.h>
#include <cocos/base/CCEventListenerMouse.h>
#include <cocos/base/CCEventListenerKeyboard.h>


PlayerSystem::PlayerSystem(cocos2d::Node* gameNode) :
	gameNode(gameNode),
	isShooting(false)
{
	const auto& appSettings = AppSettings::getInstance();

	auto* keyboardListener = cocos2d::EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = [this, &appSettings](auto keyCode, auto* event) 
	{ 
		if (appSettings.hasKeyAction(keyCode))
		{
			keyActions.push_back(appSettings.getKeyAction(keyCode));
		}
	};
	keyboardListener->onKeyReleased = [this, &appSettings](auto keyCode, auto* event) 
	{
		if (appSettings.hasKeyAction(keyCode))
		{
			keyActions.erase(std::remove(std::begin(keyActions), std::end(keyActions), appSettings.getKeyAction(keyCode)), std::end(keyActions));
		}
	};

	auto* mouseListener = cocos2d::EventListenerMouse::create();
	mouseListener->onMouseMove = [this, gameNode](auto* event) { playerFocusPoint = gameNode->convertToNodeSpace({ event->getCursorX(), event->getCursorY() }); };
	mouseListener->onMouseDown = [this](auto* event) { isShooting = true; };
	mouseListener->onMouseUp = [this](auto* event) { isShooting = false; };

	gameNode->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, gameNode);
	gameNode->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, gameNode);
}

void PlayerSystem::update(entityx::EntityManager& entityManager, entityx::EventManager& eventManager, entityx::TimeDelta deltaTime)
{
	entityx::ComponentHandle<Player> player;
	entityx::ComponentHandle<BodyComponent> body;
	entityx::ComponentHandle<SpeedComponent> speed;
	entityx::ComponentHandle<ImpulseComponent> impulse;

	for (auto entity : entityManager.entities_with_components(player, body, speed, impulse))
	{
		updateMovement(speed, impulse);
		updateRotation(body, impulse);
		updateShooting(eventManager, entity);
	}
}

void PlayerSystem::updateMovement(entityx::ComponentHandle<SpeedComponent> speed, entityx::ComponentHandle<ImpulseComponent> impulse)
{
	for (auto keyAction : keyActions)
	{
		switch (keyAction)
		{
		case KeyAction::MoveRight:
			impulse += speed->getMaxLinearSpeed() * b2Vec2(1.f, 0.f);
			break;
		case KeyAction::MoveLeft:
			impulse += speed->getMaxLinearSpeed() * b2Vec2(-1.f, 0.f);
			break;
		case KeyAction::MoveUp:
			impulse += speed->getMaxLinearSpeed() * b2Vec2(0.f, 1.f);
			break;
		case KeyAction::MoveDown:
			impulse += speed->getMaxLinearSpeed() * b2Vec2(0.f, -1.f);
			break;
		}
	}
}

void PlayerSystem::updateRotation(entityx::ComponentHandle<BodyComponent> body, entityx::ComponentHandle<ImpulseComponent> impulse)
{
	impulse += SteeringBehaviors::face(body->getPosition(), { playerFocusPoint.x / Constants::PTM_RATIO, playerFocusPoint.y / Constants::PTM_RATIO }, body->getAngle(), body->getAngularVelocity(), body->getInertia());
}

void PlayerSystem::updateShooting(entityx::EventManager& eventManager, entityx::Entity entity)
{
	if (isShooting)
	{
		eventManager.emit(ShootProjectile{ entity });
	}
}