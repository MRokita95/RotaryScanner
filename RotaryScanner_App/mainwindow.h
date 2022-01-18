#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtDebug>
#include <iostream>
#include <vector>
#include <string>
#include <QWidget>
#include <QMessageBox>
#include <QQuickWidget>
#include <QLabel>
#include <QSpinBox>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>
#include <QRadioButton>
#include <QPushButton>

#include "datacollector.h"


#define SingleMesureComm "SM_P"		//Single Measurement Position (...) ex: SM_P 90 (single mesurment at position 90 degrees)
#define MultMesureInRangeComm "MM_IR_P"		//Multiple Measurements In Range Position ex: MM_IR_P 10 90 2 (mesurments from 10 to 90 every 2 degrees)
#define FullMeasureComm "FM_P"		//Full Measurement (360 degrees) ex: FM_P 10 (mesurments from 0 to 360 every 10 degrees)
#define ReqDataComm "GET_DAT"           //Request for data from Arduino (Position + distance)
#define GoHomeComm "HOM"           //Go to 0.0 position (home)

#define write_data_to_file true     //parameter if write data to file are necessary

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void send_command(QString command);

    void on_singleModeRB_clicked();

    void on_rangedModeRB_clicked();

    void on_fullModeRB_clicked();

    void on_setPos_SM_valueChanged(int arg1);

    void on_connect_PB_clicked();

    void on_start_PB_clicked();

    void on_disconnect_PB_clicked();

    void read_data_from_port();

    void on_setStartPos_RM_valueChanged(int arg1);

    void on_setEndPos_RM_valueChanged(int arg1);

    void on_setStep_RM_valueChanged(int arg1);

    void on_setStep_FM_valueChanged(int arg1);

    void on_goHome_PB_clicked();

    void on_write_to_file_PB_clicked();

    void on_show_plot_PB_clicked();

    void show_button_to_plot();

    void hide_button_to_plot();

    void plot_data();


private:
    Ui::MainWindow *ui;

    std::vector<QWidget *> widgets_to_control;

    void hide_widgets(std::vector<QWidget *> &);

    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 10755;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;

    QString command_to_arduino = "";
    int set_startPos;
    int set_endPos;
    int set_step;

    double actual_position;
    int measured_distance;

    void request_data();
    int counter;

    QString file_name = "Pos_Dist_Data";
    DataCollector<typeof(actual_position), typeof(measured_distance)> *data_collection = nullptr;

    std::vector <QString> created_files;
    QRadioButton **file_RB;
    QPushButton *draw_plot_PB;
    QString check_selected_file();


    QString prepare_command(QString command, int endPos=0, int startPos=0, int step=0);

    void show_files();

};
#endif // MAINWINDOW_H
