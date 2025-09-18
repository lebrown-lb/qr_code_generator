#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qrcodegen.hpp"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
class QPushButton;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void generate(void);
    void save(void);
    void borderChange(void);
    void scaleChange(void);
    void lightColorChange();
    void darkColorChange();
    void lightColorUpdate(QColor c);
    void darkColorUpdate(QColor c);
    void errorCorrectionChange(void);


protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setPushButtonColor(QPushButton *pb, QColor c);
    void printQr(const qrcodegen::QrCode &qr);
    void displyQr(const qrcodegen::QrCode &qr, bool logo);
    Ui::MainWindow *ui;
    size_t m_border = 4;
    size_t m_modulePixelCount = 10;
    QColor m_light;
    QColor m_dark;
    qrcodegen::QrCode::Ecc m_errCorLvl = qrcodegen::QrCode::Ecc::LOW;
    QImage m_qrImage;
};
#endif // MAINWINDOW_H
