#pragma once
#include <utility>
#include <type_traits>

template<typename T, typename... Args>
T CreateEntity(Args&&... args) {
    return T(std::forward<Args>(args)...);
}