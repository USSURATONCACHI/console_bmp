#pragma once

#include <tuple>

namespace bmp_reader {


template<typename T, typename... Args>
struct CtorArg {
    using Type = T;

    std::tuple<Args...> params;

    CtorArg(Args&&... args) : params(std::forward<Args>(args)...) {}

private:
    void sfinae_filter(Args&&... args) {
        T(std::forward<Args>(args)...);
    }
};


} // namespace bmp_reader