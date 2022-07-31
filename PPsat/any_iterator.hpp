#pragma once
#include <memory>

namespace PPsat
{
template <typename T>
class any_iterator_base
{
public:
    virtual void advance() = 0;
    virtual T& dereference() const = 0;
    virtual bool is_equal(const any_iterator_base& other) const = 0;

    virtual std::unique_ptr<any_iterator_base<T>> copy() const = 0;

    virtual ~any_iterator_base()
    {}
};

template <typename T, typename I>
class any_iterator_impl : public any_iterator_base<T>
{
    I i;

public:
    any_iterator_impl(const I& i)
        : i(i)
    {}
    
    any_iterator_impl(I&& i)
        : i(std::move(i))
    {}

    void advance() override final
    {
        ++i;
    }

    T& dereference() const override final
    {
        return *i;
    }

    bool is_equal(const any_iterator_base<T>& other) const
    {
        auto* const other_downcast = dynamic_cast<const any_iterator_impl*>(&other);
        return other_downcast && i == other_downcast->i;
    }

    std::unique_ptr<any_iterator_base<T>> copy() const override final
    {
        return std::make_unique<any_iterator_impl>(i);
    }
};

template <typename T>
class any_iterator
{
    std::unique_ptr<any_iterator_base<T>> base;

public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;

    any_iterator() = default;

    explicit any_iterator(auto&& i)
        : base(std::make_unique<
               any_iterator_impl<T, std::remove_cvref_t<decltype(i)>>>(
              std::forward<decltype(i)>(i)))
    {}

    any_iterator(const any_iterator& other)
        : base(other.base->copy())
    {}

    any_iterator(any_iterator&& i)
        : base(std::move(i.base))
    {}

    any_iterator& operator++()
    {
        base->advance();
        return *this;
    }

    any_iterator operator++(int)
    {
        auto i = *this;
        ++i;
        return i;
    }

    T& operator*() const
    {
        return base->dereference();
    }

    auto& operator=(const any_iterator& other)
    {
        base = other.base->copy();
        return *this;
    }
    
    auto& operator=(any_iterator&& other)
    {
        base = std::move(other.base);
        return *this;
    }

    bool operator==(const any_iterator& other) const
    {
        return (!base && !other.base) || base->is_equal(*other.base);
    }
};

template <typename T>
class any_view
{
    any_iterator<T> begin_;
    any_iterator<T> end_;

public:
    explicit any_view(auto&& view)
        : begin_(std::begin(std::forward<decltype(view)>(view)))
        , end_(std::end(std::forward<decltype(view)>(view)))
    {}

    any_view(const any_view& other) = default;
    any_view(any_view&& other) = default;

    auto begin()
    {
        return begin_;
    }
    auto begin() const
    {
        return begin_;
    }

    auto end()
    {
        return end_;
    }
    auto end() const
    {
        return end_;
    }
};
}
