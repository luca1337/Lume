#include <Lume/Random.h>

std::mt19937 Rng::mMtGenerator = std::mt19937(std::random_device{}());