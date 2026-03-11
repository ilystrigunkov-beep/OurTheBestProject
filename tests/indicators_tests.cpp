#include <gtest/gtest.h>

#include "indicators/DoubleMA.hpp"
#include "indicators/MovingAverage.hpp"

TEST(MovingAverageTest, ComputesSimpleAverage) {
    indicators::MovingAverage ma(3);
    const double data_arr[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::span<const double> data(data_arr, 5);

    const auto result = ma.compute(data);

    ASSERT_EQ(result.size(), 3);
    EXPECT_DOUBLE_EQ(result[0], 2.0);
    EXPECT_DOUBLE_EQ(result[1], 3.0);
    EXPECT_DOUBLE_EQ(result[2], 4.0);
}

TEST(DoubleMATest, DetectsGoldenAndDeathCrosses) {
    // Slow MA smooths more than fast MA, crosses expected.
    const double prices_arr[] = {1, 2, 3, 4, 5, 4, 3, 2, 3, 4};
    std::span<const double> prices(prices_arr, 10);

    indicators::DoubleMA dma(2, 4);
    const auto crossovers = dma.find_crossovers(prices);

    ASSERT_FALSE(crossovers.empty());
}

