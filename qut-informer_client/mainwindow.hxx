#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QDebug>
#include <QJsonObject>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QUdpSocket;
class QVideoWidget;
using VideoWidget = QVideoWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QUdpSocket *m_udp_socket;


  template <typename WidgetType>
  WidgetType* generate_base_widget(const QJsonValueRef &json)
    {
      qDebug() << "base widget created";

      WidgetType *new_widget = new WidgetType(ui->ui_container);

      auto w_x = json.toObject().take("x").toInt();
      auto w_y = json.toObject().take("y").toInt();
      auto w_width = json.toObject().take("width").toInt();
      auto w_height = json.toObject().take("height").toInt();

      new_widget->resize(w_width, w_height);
      new_widget->move(w_x, w_y);
      new_widget->show();
      return new_widget;
    }


public slots:
  void show_ui();
  void read_datagram();

private:
  Ui::MainWindow *ui;



};
#endif // MAINWINDOW_HXX
