#pragma once
#include <memory>

namespace PPsat_base
{
namespace detail
{
template <typename T>
class iterator_any_base
{
public:
    virtual void advance() = 0;
    virtual T dereference() const = 0;
    virtual bool is_equal(const iterator_any_base& other) const = 0;

    virtual std::unique_ptr<iterator_any_base<T>> copy() const = 0;

    virtual ~iterator_any_base()
    {}
};

template <typename T, typename I>
class iterator_any_impl : public iterator_any_base<T>
{
    I i;

public:
    iterator_any_impl(const I& i)
        : i(i)
    {}

    iterator_any_impl(I&& i)
        : i(std::move(i))
    {}

    void advance() override final
    {
        ++i;
    }

    T dereference() const override final
    {
        return T(*i);
    }

    bool is_equal(const iterator_any_base<T>& other) const
    {
        auto* const other_downcast =
            dynamic_cast<const iterator_any_impl*>(&other);
        return other_downcast && i == other_downcast->i;
    }

    std::unique_ptr<iterator_any_base<T>> copy() const override final
    {
        return std::make_unique<iterator_any_impl>(i);
    }
};
}

template <typename T>
class iterator_any
{
    std::unique_ptr<detail::iterator_any_base<T>> base;

public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;

    iterator_any() = default;

    explicit iterator_any(auto&& i)
        : base(std::make_unique<
               detail::iterator_any_impl<T, std::remove_cvref_t<decltype(i)>>>(
              std::forward<decltype(i)>(i)))
    {}

    iterator_any(const iterator_any& other)
        : base(other.base->copy())
    {}

    iterator_any(iterator_any&& i)
        : base(std::move(i.base))
    {}

    iterator_any& operator++()
    {
        base->advance();
        return *this;
    }

    iterator_any operator++(int)
    {
        auto i = *this;
        ++i;
        return i;
    }

    T operator*() const
    {
        return base->dereference();
    }

    auto& operator=(const iterator_any& other)
    {
        base = other.base->copy();
        return *this;
    }

    auto& operator=(iterator_any&& other)
    {
        base = std::move(other.base);
        return *this;
    }

    bool operator==(const iterator_any& other) const
    {
        return (!base && !other.base) || base->is_equal(*other.base);
    }
};
}
