#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chart(nullptr)
    , chartView(nullptr)
{
    ui->setupUi(this);

    // Инициализация цветов для графиков
    colors = {
        Qt::blue,
        Qt::red,
        Qt::green,
        Qt::magenta,
        Qt::cyan,
        Qt::darkYellow,
        Qt::darkMagenta,
        Qt::darkCyan,
        Qt::darkRed,
        Qt::darkGreen
    };

    // Создаем контейнер для графика
    QWidget *chartContainer = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(chartContainer);

    // Создаем кнопки
    QPushButton *loadButton = new QPushButton("Загрузить файлы (1-10)", this);
    QPushButton *clearButton = new QPushButton("Очистить график", this);

    connect(loadButton, &QPushButton::clicked, this, &MainWindow::on_loadFileButton_clicked);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(clearButton);

    layout->addLayout(buttonLayout);

    // Создаем начальный пустой график
    chart = new QChart();
    chart->setTitle("Автокорреляция данных");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);

    setCentralWidget(chartContainer);

    // Устанавливаем начальные оси
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    axisX->setTitleText("Lag");
    axisY->setTitleText("Автокорреляция");
    axisY->setRange(-1.0, 1.0);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QColor MainWindow::getColorForIndex(int index)
{
    return colors[index % colors.size()];
}

void MainWindow::on_loadFileButton_clicked()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                          "Выберите файлы с данными автокорреляции (1-10)",
                                                          "",
                                                          "Текстовые файлы (*.txt);;Все файлы (*)");

    if (filenames.isEmpty()) {
        return;
    }

    if (filenames.size() > 10) {
        QMessageBox::warning(this, "Предупреждение",
                             "Выбрано более 10 файлов. Будет загружено только первые 10.");
        filenames = filenames.mid(0, 10);
    }

    loadAutocorrelationData(filenames);
}

void MainWindow::on_clearButton_clicked()
{
    // Очищаем график
    chart->removeAllSeries();

    // Создаем новые оси
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    axisX->setTitleText("Lag");
    axisY->setTitleText("Автокорреляция");
    axisY->setRange(-1.0, 1.0);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    chart->setTitle("Автокорреляция данных");
}

void MainWindow::loadAutocorrelationData(const QStringList &filenames)
{
    QVector<QVector<QPointF>> allData;
    QStringList loadedFiles;

    for (const QString &filename : filenames) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Ошибка",
                                  QString("Не удалось открыть файл:\n%1").arg(filename));
            continue;
        }

        QVector<QPointF> dataPoints;
        QTextStream in(&file);
        bool inAutocorrelation = false;
        int lineNumber = 0;

        while (!in.atEnd()) {
            QString line = in.readLine();
            lineNumber++;

            line = line.trimmed();

            if (line.isEmpty() || line.startsWith("#")) {
                if (line.contains("# Autocorrelation results")) {
                    inAutocorrelation = true;
                }
                continue;
            }

            if (inAutocorrelation) {
                QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
                if (parts.size() >= 2) {
                    bool ok1, ok2;
                    double x = parts[0].toDouble(&ok1);
                    double y = parts[1].toDouble(&ok2);

                    if (ok1 && ok2) {
                        dataPoints.append(QPointF(x, y));
                    }
                }
            }
        }

        file.close();

        if (!dataPoints.isEmpty()) {
            allData.append(dataPoints);
            loadedFiles.append(QFileInfo(filename).fileName());
        }
    }

    if (allData.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение",
                             "Не найдено данных автокорреляции в выбранных файлах.\n"
                             "Убедитесь, что файлы содержат секцию '# Autocorrelation results'.");
        return;
    }

    createChart(allData, loadedFiles);

    QString message = QString("Загружено %1 наборов данных:").arg(allData.size());
    // for (const QString &file : loadedFiles) {
    //     message += "\n- " + file;
    // }
    // Используем итератор для QStringList
    for (int i = 0; i < loadedFiles.size(); ++i) {
        message += "\n- " + loadedFiles.at(i);
    }
    QMessageBox::information(this, "Успех", message);
}

void MainWindow::createChart(const QVector<QVector<QPointF>> &allData, const QStringList &filenames)
{
    // Очищаем старый график
    chart->removeAllSeries();

    // Находим глобальные минимум и максимум по X
    double globalMinX = std::numeric_limits<double>::max();
    double globalMaxX = std::numeric_limits<double>::lowest();

    for (const auto &data : allData) {
        if (!data.isEmpty()) {
            if (data.first().x() < globalMinX) globalMinX = data.first().x();
            if (data.last().x() > globalMaxX) globalMaxX = data.last().x();
        }
    }

    // Добавляем графики для каждого набора данных
    for (int i = 0; i < allData.size(); ++i) {
        const auto &data = allData[i];
        QString name = i < filenames.size() ? filenames[i] : QString("Данные %1").arg(i + 1);

        QLineSeries *series = new QLineSeries();
        series->setName(name);

        for (const QPointF &point : data) {
            series->append(point);
        }

        QColor color = getColorForIndex(i);
        QPen pen(series->pen());
        pen.setColor(color);
        pen.setWidth(2);
        series->setPen(pen);

        chart->addSeries(series);
    }

    // Настраиваем оси
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();

    axisX->setTitleText("Lag");
    axisY->setTitleText("Автокорреляция");
    axisY->setRange(-1.0, 1.0);

    // Добавляем отступы по X
    if (globalMinX != std::numeric_limits<double>::max() &&
        globalMaxX != std::numeric_limits<double>::lowest()) {
        double padding = (globalMaxX - globalMinX) * 0.05;
        if (padding < 1.0) padding = 1.0;
        axisX->setRange(globalMinX - padding, globalMaxX + padding);
    } else {
        axisX->setRange(0, 10);
    }

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    // Прикрепляем все серии к осям
    for (auto series : chart->series()) {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }

    // Настраиваем отображение
    chart->setTitle("Автокорреляция данных");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Добавляем горизонтальную линию на y=0 для наглядности
    QLineSeries *zeroLine = new QLineSeries();
    double minX = axisX->min();
    double maxX = axisX->max();
    zeroLine->append(minX, 0);
    zeroLine->append(maxX, 0);
    zeroLine->setName("Нулевая линия");

    QPen zeroPen(zeroLine->pen());
    zeroPen.setColor(Qt::red);
    zeroPen.setStyle(Qt::DashLine);
    zeroLine->setPen(zeroPen);

    chart->addSeries(zeroLine);
    zeroLine->attachAxis(axisX);
    zeroLine->attachAxis(axisY);

    // Добавляем горизонтальные линии для уровней значимости (используем средний размер выборки)
    int totalSize = 0;
    for (const auto &data : allData) {
        totalSize += data.size();
    }
    int avgSize = allData.isEmpty() ? 50 : totalSize / allData.size();

    QLineSeries *upperLine = new QLineSeries();
    QLineSeries *lowerLine = new QLineSeries();
    double significance = 0.6745 / sqrt(avgSize); // 50% доверительный интервал

    upperLine->append(minX, significance);
    upperLine->append(maxX, significance);
    lowerLine->append(minX, -significance);
    lowerLine->append(maxX, -significance);

    upperLine->setName("68% доверительный интервал");
    lowerLine->setName("");

    QPen upperPen(upperLine->pen());
    upperPen.setColor(Qt::darkGreen);
    upperPen.setStyle(Qt::DashLine);
    upperLine->setPen(upperPen);

    QPen lowerPen(lowerLine->pen());
    lowerPen.setColor(Qt::darkGreen);
    lowerPen.setStyle(Qt::DashLine);
    lowerLine->setPen(lowerPen);

    chart->addSeries(upperLine);
    chart->addSeries(lowerLine);
    upperLine->attachAxis(axisX);
    upperLine->attachAxis(axisY);
    lowerLine->attachAxis(axisX);
    lowerLine->attachAxis(axisY);

    chart->legend()->show();
}