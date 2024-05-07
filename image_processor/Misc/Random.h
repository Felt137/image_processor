#ifndef CPP_HSE_RANDOM_H
#define CPP_HSE_RANDOM_H

#include <random>

namespace my_random {

struct PRNG {
    PRNG() {
        std::random_device device;
        engine.seed(device());
    }
    std::mt19937 engine;
};

template <typename T>
requires(std::is_floating_point_v<T>) T RandomInRange(T l, T r) {
    static PRNG rng;
    std::uniform_real_distribution<T> distribution(l, r);
    return distribution(rng.engine);
}

template <typename T>
requires(std::is_integral_v<T>) T RandomInRange(T l, T r) {
    static PRNG rng;
    std::uniform_int_distribution<T> distribution(l, r);
    return distribution(rng.engine);
}
}  // namespace my_random
#endif  // CPP_HSE_RANDOM_H
