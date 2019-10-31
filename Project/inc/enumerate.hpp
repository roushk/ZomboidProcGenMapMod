#pragma once

template <typename T, typename TIter = decltype(std::begin(std::declval<T>())), typename = decltype(std::end(std::declval<T>()))>
constexpr auto enumerate(T&& iterable, size_t start = 0)
{
  struct Iterator
  {
    size_t i;
    TIter iter;
    bool operator!=(const Iterator& other) const { return iter != other.iter; }
    void operator++() { ++i; ++iter; }
    auto operator*() const { return std::tie(i, *iter); }
  };

  struct IterableWrapper
  {
    size_t startVal;
    T iterable;
    auto begin() { return Iterator{ startVal, std::begin(iterable) }; }
    auto end() { return Iterator{ startVal, std::end(iterable) }; }
  };

  return IterableWrapper{ start, std::forward<T>(iterable) };
}
