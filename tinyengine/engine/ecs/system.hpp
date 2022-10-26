#pragma once

namespace engine {

class World;
class Entity;

class System {
public:
    enum Type {
        UpdateEachEntity = 1,
        UpdateOncePerFrame,
    };

    System(World* world): world_(world) {}
    System(const System&) = delete;
    System& operator=(const System&) = delete;

    virtual ~System() = default;

    World* World() const { return world_; }

    virtual Type GetType() = 0;

private:
    class World* world_;
};

class PerFrameSystem: public System {
public:
    static Type GetClassType() { return UpdateOncePerFrame; }

    PerFrameSystem(class World* world): System(world) {}
    virtual void Update() = 0;

    Type GetType() override { return PerFrameSystem::GetClassType(); }
};

class EntityUpdateSystem: public System {
public:
    static Type GetClassType() { return UpdateEachEntity; }

    EntityUpdateSystem(class World* world): System(world) {}
    virtual void Update(Entity*) = 0;
    
    Type GetType() override { return EntityUpdateSystem::GetClassType(); }
};

}