#include "entity.h"

// within this file we want to declare that we can see within the namespace of the class
using namespace gamelib;

// initialize the static members of the class
unsigned long Entity::nextEntityId = 0;

// default constructor - entity will be created at world origin 0, 0 with no velocity
Entity::Entity() : worldPositionX(0),
                   worldPositionY(0),
                   velocityX(0),
                   velocityY(0),
                   id(Entity::nextEntityId++),
                   active(true),
                   visible(true),
                   tags() {}

// specialized constructor - entity will be created at given world position with no velocity
Entity::Entity(double x, double y) : worldPositionX(x),
                                     worldPositionY(y),
                                     velocityX(0),
                                     velocityY(0),
                                     id(Entity::nextEntityId++),
                                     active(true),
                                     visible(true),
                                     tags() {}

// specialized constructor - entity will be created at given world position and velocity
Entity::Entity(double x, double y, double xv, double yv) : worldPositionX(x),
                                                           worldPositionY(y),
                                                           velocityX(xv),
                                                           velocityY(yv),
                                                           id(Entity::nextEntityId++),
                                                           active(true),
                                                           visible(true),
                                                           tags() {}

// specialized constructor - entity will be created at world origin 0, 0 with no velocity with given tags
Entity::Entity(const char *withTags[]) : worldPositionX(0),
                                         worldPositionY(0),
                                         velocityX(0),
                                         velocityY(0),
                                         id(Entity::nextEntityId++),
                                         active(true),
                                         visible(true),
                                         tags()
{
  for (int i = 0; withTags[i] != nullptr; ++i)
  {
    tags.insert(withTags[i]);
  }
}

// specialized constructor - entity will be created at given world position with no velocity with given tags
Entity::Entity(double x, double y, const char *withTags[]) : worldPositionX(x),
                                                             worldPositionY(y),
                                                             velocityX(0),
                                                             velocityY(0),
                                                             id(Entity::nextEntityId++),
                                                             active(true),
                                                             visible(true),
                                                             tags()
{
  for (int i = 0; withTags[i] != nullptr; ++i)
  {
    tags.insert(withTags[i]);
  }
}

// specialized constructor - entity will be created at given world position and velocity with given tags
Entity::Entity(double x, double y, double xv, double yv, const char *withTags[]) : worldPositionX(x),
                                                                                   worldPositionY(y),
                                                                                   velocityX(xv),
                                                                                   velocityY(yv),
                                                                                   id(Entity::nextEntityId++),
                                                                                   active(true),
                                                                                   visible(true),
                                                                                   tags()
{
  for (int i = 0; withTags[i] != nullptr; ++i)
  {
    tags.insert(withTags[i]);
  }
}

// copy constructor
Entity::Entity(const Entity &other) : worldPositionX(other.worldPositionX),
                                      worldPositionY(other.worldPositionY),
                                      velocityX(other.velocityX),
                                      velocityY(other.velocityY),
                                      id(Entity::nextEntityId++),
                                      active(other.active),
                                      visible(other.visible),
                                      tags()
{
  for (auto &tag : other.tags)
  {
    tags.insert(tag);
  }
}

// move constructor
Entity::Entity(Entity &&other) : worldPositionX(std::move(other.worldPositionX)),
                                 worldPositionY(std::move(other.worldPositionY)),
                                 velocityX(std::move(other.velocityX)),
                                 velocityY(std::move(other.velocityY)),
                                 id(Entity::nextEntityId++),
                                 active(std::move(other.active)),
                                 visible(std::move(other.visible)),
                                 tags(std::move(other.tags))
{
}

// assignment operator
Entity
    &
    Entity::operator=(const Entity &other)
{
  worldPositionX = other.worldPositionX;
  worldPositionY = other.worldPositionY;
  velocityX = other.velocityX;
  velocityY = other.velocityY;
  active = other.active;
  visible = other.visible;
  for (auto &tag : other.tags)
  {
    tags.insert(tag);
  }
  return *this;
}

// move-assignment operator
Entity &Entity::operator=(Entity &&other)
{
  worldPositionX = std::move(other.worldPositionX);
  worldPositionY = std::move(other.worldPositionY);
  velocityX = std::move(other.velocityX);
  velocityY = std::move(other.velocityY);
  active = std::move(other.active);
  visible = std::move(other.visible);
  tags.clear();
  tags = std::move(other.tags);
  return *this;
}

// equivalence operator - is true when the other entity.id matches this entity's id.
bool Entity::operator==(const Entity &other) const
{
  return id == other.id;
}

// less-than operator - is true when this entity's world position is < the other entity's world position
bool Entity::operator<(const Entity &other) const
{
  return worldPositionX < other.worldPositionX && worldPositionY < other.worldPositionY;
}

// greater-than operator - is true when this entity's world position is > the other entity's world position
bool Entity::operator>(const Entity &other) const
{
  return worldPositionX > other.worldPositionX && worldPositionY > other.worldPositionY;
}

// add a tag
void Entity::setTag(const std::string &tag)
{
  tags.emplace(tag);
}

// remove a tag
void Entity::untag(const std::string &tag)
{
  tags.erase(tag);
}

// check for a tag
bool Entity::hasTag(const std::string &tag) const
{
  return tags.count(tag) != 0;
}

// check if entity should be updated
bool Entity::isActive() const
{
  return active;
}

// check if entity should be rendered
bool Entity::isVisible() const
{
  return visible;
}

// set visibility to true
void Entity::show()
{
  visible = true;
}

// set visibility to false
void Entity::hide()
{
  visible = false;
}

// set active to true
void Entity::enable()
{
  active = true;
}

// set active to false
void Entity::disable()
{
  active = false;
}

// simple linear integration of velocity
void Entity::applyVelocity(double deltaTime)
{
  // if the entity is not active, it should not move
  if (active)
  {
    // the entity will move at a rate of velocityX pixels per second along the X axis (horizontal)
    worldPositionX += velocityX * deltaTime;

    // the entity will move at a rate of velocityY pixels per second along the Y axis (vertical)
    worldPositionY += velocityY * deltaTime;
  }
}

// sets velocity to zero
void Entity::cancelVelocity()
{
  // this causes the entity to stop moving
  velocityX = 0.0f;
  velocityY = 0.0f;
}
