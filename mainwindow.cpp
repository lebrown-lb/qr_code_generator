#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QColorDialog>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->generate_pb, &QPushButton::clicked, this, &MainWindow::generate);
    connect(ui->save_pb, &QPushButton::clicked, this, &MainWindow::save);
    connect(ui->light_color_pb, &QPushButton::clicked, this, &MainWindow::lightColorChange);
    connect(ui->dark_color_pb, &QPushButton::clicked, this, &MainWindow::darkColorChange);
    connect(ui->border_sbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::borderChange);
    connect(ui->scale_sbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::scaleChange);
    connect(ui->ec_low, &QRadioButton::toggled, this, &MainWindow::errorCorrectionChange);
    connect(ui->ec_medium, &QRadioButton::toggled, this, &MainWindow::errorCorrectionChange);
    connect(ui->ec_quartile, &QRadioButton::toggled, this, &MainWindow::errorCorrectionChange);
    connect(ui->ec_high, &QRadioButton::toggled, this, &MainWindow::errorCorrectionChange);

    connect(ui->image, SIGNAL(lightColorChange(QColor)),this,SLOT(lightColorUpdate(QColor)));
    connect(ui->image, SIGNAL(darkColorChange(QColor)),this,SLOT(darkColorUpdate(QColor)));

    m_light.setRgb(255,255,255);
    m_dark.setRgb(0,0,0);
    setPushButtonColor(ui->light_color_pb,m_light);
    setPushButtonColor(ui->dark_color_pb,m_dark);

    QList<int> s0, s1;

    s0 << 400 << 0;
    s1 << 400 << 100;
    ui->splitter->setSizes(s0);
    ui->splitter_2->setSizes(s1);


    ui->ec_low->setChecked(true);
    ui->border_sbox->setValue(m_border);
    ui->scale_sbox->setValue(m_modulePixelCount);
    ui->border_sbox->setMinimum(1);
    ui->scale_sbox->setMinimum(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generate()
{
    std::string text = ui->textEdit->toPlainText().toStdString();

    if(text == "")
    {
        std::cout << "NO TEXT ENTERED" << std::endl;
        return;
    }


    // Make and print the QR Code symbol
    const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(text.data(), m_errCorLvl);

    if(ui->logo_cbox->isChecked())
        displyQr(qr,true);
    else
        displyQr(qr,false);
    //printQr(qr);


}

void MainWindow::save()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("SAVE FILE"), nullptr);
    std::string file = filename.toStdString();
    size_t idx = file.find_last_of(".");

    if(idx == std::string::npos)
    {
        file += ".png";
    }

    m_qrImage.save(file.data());

    std::cout << "FILENAME:" << filename.toStdString() << std::endl;
}

void MainWindow::borderChange()
{
    m_border = ui->border_sbox->value();
}

void MainWindow::scaleChange()
{
    m_modulePixelCount = ui->scale_sbox->value();
}

void MainWindow::lightColorChange()
{
    QColor color = QColorDialog::getColor(Qt::yellow, this );
    if( color.isValid() )
    {
        m_light = color;
        setPushButtonColor(ui->light_color_pb,m_light);

    }


}

void MainWindow::darkColorChange()
{
    QColor color = QColorDialog::getColor(Qt::yellow, this );
    if( color.isValid() )
    {
        m_dark = color;
        setPushButtonColor(ui->dark_color_pb,m_dark);
    }

}

void MainWindow::lightColorUpdate(QColor c)
{
    m_light = c;
    setPushButtonColor(ui->light_color_pb,m_light);
}

void MainWindow::darkColorUpdate(QColor c)
{
    m_dark = c;
    setPushButtonColor(ui->dark_color_pb,m_dark);

}

void MainWindow::errorCorrectionChange()
{
    if(ui->ec_low->isChecked())
        m_errCorLvl = qrcodegen::QrCode::Ecc::LOW;
    else if(ui->ec_medium->isChecked())
        m_errCorLvl = qrcodegen::QrCode::Ecc::MEDIUM;
    else if(ui->ec_quartile->isChecked())
        m_errCorLvl = qrcodegen::QrCode::Ecc::QUARTILE;
    else if(ui->ec_high->isChecked())
        m_errCorLvl = qrcodegen::QrCode::Ecc::HIGH;

}

void MainWindow::setPushButtonColor(QPushButton *pb, QColor c)
{
    QPalette pal = pb->palette();
    pal.setColor(QPalette::Button, c);
    pb->setAutoFillBackground(true);
    pb->setPalette(pal);
    pb->update();

}

void MainWindow::printQr(const qrcodegen::QrCode &qr)
{
    int border = 4;
    for (int y = -border; y < qr.getSize() + border; y++) {
        for (int x = -border; x < qr.getSize() + border; x++) {
            std::cout << (qr.getModule(x, y) ? "##" : "  ");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;


}

void MainWindow::displyQr(const qrcodegen::QrCode &qr, bool logo)
{

    // calculate image size
    size_t size = (qr.getSize() + 2 * m_border) * m_modulePixelCount;

    std::cout << "size:" << size << std::endl;

    QImage image(QSize(size,size),QImage::Format_RGB32);
    QPainter painter(&image);


    painter.setBrush(QBrush(m_light));
    painter.fillRect(QRectF(0,0,size,size),m_light);

    painter.setBrush(QBrush(m_dark));

    for(int y = 0; y < qr.getSize(); y++)
    {
        for(int x = 0; x < qr.getSize(); x++)
        {
            int xo = m_border * m_modulePixelCount +(x * m_modulePixelCount);
            int yo = m_border * m_modulePixelCount +(y * m_modulePixelCount);
            if(qr.getModule(x,y))
            {
                painter.fillRect(QRectF(xo,yo,m_modulePixelCount,m_modulePixelCount),m_dark);

            }
        }

    }

    if(logo)
    {

        //calculate logo location
        int w = 8 * m_modulePixelCount;
        int ld = (m_border * m_modulePixelCount + ((qr.getSize() * m_modulePixelCount)/2)) - w/2;

        if(ui->image_cbox->isChecked() && !ui->image->m_selectedImage.isNull())
        {
            QImage tmp = ui->image->m_selectedImage.scaled(w,w,Qt::KeepAspectRatio);
            painter.drawImage(ld, ld, tmp);
        }
        else if(!ui->image->m_image.isNull())
        {
            QImage tmp = ui->image->m_image.scaled(w,w,Qt::KeepAspectRatio);
            painter.drawImage(ld, ld, tmp);
        }



    }

    QPixmap myPixmap = QPixmap::fromImage(image);
    ui->display->setPixmap(myPixmap);
    m_qrImage = image;

}
