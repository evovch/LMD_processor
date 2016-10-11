#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void ImportFile(void);
    void ImportPedestals(void);

    void SelectDataFile(void);
    void SelectPedestalsFile(void);
    void SelectOutputFile(void);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
