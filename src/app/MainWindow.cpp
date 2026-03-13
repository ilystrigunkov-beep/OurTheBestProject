#include "app/MainWindow.hpp"

#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>

#include <type_traits>
#include <variant>

namespace app {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      controller_(std::make_unique<AnalysisController>(this)) {
    setWindowTitle("OurProject - Data Analyzer");
    resize(1100, 700);

    chart_view_ = new CandleChartView(this);
    setCentralWidget(chart_view_);

    create_menu();
    statusBar()->showMessage("Open a CSV file to analyze.");
}

void MainWindow::create_menu() {
    auto* file_menu = menuBar()->addMenu("&File");

    auto* open_action = new QAction("&Open CSV...", this);
    open_action->setShortcut(QKeySequence::Open);
    connect(open_action, &QAction::triggered, this, &MainWindow::on_open_file);
    file_menu->addAction(open_action);

    file_menu->addSeparator();

    auto* exit_action = new QAction("E&xit", this);
    exit_action->setShortcut(QKeySequence::Quit);
    connect(exit_action, &QAction::triggered, this, &QWidget::close);
    file_menu->addAction(exit_action);
}

void MainWindow::on_open_file() {
    const auto filename = QFileDialog::getOpenFileName(
        this,
        "Open CSV file",
        {},
        "CSV Files (*.csv);;All Files (*)");

    if (filename.isEmpty()) {
        return;
    }

    open_csv(filename.toStdString());
}

void MainWindow::open_csv(const std::string& path) {
    try {
        auto result = controller_->load_and_analyze(path, 5, 20, 2.0);
        analysis::SpikeList spikes;
        std::visit(
            [this, &spikes](const auto& spike_result) {
                using ResultType = std::decay_t<decltype(spike_result)>;
                if constexpr (std::is_same_v<ResultType, analysis::SpikeList>) {
                    spikes = spike_result;
                } else {
                    QMessageBox::warning(this,
                                         "Spike Detection",
                                         QString::fromStdString(spike_result));
                }
            },
            result.spikes);

        chart_view_->set_data(*result.series,
                              result.sma_fast,
                              5,
                              result.sma_slow,
                              20,
                              result.crossovers,
                              spikes);

        statusBar()->showMessage(QString("Loaded and analyzed: %1").arg(QString::fromStdString(path)));
    } catch (const std::exception& ex) {
        QMessageBox::critical(this, "Error", ex.what());
        statusBar()->showMessage("Failed to analyze file.");
    }
}

} // namespace app
