#include <rnd/random.hpp>

namespace rnd
{
namespace 
{
thread_local pcg librnd_rand_state{};
}

pcg& fast_random_device() noexcept
{
  return librnd_rand_state;
}
}