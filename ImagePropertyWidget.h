#ifndef IMAGEPROPERTYWIDGET_H
#define IMAGEPROPERTYWIDGET_H

#include <QWidget>

namespace Ui {
class ImagePropertyWidget;
}

class Image;
class ImagePropertyWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImagePropertyWidget(QWidget *parent = 0);
    ~ImagePropertyWidget();
    
    void setImage(Image *image);

public slots:
    void save();
    void enabledSaveButton();

private:
    Ui::ImagePropertyWidget *ui;

    int imageId;

};

#endif // IMAGEPROPERTYWIDGET_H
