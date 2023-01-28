#ifndef ENTITY_H
#define ENTITY_H

#include <set>
#include <string>
#include <algorithm>

namespace gamelib
{

  /*

  Entity
    - every object in the game is an Entity
    - every Entity has a global world position with double precision
    - every Entity has a velocity in pixels per second with double precision
    - every Entity has a unique id which is an unsigned long value
    - every Entity has a set of strings which serve as "tags" for identification/grouping
    - every Entity has a boolean flag to determine if the entity is active
    - every Entity has a boolean flag to determine if the entity is visible

  */

  // ENTITY CLASS
  class Entity
  {
  private:
    // each time an entity is constructed, the number is incremented
    static unsigned long nextEntityId;

  protected:
    double worldPositionX;
    double worldPositionY;
    double velocityX;
    double velocityY;
    unsigned long id;
    bool active;
    bool visible;
    std::set<std::string> tags;

  public:
    // default constructor - entity will be created at world origin 0, 0 with no velocity
    Entity();

    // specialized constructor - entity will be created at given world position with no velocity
    Entity(double x, double y);

    // specialized constructor - entity will be created at given world position and velocity
    Entity(double x, double y, double xv, double yv);

    // specialized constructor - entity will be created at world origin 0, 0 with no velocity with given tags
    Entity(const char *withTags[]);

    // specialized constructor - entity will be created at given world position with no velocity with given tags
    Entity(double x, double y, const char *withTags[]);

    // specialized constructor - entity will be created at given world position and velocity with given tags
    Entity(double x, double y, double xv, double yv, const char *withTags[]);

    // copy constructor
    Entity(const Entity &other);

    // move constructor
    Entity(Entity &&other);

    // assignment operator
    Entity &operator=(const Entity &other);

    // move-assignment operator
    Entity &operator=(Entity &&other);

    // equivalence operator - is true when the other entity.id matches this entity's id.
    bool operator==(const Entity &other) const;

    // less-than operator - is true when this entity's world position is < the other entity's world position
    bool operator<(const Entity &other) const;

    // greater-than operator - is true when this entity's world position is > the other entity's world position
    bool operator>(const Entity &other) const;

    // add a tag
    void setTag(const std::string &tag);

    // remove a tag
    void untag(const std::string &tag);

    // check for a tag
    bool hasTag(const std::string &tag) const;

    // check if entity should be updated
    bool isActive() const;

    // check if entity should be rendered
    bool isVisible() const;

    // set visibility to true
    void show();

    // set visibility to false
    void hide();

    // set active to true
    void enable();

    // set active to false
    void disable();

    // simple linear integration of velocity
    void applyVelocity(double deltaTime);

    // sets velocity to zero
    void cancelVelocity();

    void setWorldPositionX(double value) { worldPositionX = value; }
    void setWorldPositionY(double value) { worldPositionY = value; }
    void setVelocityX(double value) { velocityX = value; }
    void setVelocityY(double value) { velocityY = value; }

    double getWorldPositionX() const { return worldPositionX; }
    double getWorldPositionY() const { return worldPositionY; }
    double getVelocityX() const { return velocityX; }
    double getVelocityY() const { return velocityY; }
  };
}

#endif
