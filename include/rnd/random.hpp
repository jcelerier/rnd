#pragma once
/* Copyright (c) 2018 Arvid Gerstmann. */
/* Copyright (c) 2020 Jean-Michaël Celerier. */
/* This code is licensed under MIT license. */
/* https://arvid.io/2018/07/02/better-cxx-prng/ */

#include <cstdint>
#include <random>
#if __has_include(<rnd_export.h>)
#include <rnd_export.h>
#endif

namespace rnd
{
/* For reference purpose - but we only use PCG
class splitmix
{
public:
    using result_type = uint32_t;
    static constexpr result_type min() noexcept { return 0; }
    static constexpr result_type max() noexcept { return UINT32_MAX; }

    constexpr splitmix() noexcept
        : m_seed{1}
    {

    }

    explicit splitmix(std::random_device &rd) noexcept
    {
        seed(rd);
    }

    void seed(std::random_device &rd) noexcept
    {
        m_seed = uint64_t(rd()) << 31 | uint64_t(rd());
    }

    constexpr result_type operator()() noexcept
    {
        uint64_t z = (m_seed += UINT64_C(0x9E3779B97F4A7C15));
        z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
        z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
        return result_type((z ^ (z >> 31)) >> 31);
    }

    constexpr void discard(uint64_t n) noexcept
    {
        for (uint64_t i = 0; i < n; ++i)
            operator()();
    }

    friend inline constexpr bool operator==(splitmix lhs, splitmix rhs) noexcept
    {
        return lhs.m_seed == rhs.m_seed;
    }

    friend inline constexpr bool operator!=(splitmix lhs, splitmix rhs) noexcept
    {
        return lhs.m_seed != rhs.m_seed;
    }
private:
    uint64_t m_seed;
};


class xorshift
{
public:
    using result_type = uint32_t;
    static constexpr result_type min() noexcept { return 0; }
    static constexpr result_type max() noexcept { return UINT32_MAX; }

    constexpr xorshift() noexcept : m_seed(0xc1f651c67c62c6e0ull) {}
    explicit xorshift(std::random_device &rd) noexcept
    {
        seed(rd);
    }

    void seed(std::random_device &rd) noexcept
    {
        m_seed = uint64_t(rd()) << 31 | uint64_t(rd());
    }

    constexpr result_type operator()() noexcept
    {
        uint64_t result = m_seed * 0xd989bcacc137dcd5ull;
        m_seed ^= m_seed >> 11;
        m_seed ^= m_seed << 31;
        m_seed ^= m_seed >> 18;
        return uint32_t(result >> 32ull);
    }

    constexpr void discard(unsigned long long n) noexcept
    {
        for (unsigned long long i = 0; i < n; ++i)
            operator()();
    }

    friend inline constexpr bool operator==(xorshift const &lhs, xorshift const &rhs) noexcept
    {
        return lhs.m_seed == rhs.m_seed;
    }
    friend inline constexpr bool operator!=(xorshift const &lhs, xorshift const &rhs) noexcept
    {
        return lhs.m_seed != rhs.m_seed;
    }
private:
    uint64_t m_seed;
};
*/
class pcg
{
public:
    using result_type = uint32_t;
    static constexpr result_type min() noexcept { return 0; }
    static constexpr result_type max() noexcept { return UINT32_MAX; }

    constexpr pcg() noexcept
        : m_state(UINT64_C(0x853c49e6748fea9b))
        , m_inc(UINT64_C(0xda3e39cb94b95bdb))
    {}
    explicit pcg(std::random_device &rd) noexcept
    {
        seed(rd);
    }

    void seed(std::random_device &rd) noexcept
    {
        uint64_t s0 = uint64_t(rd()) << 31 | uint64_t(rd());
        uint64_t s1 = uint64_t(rd()) << 31 | uint64_t(rd());

        m_state = 0;
        m_inc = (s1 << 1) | 1;
        (void)operator()();
        m_state += s0;
        (void)operator()();
    }

    constexpr result_type operator()() noexcept
    {
        uint64_t oldstate = m_state;
        m_state = oldstate * UINT64_C(6364136223846793005) + m_inc;
        uint32_t xorshifted = uint32_t(((oldstate >> 18u) ^ oldstate) >> 27u);
        int rot = oldstate >> 59u;
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    }

    constexpr void discard(unsigned long long n) noexcept
    {
        for (unsigned long long i = 0; i < n; ++i)
            operator()();
    }

    friend constexpr bool operator==(pcg lhs, pcg rhs) noexcept
    {
        return lhs.m_state == rhs.m_state
            && lhs.m_inc == rhs.m_inc;
    }
    friend constexpr bool operator!=(pcg lhs, pcg rhs) noexcept
    {
        return lhs.m_state != rhs.m_state
            || lhs.m_inc != rhs.m_inc;
    }

private:
    uint64_t m_state;
    uint64_t m_inc;
};


/**
 * Warning: these functions use a deterministic seed, don't use them for anything
 * requiring proper randomness.
 */
#if __has_include(<rnd_export.h>)
RND_EXPORT
pcg& fast_random_device() noexcept;

inline int rand(int min, int max) noexcept {
  return std::uniform_int_distribution<>{min, max}(fast_random_device());
}

inline float rand(float min, float max) noexcept {
  return std::uniform_real_distribution<>{min, max}(fast_random_device());
}

inline double rand(double min, double max) noexcept {
  return std::uniform_real_distribution<>{min, max}(fast_random_device());
}
#endif

}
