#include "myplot.h"

MyPlate::MyPlate(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.action1, SIGNAL(triggered()), this, SLOT(open_FileName_triggered()));
      //QVector<double> x(101), y(101); // initialize with entries 0..100
      //for (int i=0; i<101; ++i)
      //{
      //  x[i] = i/50.0 - 1; // x goes from -1 to 1
      //  y[i] = x[i]*x[i];  // let's plot a quadratic function
      //}
      //// create graph and assign data to it:
      //ui.customPlot->addGraph();
      //ui.customPlot->graph(0)->setData(x, y);
      //// give the axes some labels:
      //ui.customPlot->xAxis->setLabel("x");
      //ui.customPlot->yAxis->setLabel("y");
      //// set axes ranges, so we see all data:
      //ui.customPlot->xAxis->setRange(-1, 1);
      //ui.customPlot->yAxis->setRange(0, 1);
      //ui.customPlot->replot();
}

// 打开SAC文件
void MyPlate::open_FileName_triggered()
{

    QString filePath = QFileDialog::getOpenFileName(nullptr, "Select a SAC file", "", "All Files (*);;Text Files (*.txt)");
    if (!filePath.isEmpty()) {
        // 用户已选择了一个文件
        SACHEAD header;
        std::string stdStr = filePath.toStdString();
        QByteArray byteArray = filePath.toUtf8();
        const char* charData = byteArray.constData();
        float *a = read_sac(charData, &header);
        if (a != nullptr) {
            int numberOfElements = header.npts;
            //std::cout << header.npts << std::endl;
            QVector<double> sacVector;
            for (int i = 0; i < numberOfElements; ++i) {
                sacVector.append(static_cast<double>(a[i]));
            }
            delete[] a;
            /*.....................lineEdit...........................*/
            //显示文件名，绝对路径的最后一个元素
            //ui.label_filename->setText(filePath.split("/").back());
            ui.lineEdit_npts->setText(QString::number(header.npts));
            ui.lineEdit_delta->setText(QString::number(header.delta));
            ui.lineEdit_stla->setText(QString::number(header.stla));
            ui.lineEdit_stlo->setText(QString::number(header.stlo));
            MonthDay result = daysToMonthDay(header.nzyear, header.nzjday);
            QString start_time = QString::number(header.nzyear) +"年"+ QString::number(result.month) + "月"+QString::number(result.day)+"日"+ QString::number(header.nzhour)+"时"+ QString::number(header.nzmin)+"分"+ QString::number(header.nzsec)+"秒";
            ui.lineEdit_start_time->setText(start_time);
            int totalSec = header.npts * header.delta;
            Time convertedTime =secondsToDHMS(totalSec);
            QString sum_time = QString::number(convertedTime.days) + "天" + QString::number(convertedTime.hours) + "时" + QString::number(convertedTime.minutes) + "分" + QString::number(convertedTime.seconds) + "秒";
            ui.lineEdit_sum_time->setText(sum_time);


            /*.....................customPlot...........................*/

            // 添加图表
            ui.customPlot->addGraph();
            // 设置画笔颜色和刻度线
            ui.customPlot->graph()->setPen(QPen(Qt::black));
            ui.customPlot->xAxis2->setVisible(true);
            ui.customPlot->xAxis2->setTickLabels(false);
            ui.customPlot->yAxis2->setVisible(true);
            ui.customPlot->yAxis2->setTickLabels(false);
            double delta = double(header.delta);
            QVector<double> x(sacVector.size());
            double currentValue = 0.0;
            for (int i = 0; i < sacVector.size(); ++i) {
                x[i] = currentValue;
                currentValue += delta;
            }
            // 设置图表数据
            ui.customPlot->graph(0)->setData(x, sacVector);
            // 给坐标轴添加标签
            ui.customPlot->xAxis->setLabel("时间/s");
            ui.customPlot->yAxis->setLabel("幅值");
            //让范围自动缩放，以使图形完全适合可见区域：
            ui.customPlot->graph()->rescaleAxes();
            // 允许用户用鼠标拖动轴范围，用鼠标滚轮缩放，并通过点击选择图形：
            ui.customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
            // 更新显示
            ui.customPlot->replot();
        }
        else {
             QMessageBox::information(NULL, "Show", "Failed to open", QMessageBox::Ok);
             delete[] a;
             return;
        }
    }
    else {
        // 用户取消了选择或者发生了错误
        QMessageBox::information(NULL, "Show", "No files selected", QMessageBox::Ok);
    }
}

MyPlate::~MyPlate()
{}
