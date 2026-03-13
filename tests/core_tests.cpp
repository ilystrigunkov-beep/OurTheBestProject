#include <gtest/gtest.h>
#include "core/CsvLoader.hpp"
#include "core/Exceptions.hpp"
#include "core/TimeSeries.hpp"
#include <filesystem>
#include <fstream>
#include <string>

namespace {
std::filesystem::path write_temp_csv(const std::string& contents) {
    const auto dir = std::filesystem::temp_directory_path();
    const auto path = dir / "ourproject_test.csv";

    std::ofstream out(path, std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to create temp csv for tests");
    }
    out << contents;
    out.close();

    return path;
}
} // namespace
TEST(CsvLoaderTest, LoadsInvestingComFormatWithThousandsSeparators) {
    const std::string csv =
        "\"Date\",\"Price\",\"Open\",\"High\",\"Low\",\"Vol.\",\"Change %\"\n"
        "\"03/03/2026\",\"6,738.17\",\"6,763.38\",\"6,785.38\",\"6,730.91\",\"\",\"-2.08%\"\n"
        "\"03/02/2026\",\"6,881.62\",\"6,824.36\",\"6,901.01\",\"6,796.85\",\"\",\"0.04%\"\n";
    const auto path = write_temp_csv(csv);
    const auto series = core::CsvLoader::load(path.string());
    ASSERT_NE(series, nullptr);
    ASSERT_EQ(series->size(), 2u);
    const auto& c0 = series->at(0);
    EXPECT_NEAR(c0.open(), 6763.38, 1e-6);
    EXPECT_NEAR(c0.high(), 6785.38, 1e-6);
    EXPECT_NEAR(c0.low(), 6730.91, 1e-6);
    EXPECT_NEAR(c0.close(), 6738.17, 1e-6);
}
TEST(CsvLoaderTest, ThrowsFileExceptionWhenMissing) {
    EXPECT_THROW(core::CsvLoader::load("this_file_should_not_exist_12345.csv"),
                 core::FileException);
}