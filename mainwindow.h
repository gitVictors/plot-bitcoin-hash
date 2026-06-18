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
    void on_loadFileButton_clicked_1();
    void on_clearButton_clicked_1();
    void on_loadFileButton_clicked_2();
    void on_clearButton_clicked_2();
    void on_loadFileButton_clicked_3();
    void on_clearButton_clicked_3();

private:
    Ui::MainWindow *ui;
    void loadAutocorrelationData(const QStringList &filenames,  QChart *chart);
    void createChart(const QVector<QVector<QPointF>> &allData, const QStringList &filenames,  QChart *chart );
    QColor getColorForIndex(int index);

    QChart *chart_1;
    QChartView *chartView_1;
    QChart *chart_2;
    QChartView *chartView_2;
    QChart *chart_3;
    QChartView *chartView_3;


    QVector<QColor> colors;
};

#endif // MAINWINDOW_H