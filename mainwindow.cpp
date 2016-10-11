#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QFileDialog>

#include "cls_LmdFile.h"

#include <fstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::string readString;

    std::ifstream inputFile("/tmp/LMD_processor.txt");
    inputFile >> readString;
    ui->lineEdit->setText(QString::fromStdString(readString));
    inputFile >> readString;
    ui->lineEdit_2->setText(QString::fromStdString(readString));
    inputFile >> readString;
    ui->lineEdit_3->setText(QString::fromStdString(readString));
}

MainWindow::~MainWindow()
{
    QString inputFilename = ui->lineEdit->text();
    QString pedestalFilename = ui->lineEdit_2->text();
    QString outputFilename = ui->lineEdit_3->text();

    std::ofstream outputFile("/tmp/LMD_processor.txt");
    outputFile << inputFilename.toStdString() << std::endl;
    outputFile << pedestalFilename.toStdString() << std::endl;
    outputFile << outputFilename.toStdString() << std::endl;

    outputFile.close();

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

    ui->label->setText("Finished analysis.");
}

void MainWindow::SelectDataFile(void)
{
    QFileDialog v_dial;
    v_dial.setFileMode(QFileDialog::ExistingFile);
    v_dial.setNameFilter(tr("Data files (*.lmd)"));
    v_dial.setDirectory("~");

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
    v_dial.setDirectory("~");

    QStringList v_fileNames;
    if (v_dial.exec()) {
        v_fileNames = v_dial.selectedFiles();
        ui->lineEdit_2->setText(v_fileNames.at(0));
    }
}

void MainWindow::SelectOutputFile(void)
{
    QString v_fileName = QFileDialog::getSaveFileName(this, tr("Save analysis file"),
                               "~/analysis.root",
                               tr("Root files (*.root)"));
    ui->lineEdit_3->setText(v_fileName);
}
