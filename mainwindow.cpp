#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QFileDialog>

#include "cls_LmdFile.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ImportFile(void)
{
    ui->label->setText("Importing...");
    ui->label->update();

    QString v_filename = ui->lineEdit->text();
    QString v_pedFilename = ui->lineEdit_2->text();

    cls_LmdFile* v_inputFile = new cls_LmdFile();
    v_inputFile->SetOutputAnalysisFile(ui->lineEdit_3->text());

    v_inputFile->ImportPedestals(v_pedFilename);
    v_inputFile->StartProcessing(v_filename);

    delete v_inputFile;

    ui->label->setText("Imported. Ready.");
}

void MainWindow::ImportPedestals(void)
{
/*obsolete*/
}

void MainWindow::SelectDataFile(void)
{
    QFileDialog v_dial;
    v_dial.setFileMode(QFileDialog::ExistingFile);
    v_dial.setNameFilter(tr("Data files (*.lmd)"));
    v_dial.setDirectory("/home/evovch/Documents/NeuRad_tests_data/");

    QStringList v_fileNames;
    if (v_dial.exec()) {
        v_fileNames = v_dial.selectedFiles();
        ui->lineEdit->setText(v_fileNames.at(0));
    }
}

void MainWindow::SelectPedestalsFile(void)
{
    QFileDialog v_dial;
    v_dial.setFileMode(QFileDialog::ExistingFile);
    v_dial.setNameFilter(tr("Pedestal files (*.dat)"));
    v_dial.setDirectory("/home/evovch/Documents/NeuRad_tests_data/");

    QStringList v_fileNames;
    if (v_dial.exec()) {
        v_fileNames = v_dial.selectedFiles();
        ui->lineEdit_2->setText(v_fileNames.at(0));
    }
}

void MainWindow::SelectOutputFile(void)
{
    QString v_fileName = QFileDialog::getSaveFileName(this, tr("Save analysis file"),
                               "/home/evovch/Documents/NeuRad_tests_data/analysis.root",
                               tr("Root files (*.root)"));
    ui->lineEdit_3->setText(v_fileName);
}
