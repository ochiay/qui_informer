#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QUdpSocket;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QUdpSocket *m_udp_socket;
public slots:
  void send_datagram();
  void get_request();
private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HXX
