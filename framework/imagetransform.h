#ifndef FRAMEWORK_IMAGETRANSFORM_H_
#define FRAMEWORK_IMAGETRANSFORM_H_

#include "transform.h"

class ImageTransform : public Transform {
 public:
  explicit ImageTransform();
  //elplicit:其限定的其类的构造函数只能被显式调用(即不能使用"="进行赋值，要用构造函数赋值)
  explicit ImageTransform(const QPointF &pos);
  ImageTransform(const ImageTransform &) = delete;
  ImageTransform &operator=(const ImageTransform &) = delete;

  const QImage &getImage();
  bool setImage(const char *path);
  bool setImage(const QString &path);

  void setOffset(const QPointF &offset);
  void setAlignment(Qt::Alignment alignment);

 protected:
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget = nullptr) override;//系统自带

 protected:
  QImage image;
  QPointF offset;
  Qt::Alignment alignment;
  QRectF imageRect;

  void updateImageRect();
};

#endif  // FRAMEWORK_IMAGETRANSFORM_H_
