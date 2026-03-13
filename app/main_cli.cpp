#include "analysis/SpikeDetector.hpp"
#include "app/ChartRenderer.hpp"
#include "core/CsvLoader.hpp"
#include "core/TimeSeries.hpp"
#include "indicators/DoubleMA.hpp"
#include <iostream>
#include <span>
#include <string>
#include <vector>
namespace {
std::vector<double> make_demo_prices() {
    // Simple synthetic series with a visible spike.
    return {100, 101, 102, 103, 120, 121, 122, 119, 118, 117, 130, 129, 128};
}
}
int main(int argc, char** argv) {
    std::vector<double> prices;
    if (argc >= 2) {
        const std::string path = argv[1];
        try {
            auto series = core::CsvLoader::load(path);
            prices = series->closes();
            std::cout << "Loaded " << prices.size()
                      << " closing prices from '" << path << "'.\n";
        } catch (const std::exception& ex) {
            std::cerr << "Failed to load CSV: " << ex.what()
                      << "\nFalling back to demo data.\n";
            prices = make_demo_prices();
        }
    } else {
        std::cout << "No CSV path provided. Using demo data.\n";
        prices = make_demo_prices();
    }
    if (prices.size() < 5) {
        std::cerr << "Not enough data points to perform analysis.\n";
        return 1;
    }
    std::span<const double> price_span(prices.data(), prices.size());
    // Indicators: double moving average and crossovers.
    indicators::DoubleMA dma(3, 7);
    const auto sma_fast = dma.compute_fast(price_span);
    const auto sma_slow = dma.compute_slow(price_span);
    const auto crossovers = dma.find_crossovers(price_span);
    // Analysis: spike detection.
    analysis::SpikeDetector spike_detector(2.0);
    const auto spikes = spike_detector.detect(price_span);
    // Visualization / CLI output.
    app::ChartRenderer renderer;
    renderer.render_summary(prices, sma_fast, sma_slow, crossovers, spikes);
    return 0;
}