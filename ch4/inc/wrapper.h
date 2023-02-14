#ifndef __wrapper
#define __wrapper

#include <type_traits>
#include <utility>

#define WRAP(SourceClass, WrapperClass)         \
template <bool isScalar>                        \
class WrapperClass ## Body;                     \
                                                \
template <>                                     \
class WrapperClass ## Body<true>                \
{                                               \
public:                                         \
    explicit WrapperClass ## Body(              \
        SourceClass instance)                   \
        : instance_{ instance } {}              \
                                                \
    explicit operator SourceClass() const       \
    { return instance_; }                       \
                                                \
private:                                        \
    SourceClass instance_;                      \
};                                              \
                                                \
template <>                                     \
class WrapperClass ## Body<false>               \
{                                               \
public:                                         \
    explicit WrapperClass ## Body(              \
        const SourceClass& instance)            \
        : instance_{ instance } {}              \
                                                \
    explicit WrapperClass ## Body(              \
        SourceClass&& instance)                 \
        : instance_{ std::move(instance) } {}   \
                                                \
    explicit operator SourceClass&()            \
    { return instance_; }                       \
                                                \
    explicit operator const SourceClass&() const\
    { return instance_; }                       \
                                                \
private:                                        \
    SourceClass instance_;                      \
};                                              \
using WrapperClass = WrapperClass ## Body<      \
    std::is_scalar_v<SourceClass>>;

#endif  // __wrapper