#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QRegularExpression>
#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QVector>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadFileButton_clicked();
    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;
    void loadAutocorrelationData(const QStringList &filenames);
    void createChart(const QVector<QVector<QPointF>> &allData, const QStringList &filenames);
    QColor getColorForIndex(int index);

    QChart *chart;
    QChartView *chartView;
    QVector<QColor> colors;
};

#endif // MAINWINDOW_H