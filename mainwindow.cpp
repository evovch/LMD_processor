#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QFileDialog>

#include "cls_LmdFile.h"

#include <fstream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::string readString;

    std::ifstream inputFile("/tmp/LMD_processor.txt");
    if (!inputFile.is_open()) return;

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    ui->lineEdit->setText(QString::fromStdString(readString.substr(4)));

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    ui->lineEdit_2->setText(QString::fromStdString(readString.substr(4)));

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    ui->lineEdit_3->setText(QString::fromStdString(readString.substr(4)));

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    this->fInputFolderPath = readString.substr(4);

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    this->fPedestalsFolderPath = readString.substr(4);

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    this->fOutputFolderPath = readString.substr(4);

}

MainWindow::~MainWindow()
{
    QString inputFilename = ui->lineEdit->text();
    QString pedestalFilename = ui->lineEdit_2->text();
    QString outputFilename = ui->lineEdit_3->text();

    std::ofstream outputFile("/tmp/LMD_processor.txt");

    outputFile << "IN: " << inputFilename.toStdString() << std::endl;
    outputFile << "PED:" << pedestalFilename.toStdString() << std::endl;
    outputFile << "OUT:" << outputFilename.toStdString() << std::endl;

    std::string tmpStr;
    size_t lastSlashPos;

    tmpStr = inputFilename.toStdString();
    lastSlashPos = tmpStr.rfind("/");
    fInputFolderPath = tmpStr.substr(0, lastSlashPos+1);
    outputFile << "INP:" << fInputFolderPath << std::endl;

    tmpStr = pedestalFilename.toStdString();
    lastSlashPos = tmpStr.rfind("/");
    fPedestalsFolderPath = tmpStr.substr(0, lastSlashPos+1);
    outputFile << "PDP:" << fPedestalsFolderPath << std::endl;

    tmpStr = outputFilename.toStdString();
    lastSlashPos = tmpStr.rfind("/");
    fOutputFolderPath = tmpStr.substr(0, lastSlashPos+1);
    outputFile << "OUP:" << fOutputFolderPath << std::endl;


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
    v_dial.setDirectory(QString::fromStdString(fInputFolderPath));

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
    v_dial.setDirectory(QString::fromStdString(fPedestalsFolderPath));

    QStringList v_fileNames;
    if (v_dial.exec()) {
        v_fileNames = v_dial.selectedFiles();
        ui->lineEdit_2->setText(v_fileNames.at(0));
    }
}

void MainWindow::SelectOutputFile(void)
{
    std::string generatedFilename = this->fOutputFolderPath + "analysis.root";
    QString v_fileName = QFileDialog::getSaveFileName(this, tr("Save analysis file"),
                               QString::fromStdString(generatedFilename),
                               tr("Root files (*.root)"));
    ui->lineEdit_3->setText(v_fileName);
}
