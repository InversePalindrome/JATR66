/*
Copyright (c) 2018 Inverse Palindrome
JATR66 - BehaviorSystem.cpp
InversePalindrome.com
*/


#include "Tags.hpp"
#include "AreaQuery.hpp"
#include "FlockComponent.hpp"
#include "BehaviorSystem.hpp"
#include "SpeedComponent.hpp"
#include "AvoidComponent.hpp"
#include "QueueComponent.hpp"
#include "PhysicsUtility.hpp"
#include "ArriveComponent.hpp"
#include "WanderComponent.hpp"
#include "PatrolComponent.hpp"
#include "FollowComponent.hpp"
#include "PredictComponent.hpp"
#include "SteeringBehaviors.hpp"


void BehaviorSystem::configure(entityx::EventManager& eventManager)
{
	eventManager.subscribe<EntityCreated>(*this);
}

void BehaviorSystem::update(entityx::EntityManager& entityManager, entityx::EventManager& eventManager, entityx::TimeDelta deltaTime)
{ 
	entityx::ComponentHandle<AI> ai;

	for (auto entity : entityManager.entities_with_components(ai))
	{
		updateSeek(entity);
		updateFlee(entity);
		updatePursue(entity);
		updateEvade(entity);
		updateArrive(entity);
		updateWander(entity);
		updatePatrol(entity);
		updateAvoid(entity);
		updateAlign(entity);
		updateCohesion(entity);
		updateSeparate(entity);
		updateFollow(entity);
		updateQueue(entity);
		updateRotation(entity);
	}
}

void BehaviorSystem::receive(const EntityCreated& event)
{
	if (event.entity.has_component<Player>())
	{
		if (auto targetBody = event.entity.component<BodyComponent>())
		{
			this->targetBody = targetBody;
		}
	}
}

void BehaviorSystem::updateSeek(entityx::Entity entity)
{
	auto body = entity.component<BodyComponent>();
	auto speed = entity.component<SpeedComponent>();

	if(entity.has_component<SeekComponent>() && body && speed)
	{
		SteeringBehaviors::seek(body->getBody(), targetBody->getPosition(), speed->getMaxSpeed());
	}
}

void BehaviorSystem::updateFlee(entityx::Entity entity)
{
	auto body = entity.component<BodyComponent>();
	auto speed = entity.component<SpeedComponent>();

	if (entity.has_component<FleeComponent>() && body && speed)
	{
		SteeringBehaviors::seek(body->getBody(), targetBody->getPosition(), -speed->getMaxSpeed());
	}
}

void BehaviorSystem::updatePursue(entityx::Entity entity)
{
	auto pursue = entity.component<PredictComponent<Pursue>>();
	auto body = entity.component<BodyComponent>();
	auto speed = entity.component<SpeedComponent>();

	if(pursue && body && speed)
	{
		SteeringBehaviors::pursue(body->getBody(), targetBody->getPosition(), targetBody->getLinearVelocity(), speed->getMaxSpeed(), pursue->getPredictionTime());
	}
}

void BehaviorSystem::updateEvade(entityx::Entity entity)
{
	auto evade = entity.component<PredictComponent<Evade>>();
	auto body = entity.component<BodyComponent>();
	auto speed = entity.component<SpeedComponent>();

    if(evade && body && speed)
	{
		SteeringBehaviors::pursue(body->getBody(), targetBody->getPosition(), targetBody->getLinearVelocity(), -speed->getMaxSpeed(), evade->getPredictionTime());
	}
}

void BehaviorSystem::updateArrive(entityx::Entity entity)
{
	auto arrive = entity.component<ArriveComponent>();
	auto body = entity.component<BodyComponent>();
	auto speed = entity.component<SpeedComponent>();

	if(arrive && body && speed)
	{
		SteeringBehaviors::arrive(body->getBody(), targetBody->getPosition(), speed->getMaxSpeed(), arrive->getSlowingRadius());
	}
}

void BehaviorSystem::updateWander(entityx::Entity entity)
{
	auto wander = entity.component<WanderComponent>();
	auto body = entity.component<BodyComponent>();
	auto speed = entity.component<SpeedComponent>();
	     
	if(wander && body && speed)
	{
		SteeringBehaviors::wander(body->getBody(), wander->getWanderDistance(), wander->getWanderRadius(), wander->getWanderRate(), wander->getWanderAngle(), speed->getMaxSpeed());
	}
}

void BehaviorSystem::updatePatrol(entityx::Entity entity)
{
	auto patrol = entity.component<PatrolComponent>();
	auto body = entity.component<BodyComponent>();
	auto speed = entity.component<SpeedComponent>();

	if(patrol && body && speed)
	{
		SteeringBehaviors::seek(body->getBody(), patrol->getCurrentPoint(), speed->getMaxSpeed());

		if (body->contains(patrol->getCurrentPoint()))
		{
			patrol->nextPoint();
		}
	}
}

void BehaviorSystem::updateAvoid(entityx::Entity entity)
{
	auto avoid = entity.component<AvoidComponent>();
	auto body = entity.component<BodyComponent>();

	if(avoid && body)
	{
		SteeringBehaviors::avoid(body->getBody(), avoid->getAvoidanceDistance(), avoid->getAvoidanceForce());
	}
}

void BehaviorSystem::updateAlign(entityx::Entity entity)
{
	auto flock = entity.component<FlockComponent>();
	auto body = entity.component<BodyComponent>();
	auto speed = entity.component<SpeedComponent>();

	if (entity.has_component<AlignComponent>() && flock && body && speed)
	{
		SteeringBehaviors::align(body->getBody(), flock->getGroupRadius(), speed->getMaxSpeed());
	}
}

void BehaviorSystem::updateCohesion(entityx::Entity entity)
{
	auto flock = entity.component<FlockComponent>();
	auto body = entity.component<BodyComponent>();
	auto speed = entity.component<SpeedComponent>();

	if (entity.has_component<CohesionComponent>() && flock && body && speed)
	{
		SteeringBehaviors::cohesion(body->getBody(), flock->getGroupRadius(), speed->getMaxSpeed());
	}
}

void BehaviorSystem::updateSeparate(entityx::Entity entity)
{
	auto flock = entity.component<FlockComponent>();
	auto body = entity.component<BodyComponent>();
	auto speed = entity.component<SpeedComponent>();

	if (entity.has_component<SeparateComponent>() && flock && body && speed)
	{
		SteeringBehaviors::separate(body->getBody(), flock->getGroupRadius(), speed->getMaxSpeed());
	}
}

void BehaviorSystem::updateFollow(entityx::Entity entity)
{
	auto follow = entity.component<FollowComponent>();
	auto body = entity.component<BodyComponent>();
	auto speed = entity.component<SpeedComponent>();

	if(follow && body && speed)
	{
		SteeringBehaviors::follow(body->getBody(), targetBody->getPosition(), targetBody->getLinearVelocity(), follow->getDistanceFromLeader(), speed->getMaxSpeed());
	}
}

void BehaviorSystem::updateQueue(entityx::Entity entity)
{
	auto queue = entity.component<QueueComponent>();
	auto flock = entity.component<FlockComponent>();
	auto body = entity.component<BodyComponent>();
	auto speed = entity.component<SpeedComponent>();

	if (queue && flock && body && speed)
	{
		SteeringBehaviors::queue(body->getBody(), flock->getGroupRadius(), queue->getQueueDistance(), queue->getShrinkingFactor(), speed->getMaxSpeed());
	}
}

void BehaviorSystem::updateRotation(entityx::Entity entity)
{
	if(auto body = entity.component<BodyComponent>())
	{
		body->applyAngularImpulse(Utility::angularImpulseToAngle(body->getAngle(), Utility::vectorToRadian(body->getLinearVelocity()), body->getAngularVelocity(), body->getInertia()));
	}
}