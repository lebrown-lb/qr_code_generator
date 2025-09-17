#include "imagelabel.h"
#include <iostream>
#include <QFileDialog>
#include <QPainter>
#include <QPixmap>

ImageLabel::ImageLabel(QWidget *parent)
    : QLabel(parent)
{
    setMouseTracking(true); // Enable mouse tracking for this label

    if(m_contextMenu == nullptr)
        buildContextMenu();

}

ImageLabel::ImageLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{
    setMouseTracking(true); // Enable mouse tracking for this label

    if(m_contextMenu == nullptr)
        buildContextMenu();

}

void ImageLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        //emit clicked(); // Emit the custom clicked signal
        if(m_selectFlg)
        {
            if(!m_clickFlg)
            {
                m_p0 = event->pos();
                m_clickFlg = true;
            }
            else
            {
                m_p1 = event->pos();
                m_selectFlg = false;
                m_clickFlg = false;
                std::cout << "P0:[" << m_p0.x() << "," << m_p0.y() << "] P1:[" << m_p1.x() << "," << m_p1.y() << "]" << std::endl;
                if(!m_image.isNull())
                {
                    int width = m_p1.x() - m_p0.x();
                    int height = m_p1.y() - m_p0.y();
                    QRect subset(m_p0.x(), m_p0.y(), width, height);

                    m_selectedImage = m_image.copy(subset);
                }
            }
        }
        std::cout << "[CLICKED]"  << std::endl;

    }
    else if (event->button() == Qt::RightButton)
    {
        m_menuPos = event->pos();
        m_contextMenu->exec(mapToGlobal(event->pos()));
        m_contextMenu->show();
    }
    QLabel::mousePressEvent(event); // Call base class implementation

}

void ImageLabel::mouseMoveEvent(QMouseEvent *event)
{
    //QPoint p = event->pos();
    //std::cout << "[" << p.x() << "," << p.y() << "]" << std::endl;
    if(m_selectFlg && m_clickFlg && !m_image.isNull())
    {
        displaySelectionBox(event->pos());
    }
    QLabel::mouseMoveEvent(event); // Call the base class implementation

}

void ImageLabel::loadImage()
{
     QString filename = QFileDialog::getOpenFileName(this, tr("OPEN FILE"), nullptr, nullptr);

    m_image = QImage(filename);

     QPixmap myPixmap = QPixmap::fromImage(m_image);
     setPixmap(myPixmap);

}

void ImageLabel::logoBounds()
{
    m_selectFlg = true;

}

void ImageLabel::copyLightColor()
{
    if(!m_image.isNull())
    {
        emit lightColorChange(m_image.pixelColor(m_menuPos.x(), m_menuPos.y()));
    }

}

void ImageLabel::copyDarkColor()
{
    if(!m_image.isNull())
    {
        emit darkColorChange(m_image.pixelColor(m_menuPos.x(), m_menuPos.y()));
    }
}

void ImageLabel::buildContextMenu()
{
    m_contextMenu = new QMenu(tr("Context menu"), this);
    m_action0 = new QAction("LOAD IMAGE", this);
    m_action1 = new QAction("PLACE LOGO BOUNDS ", this);
    m_action2 = new QAction("Copy Color (Light)", this);
    m_action3 = new QAction("Copy Color (Dark)", this);

    m_contextMenu->addAction(m_action0);
    m_contextMenu->addAction(m_action1);
    m_contextMenu->addAction(m_action2);
    m_contextMenu->addAction(m_action3);

    connect(m_action0, &QAction::triggered, this, &ImageLabel::loadImage);
    connect(m_action1, &QAction::triggered, this, &ImageLabel::logoBounds);
    connect(m_action2, &QAction::triggered, this, &ImageLabel::copyLightColor);
    connect(m_action3, &QAction::triggered, this, &ImageLabel::copyDarkColor);
}

void ImageLabel::displaySelectionBox(QPoint p)
{
    QImage image = m_image;
    QPainter painter(&image);

    QPen pen(Qt::red, 2, Qt::SolidLine); // Black pen, 2 pixels wide, solid line
    QBrush brush(Qt::NoBrush);
    painter.setPen(pen);
    painter.setBrush(brush);
    int width = p.x() - m_p0.x();
    int height = p.y() - m_p0.y();

    painter.drawRect(m_p0.x(), m_p0.y(), width, height);

    QPixmap myPixmap = QPixmap::fromImage(image);
    setPixmap(myPixmap);

}
