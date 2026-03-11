#pragma once

namespace indicators {

class Indicator {
public:
    virtual ~Indicator() = default;

    [[nodiscard]] virtual const char* name() const noexcept = 0;
};

} // namespace indicators

