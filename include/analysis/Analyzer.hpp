#pragma once

namespace analysis {

class Analyzer {
public:
    virtual ~Analyzer() = default;

    [[nodiscard]] virtual const char* name() const noexcept = 0;
};

} // namespace analysis

