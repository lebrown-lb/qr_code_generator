#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>
#include <QMenu>
#include <QImage>
#include <QMouseEvent>

class ImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ImageLabel(QWidget *parent = nullptr);
    explicit ImageLabel(const QString &text, QWidget *parent = nullptr);
    QImage m_image;
    QImage m_selectedImage;

signals:
    void lightColorChange(QColor c);
    void darkColorChange(QColor c);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void loadImage(void);
    void logoBounds(void);
    void copyLightColor(void);
    void copyDarkColor(void);

private:
    void buildContextMenu(void);
    void displaySelectionBox(QPoint p);
    QMenu * m_contextMenu = nullptr;
    QAction * m_action0 = nullptr;
    QAction * m_action1 = nullptr;
    QAction * m_action2 = nullptr;
    QAction * m_action3 = nullptr;
    bool m_selectFlg = false;
    bool m_clickFlg = false;
    QPoint m_p0;
    QPoint m_p1;
    QPoint m_menuPos;
};

#endif // IMAGELABEL_H
