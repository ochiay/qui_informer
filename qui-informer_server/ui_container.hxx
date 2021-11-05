#ifndef UICONTAINER_HXX
#define UICONTAINER_HXX

#include <QWidget>
#include <QGroupBox>
#include <tuple>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

class QRandomGenerator;
class QGraphicsView;
class QVideoWidget;

using Container = QWidget;
using VideoWidget = QVideoWidget;

class UIContainer : public QWidget
{
  Q_OBJECT
public:
  UIContainer(QWidget *parent = nullptr);

  QJsonDocument m_json;

public slots:

  void generate_text();
  void generate_video();
  void generate_picture();

signals:
  void json_updated(QString &json_str);

private:
  QJsonObject json_obj;

  std::tuple<int,int,int,int> child_widget_size();

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

  void add_coord(QJsonObject &obj, QWidget &widget);


};

#endif // UICONTAINER_HXX
