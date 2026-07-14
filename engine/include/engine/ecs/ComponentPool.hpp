#pragma once

#include <cassert>
#include <cstddef>
#include <limits>
#include <span>
#include <vector>

#include "engine/ecs/Entity.hpp"

namespace engine {

template <typename T>
class ComponentPool {
public:
    T& emplace(Entity entity, T component) {
        ensureSparseCapacity(entity.index);

        std::size_t& slot = sparse_[entity.index];
        if (slot != kInvalidSlot && denseEntities_[slot] == entity) {
            dense_[slot] = std::move(component);
            return dense_[slot];
        }

        slot = dense_.size();
        dense_.push_back(std::move(component));
        denseEntities_.push_back(entity);
        return dense_.back();
    }

    void remove(Entity entity) {
        if (!has(entity)) {
            return;
        }
        std::size_t slot = sparse_[entity.index];
        std::size_t lastSlot = dense_.size() - 1;

        dense_[slot] = std::move(dense_[lastSlot]);
        denseEntities_[slot] = denseEntities_[lastSlot];
        sparse_[denseEntities_[slot].index] = slot;

        dense_.pop_back();
        denseEntities_.pop_back();
        sparse_[entity.index] = kInvalidSlot;
    }

    bool has(Entity entity) const {
        if (entity.index >= sparse_.size()) {
            return false;
        }
        std::size_t slot = sparse_[entity.index];
        return slot != kInvalidSlot && denseEntities_[slot] == entity;
    }

    T& get(Entity entity) {
        assert(has(entity) && "ComponentPool::get on entity without this component");
        return dense_[sparse_[entity.index]];
    }

    const T& get(Entity entity) const {
        assert(has(entity) && "ComponentPool::get on entity without this component");
        return dense_[sparse_[entity.index]];
    }

    std::span<Entity> entities() { return denseEntities_; }
    std::span<T> components() { return dense_; }
    std::size_t size() const { return dense_.size(); }

private:
    static constexpr std::size_t kInvalidSlot = std::numeric_limits<std::size_t>::max();

    void ensureSparseCapacity(std::uint32_t index) {
        if (index >= sparse_.size()) {
            sparse_.resize(index + 1, kInvalidSlot);
        }
    }

    std::vector<T> dense_;
    std::vector<Entity> denseEntities_;
    std::vector<std::size_t> sparse_;
};

}
