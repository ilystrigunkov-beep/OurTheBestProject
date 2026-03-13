#pragma once

#include "app/AnalysisController.hpp"
#include "app/CandleChartView.hpp"

#include <QMainWindow>

#include <memory>
#include <string>

namespace app {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

    void open_csv(const std::string& path);

private slots:
    void on_open_file();

private:
    void create_menu();

    std::unique_ptr<AnalysisController> controller_;
    CandleChartView* chart_view_{nullptr};
};

} // namespace app

