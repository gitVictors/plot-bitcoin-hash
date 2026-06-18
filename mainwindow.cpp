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
    , chart_1(nullptr)
    , chartView_1(nullptr)
    , chart_2(nullptr)
    , chartView_2(nullptr)
    , chart_3(nullptr)
    , chartView_3(nullptr)
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
    QVBoxLayout *mainLayout = new QVBoxLayout(chartContainer);

    // Создаем кнопки первой строки ------------------------------------------------------
    QPushButton *loadButton = new QPushButton("Загрузить файлы для графика 1", this);
    QPushButton *clearButton = new QPushButton("Очистить график", this);

    connect(loadButton, &QPushButton::clicked, this, &MainWindow::on_loadFileButton_clicked_1);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked_1);

    // Создаем горизонтальный макет для первой строки кнопок
    QHBoxLayout *buttonLayout1 = new QHBoxLayout();
    buttonLayout1->addWidget(loadButton);
    buttonLayout1->addWidget(clearButton);
    buttonLayout1->addStretch(); // Добавляем растяжение для выравнивания влево

    // Создаем кнопки второй строки-----------------------------------------------------------
    QPushButton *loadButton2 = new QPushButton("Загрузить файлы для графика 2", this);
    QPushButton *clearButton2 = new QPushButton("Очистить график", this);

    connect(loadButton2, &QPushButton::clicked, this, &MainWindow::on_loadFileButton_clicked_2);
    connect(clearButton2, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked_2);

    // Создаем горизонтальный макет для второй строки кнопок
    QHBoxLayout *buttonLayout2 = new QHBoxLayout();
    buttonLayout2->addWidget(loadButton2);
    buttonLayout2->addWidget(clearButton2);
    buttonLayout2->addStretch(); // Добавляем растяжение для выравнивания влево

    // Создаем кнопки третьей строки-----------------------------------------------------------
    QPushButton *loadButton3 = new QPushButton("Загрузить файлы для графика 3", this);
    QPushButton *clearButton3 = new QPushButton("Очистить график", this);

    connect(loadButton3, &QPushButton::clicked, this, &MainWindow::on_loadFileButton_clicked_3);
    connect(clearButton3, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked_3);

    // Создаем горизонтальный макет для второй строки кнопок
    QHBoxLayout *buttonLayout3 = new QHBoxLayout();
    buttonLayout3->addWidget(loadButton3);
    buttonLayout3->addWidget(clearButton3);
    buttonLayout3->addStretch(); // Добавляем растяжение для выравнивания влево

    // Добавляем оба макета в главный вертикальный макет---*****
    mainLayout->addLayout(buttonLayout1);
    mainLayout->addLayout(buttonLayout2);
    mainLayout->addLayout(buttonLayout3);
    //-----------------------------------------------------------

    // Создаем начальный пустой график 1 --------------------------------------------
    chart_1 = new QChart();
    chart_1->setTitle("Автокорреляция данных");
    chart_1->setAnimationOptions(QChart::SeriesAnimations);

    chartView_1 = new QChartView(chart_1);
    chartView_1->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(chartView_1);

    setCentralWidget(chartContainer);


    // Устанавливаем начальные оси
    QValueAxis *axisX_1 = new QValueAxis();
    QValueAxis *axisY_1 = new QValueAxis();
    axisX_1->setTitleText("Lag");
    axisY_1->setTitleText("Автокорреляция");
    axisY_1->setRange(-1.0, 1.0);

    chart_1->addAxis(axisX_1, Qt::AlignBottom);
    chart_1->addAxis(axisY_1, Qt::AlignLeft);

    // Создаем начальный пустой график 2 -----------------------------------------------
    chart_2 = new QChart();
    chart_2->setTitle("Автокорреляция данных");
    chart_2->setAnimationOptions(QChart::SeriesAnimations);

    chartView_2 = new QChartView(chart_2);
    chartView_2->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(chartView_2);

    setCentralWidget(chartContainer);

    // Устанавливаем начальные оси
    QValueAxis *axisX_2 = new QValueAxis();
    QValueAxis *axisY_2 = new QValueAxis();
    axisX_2->setTitleText("Lag");
    axisY_2->setTitleText("Автокорреляция");
    axisY_2->setRange(-1.0, 1.0);

    chart_2->addAxis(axisX_2, Qt::AlignBottom);
    chart_2->addAxis(axisY_2, Qt::AlignLeft);

    // Создаем начальный пустой график 2 -----------------------------------------------
    chart_3 = new QChart();
    chart_3->setTitle("Автокорреляция данных");
    chart_3->setAnimationOptions(QChart::SeriesAnimations);

    chartView_3 = new QChartView(chart_3);
    chartView_3->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(chartView_3);

    setCentralWidget(chartContainer);

    // Устанавливаем начальные оси
    QValueAxis *axisX_3 = new QValueAxis();
    QValueAxis *axisY_3 = new QValueAxis();
    axisX_3->setTitleText("Lag");
    axisY_3->setTitleText("Автокорреляция");
    axisY_3->setRange(-1.0, 1.0);

    chart_3->addAxis(axisX_3, Qt::AlignBottom);
    chart_3->addAxis(axisY_3, Qt::AlignLeft);

}

MainWindow::~MainWindow()
{
    delete ui;
}

QColor MainWindow::getColorForIndex(int index)
{
    return colors[index % colors.size()];
}

//======== Load file button ===============================================
void MainWindow::on_loadFileButton_clicked_1()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                          "Выберите файлы с данными автокорреляции. График 1.",
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

    loadAutocorrelationData(filenames, chart_1);
}

void MainWindow::on_loadFileButton_clicked_2()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                          "Выберите файлы с данными автокорреляции. График 2.",
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

    loadAutocorrelationData(filenames, chart_2);
}


void MainWindow::on_loadFileButton_clicked_3()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                          "Выберите файлы с данными автокорреляции. График 3.",
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

    loadAutocorrelationData(filenames, chart_3);
}

//====================================================================================

//==========Clear button==============================================================
void MainWindow::on_clearButton_clicked_1()
{
    // Очищаем график
    chart_1->removeAllSeries();

    // Удаляем все существующие оси
    QList<QAbstractAxis*> axes = chart_1->axes();
    for (int i = 0; i< axes.size(); ++i){
        QAbstractAxis *ax = axes.at(i);
        chart_1->removeAxis(ax);
        delete ax;
    }
    // Создаем новые оси
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    axisX->setTitleText("Lag");
    axisY->setTitleText("Автокорреляция");
    axisY->setRange(-1.0, 1.0);

    chart_1->addAxis(axisX, Qt::AlignBottom);
    chart_1->addAxis(axisY, Qt::AlignLeft);

    chart_1->setTitle("Автокорреляция данных");

}

void MainWindow::on_clearButton_clicked_2()
{
    // Очищаем график
    chart_2->removeAllSeries();

    // Удаляем все существующие оси
    QList<QAbstractAxis*> axes = chart_2->axes();
    for (int i = 0; i< axes.size(); ++i){
        QAbstractAxis *ax = axes.at(i);
        chart_2->removeAxis(ax);
        delete ax;
    }

    // Создаем новые оси
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    axisX->setTitleText("Lag");
    axisY->setTitleText("Автокорреляция");
    axisY->setRange(-1.0, 1.0);

    chart_2->addAxis(axisX, Qt::AlignBottom);
    chart_2->addAxis(axisY, Qt::AlignLeft);

    chart_2->setTitle("Автокорреляция данных");

}

void MainWindow::on_clearButton_clicked_3()
{
    // Очищаем график
    chart_3->removeAllSeries();

    // Удаляем все существующие оси
    QList<QAbstractAxis*> axes = chart_3->axes();
    for (int i = 0; i< axes.size(); ++i){
        QAbstractAxis *ax = axes.at(i);
        chart_3->removeAxis(ax);
        delete ax;
    }

    // Создаем новые оси
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    axisX->setTitleText("Lag");
    axisY->setTitleText("Автокорреляция");
    axisY->setRange(-1.0, 1.0);

    chart_3->addAxis(axisX, Qt::AlignBottom);
    chart_3->addAxis(axisY, Qt::AlignLeft);

    chart_3->setTitle("Автокорреляция данных");

}

//============================================================================================


void MainWindow::loadAutocorrelationData(const QStringList &filenames,   QChart *chart )
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

    createChart(allData, loadedFiles, chart);

    QString message = QString("Загружено %1 наборов данных:").arg(allData.size());

    // Используем итератор для QStringList
    for (int i = 0; i < loadedFiles.size(); ++i) {
        message += "\n- " + loadedFiles.at(i);
    }
    QMessageBox::information(this, "Успех", message);
}

void MainWindow::createChart(const QVector<QVector<QPointF>> &allData, const QStringList &filenames,  QChart *chart)
{

    // Очищаем старый график
    chart->removeAllSeries();

    // Удаляем все существующие оси
    QList<QAbstractAxis*> axes = chart->axes();
    for (int i = 0; i< axes.size(); ++i){
        QAbstractAxis *ax = axes.at(i);
        chart->removeAxis(ax);
        delete ax;
    }


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