#include "core/Candle.hpp"

namespace core {

Candle::Candle(TimePoint timestamp,
               double open,
               double high,
               double low,
               double close,
               std::optional<double> volume)
    : timestamp_(timestamp),
      open_(open),
      high_(high),
      low_(low),
      close_(close),
      volume_(volume) {}

} // namespace core

