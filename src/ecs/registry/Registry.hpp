#pragma once

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <typeindex>
#include <memory>
#include <stdexcept>
#include <algorithm>

using Entity = uint32_t;

class Registry {
public:
    Entity createEntity() {
        Entity id = nextEntity++;
        entities.push_back(id);
        return id;
    }

    void destroyEntity(Entity e) {
        entities.erase(std::remove(entities.begin(), entities.end(), e), entities.end());
        for (auto& [type, pool] : componentPools) {
            pool->remove(e);
        }
    }

    template<typename T>
    void addComponent(Entity e, const T &component) {
        getOrCreatePool<T>()->insert(e, component);
    }

    template<typename T>
    bool hasComponent(Entity e) {
        auto pool = getPool<T>();
        return pool && pool->has(e);
    }

    template<typename T>
    T &getComponent(Entity e) {
        auto pool = getPool<T>();
        if (!pool) throw std::runtime_error("Component not found");
        return pool->get(e);
    }

    template<typename ...Component>
    std::vector<Entity> getEntitiesWith() {
        std::vector<Entity> result;
        for (auto e : entities) {
            if ((hasComponent<Component>(e) && ...)) {
                result.push_back(e);
            }
        }
        return result;
    }

    const std::vector<Entity> &getEntities() const { return entities; }

private:
    struct IComponentPool {
        virtual ~IComponentPool() = default;
        virtual void remove(Entity e) = 0;
    };

    template<typename T>
    struct ComponentPool : IComponentPool {
        std::unordered_map<Entity, T> components;
        void insert(Entity e, const T &c) { components[e] = c; }
        bool has(Entity e) { return components.count(e) > 0; }
        T &get(Entity e) { return components.at(e); }
        void remove(Entity e) override { components.erase(e); }
    };

    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> componentPools;
    std::vector<Entity> entities;
    Entity nextEntity = 0;

    template<typename T>
    ComponentPool<T> *getPool() {
        auto it = componentPools.find(std::type_index(typeid(T)));
        if (it == componentPools.end()) return nullptr;
        return static_cast<ComponentPool<T>*>(it->second.get());
    }

    template<typename T>
    ComponentPool<T> *getOrCreatePool() {
        auto pool = getPool<T>();
        if (!pool) {
            pool = new ComponentPool<T>();
            componentPools[std::type_index(typeid(T))] = std::unique_ptr<IComponentPool>(pool);
        }
        return pool;
    }
};
