#pragma once

#include <chrono>
#include <optional>

namespace core {

class Candle {
public:
    using TimePoint = std::chrono::sys_seconds;

    Candle() = default;

    Candle(TimePoint timestamp,
           double open,
           double high,
           double low,
           double close,
           std::optional<double> volume = std::nullopt);

    ~Candle() = default;
    Candle(const Candle&) = default;
    Candle(Candle&&) noexcept = default;
    Candle& operator=(const Candle&) = default;
    Candle& operator=(Candle&&) noexcept = default;

    [[nodiscard]] TimePoint timestamp() const noexcept { return timestamp_; }
    [[nodiscard]] double open() const noexcept { return open_; }
    [[nodiscard]] double high() const noexcept { return high_; }
    [[nodiscard]] double low() const noexcept { return low_; }
    [[nodiscard]] double close() const noexcept { return close_; }
    [[nodiscard]] std::optional<double> volume() const noexcept { return volume_; }

private:
    TimePoint timestamp_{};
    double open_{};
    double high_{};
    double low_{};
    double close_{};
    std::optional<double> volume_{};
};

} // namespace core

