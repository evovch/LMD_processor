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

    // Input
    void SelectDataFile(void);
    void SelectPedestalsFile(void);
    void SelectPixelMapFile(void);
    void SelectEffCalibFile(void);
    void SelectGraphsFile(void);

    // Output
    void SelectOutputFile(void);
    void SelectTreeFile(void);
    void SelectCrossTalkFile(void);

private:
    Ui::MainWindow *ui;

    // Input
    std::string fInputFolderPath;
    std::string fPedestalsFolderPath;
    std::string fPixelMapFolderPath;
    std::string fEffCalFolderPath;
    std::string fGraphsFolderPath;

    // Output
    std::string fOutputFolderPath;

};

#endif // MAINWINDOW_H
