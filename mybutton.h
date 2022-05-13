#ifndef MYBUTTON_H
#define MYBUTTON_H
#include <QPushButton>
#include <QWidget>
#include <QString>

class MyButton : public QPushButton
{
    Q_OBJECT
private:
    QString image;
    QString pressImage;
public:
    //explicit MyButton(QWidget *parent = nullptr);

    MyButton(QString image, QString pressImage = "");
    void zoom(int offset1, int offset2);
signals:

};

#endif // MYBUTTON_H
