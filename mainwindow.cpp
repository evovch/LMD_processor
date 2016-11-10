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

    // -------------------------------------------------------------------
    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    ui->lineEdit->setText(QString::fromStdString(readString.substr(4)));

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    ui->lineEdit_2->setText(QString::fromStdString(readString.substr(4)));

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    ui->lineEdit_7->setText(QString::fromStdString(readString.substr(4)));

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    ui->lineEdit_4->setText(QString::fromStdString(readString.substr(4)));

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    ui->lineEdit_5->setText(QString::fromStdString(readString.substr(4)));

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    ui->lineEdit_3->setText(QString::fromStdString(readString.substr(4)));

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    ui->lineEdit_6->setText(QString::fromStdString(readString.substr(4)));

    // -------------------------------------------------------------------
    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    this->fInputFolderPath = readString.substr(4);

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    this->fPedestalsFolderPath = readString.substr(4);

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    this->fPixelMapFolderPath = readString.substr(4);

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    this->fEffCalFolderPath = readString.substr(4);

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    this->fGraphsFolderPath = readString.substr(4);

    getline(inputFile, readString);
    if (inputFile.eof() || readString.length()<4) { return; }
    this->fOutputFolderPath = readString.substr(4);
}

MainWindow::~MainWindow()
{
    QString inputFilename = ui->lineEdit->text();
    QString pedestalFilename = ui->lineEdit_2->text();
    QString pixelMapFilename = ui->lineEdit_7->text();
    QString effCalibFilename = ui->lineEdit_4->text();
    QString graphsFilename = ui->lineEdit_5->text();
    QString outputFilename = ui->lineEdit_3->text();
    QString outTreeFilename = ui->lineEdit_6->text();

    std::ofstream outputFile("/tmp/LMD_processor.txt");

    outputFile << "IN: " << inputFilename.toStdString() << std::endl;
    outputFile << "PED:" << pedestalFilename.toStdString() << std::endl;
    outputFile << "PXM:" << pixelMapFilename.toStdString() << std::endl;
    outputFile << "EFF:" << effCalibFilename.toStdString() << std::endl;
    outputFile << "GRA:" << graphsFilename.toStdString() << std::endl;
    outputFile << "OUT:" << outputFilename.toStdString() << std::endl;
    outputFile << "OTR:" << outTreeFilename.toStdString() << std::endl;

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

    tmpStr = pedestalFilename.toStdString();
    lastSlashPos = tmpStr.rfind("/");
    fPedestalsFolderPath = tmpStr.substr(0, lastSlashPos+1);
    outputFile << "PMP:" << fPixelMapFolderPath << std::endl;

    tmpStr = effCalibFilename.toStdString();
    lastSlashPos = tmpStr.rfind("/");
    fEffCalFolderPath = tmpStr.substr(0, lastSlashPos+1);
    outputFile << "EFP:" << fEffCalFolderPath << std::endl;

    tmpStr = graphsFilename.toStdString();
    lastSlashPos = tmpStr.rfind("/");
    fGraphsFolderPath = tmpStr.substr(0, lastSlashPos+1);
    outputFile << "GRP:" << fGraphsFolderPath << std::endl;

    tmpStr = outputFilename.toStdString();
    lastSlashPos = tmpStr.rfind("/");
    fOutputFolderPath = tmpStr.substr(0, lastSlashPos+1);
    outputFile << "OUP:" << fOutputFolderPath << std::endl;

    outputFile.close();

    delete ui;
}

void MainWindow::ImportFile(void)
{
    //TODO check
    ui->label->setText("Importing...");
    ui->label->update();

    QString v_filename = ui->lineEdit->text();
    QString v_pedFilename = ui->lineEdit_2->text();
    QString v_pixelMapFilename = ui->lineEdit_7->text();
    QString v_effCalibFilename = ui->lineEdit_4->text();
    QString v_graphsFilename = ui->lineEdit_5->text();

    cls_LmdFile* v_inputFile = new cls_LmdFile();
    v_inputFile->SetOutputHistoFile(ui->lineEdit_3->text());
    v_inputFile->SetOutputTreeFile(ui->lineEdit_6->text());

    v_inputFile->ImportPedestals(v_pedFilename);
    v_inputFile->ImportPixelMap(v_pixelMapFilename);
    //v_inputFile->ImportEffCalib(v_effCalibFilename);
    //v_inputFile->ImportGraphsFile(v_graphsFilename);

    v_inputFile->StartProcessing(v_filename);

    delete v_inputFile;

    ui->label->setText("Finished analysis.");
}

// ========================================================================================
// Input
// ========================================================================================

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

void MainWindow::SelectPixelMapFile(void)
{
    QFileDialog v_dial;
    v_dial.setFileMode(QFileDialog::ExistingFile);
    v_dial.setNameFilter(tr("Pixel map files (*.txt)"));
    v_dial.setDirectory(QString::fromStdString(fPixelMapFolderPath));

    QStringList v_fileNames;
    if (v_dial.exec()) {
        v_fileNames = v_dial.selectedFiles();
        ui->lineEdit_7->setText(v_fileNames.at(0));
    }
}

void MainWindow::SelectEffCalibFile(void)
{
    QFileDialog v_dial;
    v_dial.setFileMode(QFileDialog::ExistingFile);
    v_dial.setNameFilter(tr("Calibration files (*.par)"));
    v_dial.setDirectory("~");

    QStringList v_fileNames;
    if (v_dial.exec()) {
        v_fileNames = v_dial.selectedFiles();
        ui->lineEdit_4->setText(v_fileNames.at(0));
    }
}

void MainWindow::SelectGraphsFile(void)
{
    QFileDialog v_dial;
    v_dial.setFileMode(QFileDialog::ExistingFile);
    v_dial.setNameFilter(tr("Graphs files (*.root)"));
    v_dial.setDirectory("~");

    QStringList v_fileNames;
    if (v_dial.exec()) {
        v_fileNames = v_dial.selectedFiles();
        ui->lineEdit_5->setText(v_fileNames.at(0));
    }
}

// ========================================================================================
// Output
// ========================================================================================

void MainWindow::SelectOutputFile(void)
{
    std::string generatedFilename = this->fOutputFolderPath + "analysis.root";
    QString v_fileName = QFileDialog::getSaveFileName(this, tr("Save analysis file"),
                               QString::fromStdString(generatedFilename),
                               tr("Root files (*.root)"));
    ui->lineEdit_3->setText(v_fileName);
}

void MainWindow::SelectTreeFile(void)
{
    std::string generatedFilename = this->fOutputFolderPath + "tree.root";
    QString v_fileName = QFileDialog::getSaveFileName(this, tr("Save tree file"),
                               QString::fromStdString(generatedFilename),
                               tr("Root files (*.root)"));
    ui->lineEdit_6->setText(v_fileName);
}
