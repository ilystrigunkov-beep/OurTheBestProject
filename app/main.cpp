#include "app/MainWindow.hpp"

#include <QApplication>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    app::MainWindow window;
    window.show();

    if (argc >= 2) {
        window.open_csv(argv[1]);
    }

    return app.exec();
}
