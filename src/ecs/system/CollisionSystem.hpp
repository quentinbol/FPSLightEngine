#pragma once
#include "../registry/Registry.hpp"
#include "../component/TransformComponent.hpp"
#include "../component/ColliderComponent.hpp"
#include <iostream>


class CollisionSystem {

    public:
        void update(Registry &registry) {
            auto entities = registry.getEntitiesWith<Transform, Collider>();

            Entity player = 0;
            bool hasPlayer = false;

            for (auto entity : registry.getEntitiesWith<Player>()) {
                player = entity;
                hasPlayer = true;
                break;
            }

            for (size_t i = 0; i < entities.size(); ++i) {
                Entity a = entities[i];
                Transform& transA = registry.getComponent<Transform>(a);
                Collider& colA = registry.getComponent<Collider>(a);

                for (size_t j = i + 1; j < entities.size(); ++j) {
                    Entity b = entities[j];
                    Transform& transB = registry.getComponent<Transform>(b);
                    Collider& colB = registry.getComponent<Collider>(b);

                    if (checkAABBCollision(transA, colA, transB, colB)) {
                        if (hasPlayer && (a == player || b == player)) {
                            (a == player ? 
                                resolveCollision(transA, colA, transB, colB) :
                                resolveCollision(transB, colB, transA, colA));
                        }
                    }
                }
            }
        }

    private:
        bool checkAABBCollision(const Transform &t1, const Collider &c1, const Transform &t2, const Collider &c2) {

            glm::vec3 aMin = t1.position - c1.halfExtents * t1.scale;
            glm::vec3 aMax = t1.position + c1.halfExtents * t1.scale;

            glm::vec3 bMin = t2.position - c2.halfExtents * t2.scale;
            glm::vec3 bMax = t2.position + c2.halfExtents * t2.scale;

            return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
                (aMin.y <= bMax.y && aMax.y >= bMin.y) &&
                (aMin.z <= bMax.z && aMax.z >= bMin.z);
        }

        void resolveCollision(Transform& player, const Collider& playerCol, const Transform& other, const Collider& otherCol) {

            glm::vec3 pMin = player.position - playerCol.halfExtents * player.scale;
            glm::vec3 pMax = player.position + playerCol.halfExtents * player.scale;

            glm::vec3 oMin = other.position - otherCol.halfExtents * other.scale;
            glm::vec3 oMax = other.position + otherCol.halfExtents * other.scale;

            float overlapX = std::min(pMax.x, oMax.x) - std::max(pMin.x, oMin.x);
            float overlapY = std::min(pMax.y, oMax.y) - std::max(pMin.y, oMin.y);
            float overlapZ = std::min(pMax.z, oMax.z) - std::max(pMin.z, oMin.z);

            if (overlapX < overlapY && overlapX < overlapZ) {
                player.position.x += (player.position.x < other.position.x ? -overlapX : overlapX);
            } else if (overlapY < overlapZ) {
                player.position.y += (player.position.y < other.position.y ? -overlapY : overlapY);
            } else {
                player.position.z += (player.position.z < other.position.z ? -overlapZ : overlapZ);
            }
        }
};
