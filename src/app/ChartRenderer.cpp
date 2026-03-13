#include "app/ChartRenderer.hpp"

#include <iomanip>
#include <iostream>
#include <type_traits>
#include <variant>

#include "app/AnalysisController.hpp"

namespace app {

void ChartRenderer::print_series(const char* name,
                                 const std::vector<double>& data,
                                 std::size_t max_points) {
    std::cout << name << " (" << data.size() << " points): ";
    const auto count = std::min<std::size_t>(data.size(), max_points);
    for (std::size_t i = 0; i < count; ++i) {
        std::cout << std::fixed << std::setprecision(2) << data[i];
        if (i + 1 < count) {
            std::cout << ", ";
        }
    }
    if (data.size() > count) {
        std::cout << ", ...";
    }
    std::cout << '\n';
}

void ChartRenderer::render_summary(const std::vector<double>& prices,
                                   const std::vector<double>& sma_fast,
                                   const std::vector<double>& sma_slow,
                                   const std::vector<indicators::Crossover>& crossovers,
                                   const SpikeDisplayResult& spikes) const {
    std::cout << "=== Price & Indicator Summary ===\n";

    print_series("Prices     ", prices, 20);
    print_series("SMA Fast   ", sma_fast, 20);
    print_series("SMA Slow   ", sma_slow, 20);

    std::cout << "\nCrossover signals (" << crossovers.size() << "):\n";
    for (const auto& sig : crossovers) {
        const char* type = sig.type == indicators::CrossoverType::GoldenCross
                               ? "GoldenCross"
                               : "DeathCross";
        std::cout << "  index=" << sig.index << " type=" << type << '\n';
    }

    std::visit(
        [](const auto& result) {
            using ResultType = std::decay_t<decltype(result)>;
            if constexpr (std::is_same_v<ResultType, analysis::SpikeList>) {
                std::cout << "\nSpikes (" << result.size() << "):\n";
                for (const auto& spike : result) {
                    std::cout << "  index=" << spike.index
                              << " change=" << std::fixed << std::setprecision(2)
                              << spike.change_pct << "%\n";
                }
            } else {
                std::cout << "\nSpikes: unavailable (" << result << ")\n";
            }
        },
        spikes);

    std::cout << "\n(For real plotting, these series can be exported to a file\n"
                 "and visualized with GNUPlot or matplotlib.)\n";
}

} // namespace app
