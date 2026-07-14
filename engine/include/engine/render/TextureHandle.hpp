#pragma once

#include <cstddef>
#include <limits>

namespace engine {

struct TextureHandle {
    std::size_t index = std::numeric_limits<std::size_t>::max();

    bool isValid() const { return index != std::numeric_limits<std::size_t>::max(); }
    bool operator==(const TextureHandle& other) const { return index == other.index; }
};

}
