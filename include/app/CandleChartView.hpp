#pragma once

#include "analysis/SpikeDetector.hpp"
#include "core/TimeSeries.hpp"
#include "indicators/DoubleMA.hpp"

#include <QtCharts/QChartView>
#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>

#include <vector>

class QCandlestickSeries;
class QLineSeries;
class QScatterSeries;
class QValueAxis;
class QDateTimeAxis;

namespace app {

class CandleChartView : public QChartView {
    Q_OBJECT
public:
    explicit CandleChartView(QWidget* parent = nullptr);

    void set_data(const core::TimeSeries& series,
                  const std::vector<double>& sma_fast,
                  std::size_t fast_window,
                  const std::vector<double>& sma_slow,
                  std::size_t slow_window,
                  const std::vector<indicators::Crossover>& crossovers,
                  const std::vector<analysis::Spike>& spikes);

private:
    void rebuild_axes();

    QCandlestickSeries* candles_{nullptr};
    QLineSeries* sma_fast_{nullptr};
    QLineSeries* sma_slow_{nullptr};
    QScatterSeries* crossovers_{nullptr};
    QScatterSeries* spikes_{nullptr};

    QDateTimeAxis* axis_x_{nullptr};
    QValueAxis* axis_y_{nullptr};
};

} // namespace app

