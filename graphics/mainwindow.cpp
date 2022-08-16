#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::makePlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makePlot()
{
    QFile file("/home/artem/src/cpp files/DFT/sound_in_the_frequency_domain.txt");
    //QFile file("/home/artem/src/cpp files/DFT/sound_in_the_time_domain.txt");
    // С помощью метода open() открываем файл в режиме чтения
    if (!file.open(QIODevice::ReadOnly)) {
       qWarning("Cannot open file for reading"); // если файл не найден, то выводим предупреждение и завершаем выполнение программы
       return 1;
    }
    // Создаем входящий поток, из которого будут считываться данные, и связываем его с нашим файлом
    QTextStream in(&file);
    QVector<double> y;
    QVector<double> x;
    double yMin=1.7e+308, yMax=1.7e-308;
    // заполняем x и y данными
    int i = 0;
    // Считываем файл строка за строкой
    while(!in.atEnd()){// метод atEnd() возвращает true, если в потоке больше нет данных для чтения
        QString line = in.readLine();// метод readLine() считывает одну строку из потока
        if(line.toDouble()<yMin)
            yMin = line.toDouble();
        if(line.toDouble()>yMax)
            yMax=line.toDouble();
        x.push_back(i);
        y.push_back(line.toDouble());
        i++;
    }
    // создаем график и добавляем данные
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x, y);
    // задаем имена осей координат
    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");
    // задаем размеры осей
    ui->widget->xAxis->setRange(0, i);
    ui->widget->yAxis->setRange(yMin, yMax);
    ui->widget->replot();
}

