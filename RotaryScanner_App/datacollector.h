#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <vector>
#include <functional>
#include <string>
#include <unordered_map>
#include <map>
#include<fstream>
#include <QScopedPointer>
#include <QString>
#include <QTextStream>
#include <QtCore>
#include <QDebug>
#include <iostream>

//template class for data collection and file handling
template <typename Pos_t, typename Dist_t>
class DataCollector
{
private:
    //file path for written files
    const QString file_path = "C:\\QT_Apps\\Datafiles\\";

    //filename created during constructing object
    QString filename;

    //static member for calculating created file (and adds its number to filename)
    static int file_cnt;

    //data collection (key: pos, val: dist)
    std::map <Pos_t, Dist_t> data;

public:
    DataCollector<Pos_t, Dist_t>(QString);

    //returns created filename
    QString get_filename();

    //insert data (pos + dist) to data collector
    void put_data(Pos_t, Dist_t);

    //write data file with propper column description
    void write_to_file(QString, QString);
};


template <typename Pos_t, typename Dist_t>
int DataCollector<Pos_t, Dist_t>::file_cnt = 0;

template <typename Pos_t, typename Dist_t>
DataCollector<Pos_t, Dist_t>::DataCollector(QString f_name)
{
    file_cnt++;

    filename = file_path + f_name+ "_" + QString::number(file_cnt) + ".txt";
}

template <typename Pos_t, typename Dist_t>
void DataCollector<Pos_t, Dist_t>::put_data(Pos_t position, Dist_t distance)
{
    data.insert({position, distance} );
}

template <typename Pos_t, typename Dist_t>
void DataCollector<Pos_t, Dist_t>::write_to_file(QString param1_name, QString param2_name)
{   

    int white_spaces = 10;

    QFile *file = new QFile(filename);

    if (file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(file);

        stream << QString ("%1%2\n").arg(param1_name, -white_spaces).arg(param2_name, -white_spaces);

        for (auto itr = data.begin(); itr != data.end(); ++itr)
        {
            stream << QString ("%1%2\n").arg(itr->first, -white_spaces).arg(itr->second, -white_spaces);
        }

        file->close();

        qDebug()<<"File written";
    }
}

template <typename Pos_t, typename Dist_t>
QString DataCollector<Pos_t, Dist_t>::get_filename()
{
    return filename;
}

#endif // DATACOLLECTOR_H
