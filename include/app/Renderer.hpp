#pragma once

namespace app {

class Renderer {
public:
    virtual ~Renderer() = default;

    [[nodiscard]] virtual const char* name() const noexcept = 0;
};

} // namespace app

