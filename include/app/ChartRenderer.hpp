#pragma once

#include "analysis/SpikeDetector.hpp"
#include "app/Renderer.hpp"
#include "indicators/DoubleMA.hpp"

#include <cstddef>
#include <vector>

namespace app {

class ChartRenderer : public Renderer {
public:
    ChartRenderer() = default;

    [[nodiscard]] const char* name() const noexcept override { return "ChartRenderer"; }

    void render_summary(const std::vector<double>& prices,
                        const std::vector<double>& sma_fast,
                        const std::vector<double>& sma_slow,
                        const std::vector<indicators::Crossover>& crossovers,
                        const std::vector<analysis::Spike>& spikes) const;

private:
    static void print_series(const char* name,
                             const std::vector<double>& data,
                             std::size_t max_points);
};

} // namespace app

