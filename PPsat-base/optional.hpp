#pragma once
#include <functional>
#include <optional>
#include <ranges>

namespace PPsat_base
{
template <typename T>
class optional
{
    std::optional<T> storage;

public:
    optional() = default;

    optional(const std::optional<T>& other)
        : storage(other)
    {}
    optional(std::optional<T>&& other)
        : storage(std::move(other))
    {}

    optional(const T& val)
        : storage(val)
    {}
    optional(T&& val)
        : storage(std::move(val))
    {}

    optional(std::ranges::input_range auto&& view)
        : storage(
              [&view]()
              {
                  for (auto&& val : std::forward<decltype(view)>(view))
                  {
                      return std::optional<T>(std::forward<decltype(val)>(val));
                  }

                  return std::optional<T>();
              }())
    {}

    explicit operator bool() const noexcept
    {
        return storage.has_value();
    }

    auto& operator=(const std::optional<T>& other)
    {
        if (other)
        {
            storage.emplace(*other);
        }
        else
        {
            storage.reset();
        }

        return *this;
    }

    T& operator*()
    {
        return *storage;
    }
    const T& operator*() const
    {
        return *storage;
    }

    T* begin()
    {
        if (storage)
        {
            return &storage.value();
        }
        else
        {
            return nullptr;
        }
    }
    const T* begin() const
    {
        if (storage)
        {
            return &storage.value();
        }
        else
        {
            return nullptr;
        }
    }
    T* end()
    {
        if (storage)
        {
            return &storage.value() + 1;
        }
        else
        {
            return nullptr;
        }
    }
    const T* end() const
    {
        if (storage)
        {
            return &storage.value() + 1;
        }
        else
        {
            return nullptr;
        }
    }

    void emplace(auto&&... args)
    {
        storage.emplace(std::forward<decltype(args)>(args)...);
    }
};

template <typename T>
class optional<T&>
{
    std::optional<std::reference_wrapper<T>> storage;

public:
    optional() = default;

    optional(T& val)
        : storage(std::ref(val))
    {}

    optional(std::ranges::input_range auto&& view)
        : storage(
              [&view]()
              {
                  for (T& val : std::forward<decltype(view)>(view))
                  {
                      return std::optional<std::reference_wrapper<T>>(
                          std::ref(val));
                  }

                  return std::optional<std::reference_wrapper<T>>();
              }())
    {}

    explicit operator bool() const noexcept
    {
        return storage.has_value();
    }

    T& operator*()
    {
        return *storage;
    }
    T& operator*() const
    {
        return *storage;
    }

    T* begin()
    {
        if (storage)
        {
            return &storage.value().get();
        }
        else
        {
            return nullptr;
        }
    }
    T* begin() const
    {
        if (storage)
        {
            return &storage.value().get();
        }
        else
        {
            return nullptr;
        }
    }
    T* end()
    {
        if (storage)
        {
            return &storage.value().get() + 1;
        }
        else
        {
            return nullptr;
        }
    }
    T* end() const
    {
        if (storage)
        {
            return &storage.value().get() + 1;
        }
        else
        {
            return nullptr;
        }
    }
};
}
