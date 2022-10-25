#include "engine/ecs/world.hpp"
#include "engine/ecs/component.hpp"
#include "engine/ecs/entity.hpp"
#include "engine/core/scene.hpp"
#include "engine/ui/ui_system.hpp"
#include "engine/components/node.hpp"
#include "engine/core/init_config.hpp"

namespace engine {

std::unique_ptr<World> World::instance_;

World::World() {
}

World::~World() {
}

World* World::Instance() {
    if (!instance_) {
        instance_ = std::make_unique<World>();
    }
    return instance_.get();
}

void World::Init() {
}

void World::Quit() {
    instance_.reset();
}

Entity* World::CreateEntity(const std::string& name) {
    if (entityTrashes_.empty()) {
        entities_.push_back(std::make_unique<Entity>(this, entityID_++, name));
        return entities_.back().get();
    } else {
        std::unique_ptr<Entity> entity = std::move(entityTrashes_.top());
        entity->components_.clear();
        entity->id_ = entityID_++;
        entity->name_ = name;
        entity->shouldBeCleanUp_ = false;
        entities_.push_back(std::move(entity));
        entityTrashes_.pop();
        return entities_.back().get();
    }
}

void World::DestroyEntity(EntityID id) {
    auto it = entities_.begin();
    while (it != entities_.end() && (*it)->ID() != id) {
        it++;
    }
    if (it != entities_.end()) {
        destroyEntity(it);
    }
}

void World::DestroyEntity(Entity* entity) {
    if (!entity) return;

    auto it = entities_.begin();
    while (it != entities_.end() && (*it)->ID() != entity->ID()) {
        it++;
    }
    if (it != entities_.end()) {
        destroyEntity(it);
    }
}

void World::destroyEntity(const std::vector<std::unique_ptr<Entity>>::const_iterator& it) {
    (*it)->shouldBeCleanUp_ = true;
}

void World::RemoveComponent(Component* component) {
    if(!component) return;

    for (auto& [id, info] : components_) {
        auto it = info.components.begin(); 
        while (it != info.components.end() && (*it)->ID() != component->ID()) {
            it++;
        }
        if (it != info.components.end()) {
            info.componentTrashes_.push(std::move(*it));
            info.componentTrashes_.top()->Reset();
            info.components.erase(it);
        }
    }
}

void World::TryInitEntities() {
    auto scene = SceneMgr::CurrentScene();
    if (!scene) return;
    auto node = scene->GetRootEntity()->GetComponent<NodeComponent>();

    for (auto& entity : node->children) {
        initEntity(entity);
    }
}

void World::initEntity(Entity* entity) {
    if (auto behavior = entity->GetBehavior(); behavior != nullptr) {
        if (!behavior->IsInited()) {
            behavior->OnInit();
            behavior->Inited();
        }
    }

    if (auto node = entity->GetComponent<NodeComponent>(); node != nullptr) {
        for (auto& ent : node->children) {
            initEntity(ent);
        }
    }
    if (auto node = entity->GetComponent<Node2DComponent>(); node != nullptr) {
        for (auto& ent : node->children) {
            initEntity(ent);
        }
    }
}

void World::Update() {
    auto scene = SceneMgr::CurrentScene();
    if (!scene) return;

    callEntityScripts(scene->GetRootEntity());
    updateSystems(scene->GetRootEntity());
    dispatchEvent2Entity(scene->GetRootEntity());
}

void World::updateSystems(Entity* entity) {
    for (auto& system : systems_) {
        walkThroughNodeTree(entity, [&](Entity* entity){
            if (system->GetType() == System::Type::UpdateEachEntity) {
                ((EntityUpdateSystem*)system.get())->Update(entity);
            } 
        });
        if (system->GetType() == System::Type::UpdateOncePerFrame) {
            ((PerFrameSystem*)system.get())->Update();
        }
    }
}

void World::callEntityScripts(Entity* entity) {
    walkThroughNodeTree(entity, [&](Entity* entity){
        if (auto behavior = entity->GetBehavior(); behavior) {
            behavior->OnUpdate();
        }
    });
}

void World::dispatchEvent2Entity(Entity* entity) {
    walkThroughNodeTree(entity, [](Entity* entity){
        if (entity->GetBehavior()) {
            Event::GetDispatcher().Dispatch(entity->GetBehavior());
        }
    });
}

void World::walkThroughNodeTree(engine::Entity* entity, std::function<void(Entity*)> func) {
    if (!entity || !entity->IsActive()) return;

    if (func) {
        func(entity);
    }

    if (auto node = entity->GetComponent<NodeComponent>(); node != nullptr) {
        for (auto& ent : node->children) {
            walkThroughNodeTree(ent, func);
        }
    }
    if (auto node = entity->GetComponent<Node2DComponent>(); node != nullptr) {
        for (auto& ent : node->children) {
            walkThroughNodeTree(ent, func);
        }
    }
}


void World::CleanUp() {
    size_t idx = 0;
    while (idx < entities_.size()) {
        if (entities_[idx]->shouldBeCleanUp_) {
            for (auto& component : entities_[idx]->components_) {
                RemoveComponent(component.second);
            }
            entities_[idx]->components_.clear();
            if (entities_[idx]->behavior_) {
                entities_[idx]->behavior_->OnQuit();
            }
            entities_.erase(entities_.begin() + idx);
        } else {
            idx++;
        }
    }
}

void World::RemoveSystem(System* system) {
    if (!system) return;

    for (auto it = systems_.begin(); it != systems_.end(); it++) {
        if (it->get() == system) {
            systems_.erase(it);
            break;
        }
    }
}

void World::Shutdown() {
    for (auto it = entities_.begin(); it != entities_.end(); it++) {
        destroyEntity(it);
    }
    CleanUp();
}

}
