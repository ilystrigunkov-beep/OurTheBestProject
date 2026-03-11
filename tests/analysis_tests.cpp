#include <gtest/gtest.h>

#include "analysis/SpikeDetector.hpp"
#include "analysis/Statistics.hpp"

TEST(StatisticsTest, ComputesMeanAndStddev) {
    const double data_arr[] = {1.0, 2.0, 3.0, 4.0};
    std::span<const double> data(data_arr, 4);

    const auto stats = analysis::compute_summary(data);

    EXPECT_NEAR(stats.mean, 2.5, 1e-9);
    EXPECT_GT(stats.stddev, 0.0);
}

TEST(SpikeDetectorTest, DetectsSimpleSpike) {
    const double prices_arr[] = {100.0, 101.0, 102.0, 130.0, 131.0};
    std::span<const double> prices(prices_arr, 5);

    analysis::SpikeDetector detector(1.0);
    const auto spikes = detector.detect(prices);

    ASSERT_FALSE(spikes.empty());
}

