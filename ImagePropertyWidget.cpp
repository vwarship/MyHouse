#include "ImagePropertyWidget.h"
#include "ui_ImagePropertyWidget.h"
#include <QDateTime>
#include <QPushButton>
#include <QDebug>
#include "MainWindow.h"
#include "Image.h"
#include "TableImagesUtil.h"

ImagePropertyWidget::ImagePropertyWidget(QWidget *parent) :
    QWidget(parent), imageId(0),
    ui(new Ui::ImagePropertyWidget)
{
    ui->setupUi(this);

    ui->tagButtons->setColumnMinimumWidth(0, 100);
    ui->tagButtons->setColumnMinimumWidth(1, 100);

    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->title, SIGNAL(textChanged(QString)), this, SLOT(enabledSaveButton()));
    connect(ui->description, SIGNAL(textChanged()), this, SLOT(enabledSaveButton()));
}

ImagePropertyWidget::~ImagePropertyWidget()
{
    delete ui;
}

void ImagePropertyWidget::setImage(Image *image)
{
    if (!image)
        return;

    imageId = image->id();
    ui->title->setText( image->title() );
    ui->createDate->setText( image->createTime() );
    ui->description->setText( image->description() );

    ui->saveButton->setEnabled(false);
}

void ImagePropertyWidget::save()
{
    QString title = ui->title->text();
    QString description = ui->description->toPlainText();
    TableImagesUtil::update(imageId, title, description);

    ui->saveButton->setEnabled(false);
}

void ImagePropertyWidget::enabledSaveButton()
{
    ui->saveButton->setEnabled(true);
}
