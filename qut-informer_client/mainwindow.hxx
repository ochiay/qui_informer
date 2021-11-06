#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>
#include <QDebug>
#include <QJsonObject>
#include "ui_mainwindow.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QUdpSocket;
class QVideoWidget;
class QGraphicsPixmapItem;
class QGraphicsView;
class QGraphicsScene;
using VideoWidget = QVideoWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QUdpSocket *m_udp_socket;


  QVector <QGraphicsScene *> e_noe_scenes;
  QVector <QGraphicsView *> e_noe_views;
  template <typename TypeForRemoving>
  void delete_all(const QWidget *parent)
    {
      QList <TypeForRemoving *> deleting = parent->findChildren<TypeForRemoving *>();
      for (auto d: deleting)
        delete d;
      deleting.clear();
    }


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
