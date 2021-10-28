#ifndef UICONTAINER_HXX
#define UICONTAINER_HXX

#include <QWidget>
#include <QGroupBox>
#include <tuple>
#include <QDebug>
class QRandomGenerator;
class QGraphicsView;
class QVideoWidget;
using Container = QWidget;

class UIContainer : public QWidget
{
  Q_OBJECT
public:
  UIContainer(QWidget *parent = nullptr);

  QString ui_data();

public slots:
  void generate_text();
  void generate_video();
  void generate_picture();

private:
  std::tuple<int,int,int,int> child_widget_size();
  QVector<std::tuple<QGraphicsView *, QString>> m_picture;
  QVector<std::tuple<QVideoWidget *, QString>> m_video;

  template <typename WidgetType>
  WidgetType* generate_base_widget()
    {
      qDebug() << "base widget created";
      WidgetType *new_widget = new WidgetType(this);
      int w_x, w_y, w_wigth, w_height;
      std::tie(w_x, w_y, w_wigth, w_height) = child_widget_size();

      new_widget->resize(w_wigth, w_height);
      new_widget->move(w_x, w_y);
      new_widget->show();
      return new_widget;
    }



signals:

};

#endif // UICONTAINER_HXX
