#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <string>

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

    void SelectDataFile(void);
    void SelectPedestalsFile(void);
    void SelectOutputFile(void);

private:
    Ui::MainWindow *ui;

    std::string fInputFolderPath;
    std::string fPedestalsFolderPath;
    std::string fOutputFolderPath;
};

#endif // MAINWINDOW_H
