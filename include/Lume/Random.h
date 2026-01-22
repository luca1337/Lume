#pragma once

#include <algorithm>
#include <Lume/Export.h>
#include <numbers>
#include <random>

class RENDER_API Rng
{
public:
    template <typename Type>
    static Type generate_random_number(const Type min, const Type max) { return generate_random_number_internal(min, max, std::is_integral<Type>()); }

    template <typename T>
    static void shuffle_array(std::vector<T> &arr) { std::shuffle(arr.begin(), arr.end(), mMtGenerator); }

    template <typename Type>
    static std::tuple<Type, Type> generate_random_unit_in_circle(const Type radius)
    {
	    const auto angle = generate_random_number<float>(0.0f, 2.0f * static_cast<float>(std::numbers::pi));
        const auto r = std::sqrt(generate_random_number<float>(0.0f, 1.0f));
        return std::make_tuple(std::cos(angle) * r * radius, std::sin(angle) * r * radius);
    }
private:
    template <typename Type>
    static Type generate_random_number_internal(const Type min, const Type max, std::true_type)
    {
        auto distribution = std::uniform_int_distribution<Type>(min, max);
        return distribution(mMtGenerator);
    }

    template <typename Type>
    static Type generate_random_number_internal(const Type min, const Type max, std::false_type)
    {
        auto distribution = std::uniform_real_distribution<Type>(min, max);
        return distribution(mMtGenerator);
    }

    static std::mt19937 mMtGenerator;
};
