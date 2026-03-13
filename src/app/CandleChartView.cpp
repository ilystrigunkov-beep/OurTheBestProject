#include "app/CandleChartView.hpp"

#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QCandlestickSet>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>

#include <QBrush>
#include <QPen>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QDateTime>

#include <algorithm>
#include <limits>

namespace app {

namespace {

double candle_x_value(const core::Candle& c, std::size_t fallback_index) {
    const auto secs = std::chrono::duration_cast<std::chrono::seconds>(
        c.timestamp().time_since_epoch());
    const auto ts = secs.count();
    if (ts != 0) {
        return static_cast<double>(ts) * 1000.0;
    }
    return static_cast<double>(fallback_index) * 1000.0;
}

} // namespace

CandleChartView::CandleChartView(QWidget* parent)
    : QChartView(new QChart(), parent) {
    setRenderHint(QPainter::Antialiasing, true);

    candles_ = new QCandlestickSeries();
    candles_->setName("Candles");
    candles_->setIncreasingColor(QColor(0, 170, 0));
    candles_->setDecreasingColor(QColor(200, 0, 0));

    sma_fast_ = new QLineSeries();
    sma_fast_->setName("SMA Fast");
    sma_fast_->setPen(QPen(QColor(0, 120, 255), 2));

    sma_slow_ = new QLineSeries();
    sma_slow_->setName("SMA Slow");
    sma_slow_->setPen(QPen(QColor(255, 165, 0), 2));

    crossovers_ = new QScatterSeries();
    crossovers_->setName("Crossovers");
    crossovers_->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    crossovers_->setMarkerSize(10.0);
    crossovers_->setColor(QColor(150, 0, 200));

    spikes_ = new QScatterSeries();
    spikes_->setName("Spikes (2σ)");
    spikes_->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    spikes_->setMarkerSize(10.0);
    spikes_->setColor(QColor(0, 0, 0));

    chart()->addSeries(candles_);
    chart()->addSeries(sma_fast_);
    chart()->addSeries(sma_slow_);
    chart()->addSeries(crossovers_);
    chart()->addSeries(spikes_);
    chart()->legend()->setVisible(true);
    chart()->setTitle("Candlestick chart");

    axis_x_ = new QDateTimeAxis();
    axis_y_ = new QValueAxis();
    axis_x_->setTitleText("Date");
    axis_x_->setFormat("MM/dd/yyyy");
    axis_y_->setTitleText("Price / Index");
    chart()->addAxis(axis_x_, Qt::AlignBottom);
    chart()->addAxis(axis_y_, Qt::AlignLeft);

    for (auto* s : chart()->series()) {
        s->attachAxis(axis_x_);
        s->attachAxis(axis_y_);
    }
}

void CandleChartView::set_data(const core::TimeSeries& series,
                               const std::vector<double>& sma_fast,
                               std::size_t fast_window,
                               const std::vector<double>& sma_slow,
                               std::size_t slow_window,
                               const std::vector<indicators::Crossover>& crossovers,
                               const std::vector<analysis::Spike>& spikes) {
    candles_->clear();
    sma_fast_->clear();
    sma_slow_->clear();
    crossovers_->clear();
    spikes_->clear();

    const auto span = series.candles();

    double min_y = std::numeric_limits<double>::infinity();
    double max_y = -std::numeric_limits<double>::infinity();
    long long min_ts = std::numeric_limits<long long>::max();
    long long max_ts = std::numeric_limits<long long>::min();

    for (std::size_t i = 0; i < span.size(); ++i) {
        const auto& c = span[i];
        const double x = candle_x_value(c, i);

        auto* set = new QCandlestickSet(c.open(), c.high(), c.low(), c.close(), x);
        candles_->append(set);

        min_y = std::min(min_y, c.low());
        max_y = std::max(max_y, c.high());

        const auto secs = std::chrono::duration_cast<std::chrono::seconds>(
            c.timestamp().time_since_epoch()).count();
        const long long ms = secs * 1000LL;
        min_ts = std::min(min_ts, ms);
        max_ts = std::max(max_ts, ms);
    }
    if (fast_window > 0 && sma_fast.size() + fast_window - 1 <= span.size()) {
        for (std::size_t k = 0; k < sma_fast.size(); ++k) {
            const std::size_t i = k + fast_window - 1;
            const double x = candle_x_value(span[i], i);
            sma_fast_->append(x, sma_fast[k]);
        }
    }
    if (slow_window > 0 && sma_slow.size() + slow_window - 1 <= span.size()) {
        for (std::size_t k = 0; k < sma_slow.size(); ++k) {
            const std::size_t i = k + slow_window - 1;
            const double x = candle_x_value(span[i], i);
            sma_slow_->append(x, sma_slow[k]);
        }
    }
    for (const auto& sig : crossovers) {
        if (sig.index >= span.size()) {
            continue;
        }
        const auto& c = span[sig.index];
        const double x = candle_x_value(c, sig.index);
        crossovers_->append(x, c.close());
    }
    for (const auto& sp : spikes) {
        if (sp.index >= span.size()) {
            continue;
        }
        const auto& c = span[sp.index];
        const double x = candle_x_value(c, sp.index);
        spikes_->append(x, c.close());
    }
    if (min_ts != std::numeric_limits<long long>::max() &&
        max_ts != std::numeric_limits<long long>::min() &&
        min_ts != max_ts) {
        axis_x_->setRange(QDateTime::fromMSecsSinceEpoch(min_ts),
                          QDateTime::fromMSecsSinceEpoch(max_ts));
        }
    if (std::isfinite(min_y) && std::isfinite(max_y) && min_y != max_y) {
        const double padding = (max_y - min_y) * 0.05;
        axis_y_->setRange(min_y - padding, max_y + padding);
    }
}

} // namespace app
