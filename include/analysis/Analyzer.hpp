#pragma once
#include <memory>
#include <string>
namespace analysis {
    class Analyzer {
    public:
        virtual ~Analyzer() = default;
        [[nodiscard]] virtual std::shared_ptr<Analyzer> clone() const = 0;
        [[nodiscard]] virtual const char* name() const noexcept = 0;
    };
}
