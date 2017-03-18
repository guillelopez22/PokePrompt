#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <time.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "btree.h"
#include "btreenode.h"
#include <stdio.h>
#include <string>

using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plainTextEdit->setStyleSheet("color: white; Background-color: black");
    ui->pushButton->setStyleSheet("color: white; Background-color: black");
    ui->lineEdit->setStyleSheet("color: white; Background-color: black");

    DIR *dp;
    dirent *d;
    vector<string> *vec = new vector<string>;
    dp = opendir("C:/pokeos");
    while((d = readdir(dp)) != NULL)
        vec->push_back(d->d_name);
    string test ="";
    sort(vec->begin(), vec->end());

    const int AMOUNT = vec->size();
    const int MAX = AMOUNT;

    int value[AMOUNT];

    srand(time(NULL));


    for (int i=0;i<AMOUNT;i++)
    {
        bool check;
        int n;
        do
        {
        n=rand()%MAX;

        check=true;
        for (int j=0;j<i;j++)
            if (n == value[j])
            {
                check=false;
                break;
            }
        } while (!check);
        value[i]=n;
    }
    for(int i = 0; i < vec->size(); i++)
         test =test + std::to_string(value[i]) + ":" + vec->at(i)+"\n";
      ofstream myfile;
      myfile.open ("C:/pokeos/sys.txt");
      myfile << test;
      myfile.close();
      wchar_t* fileLPCWSTR = L"C:/pokeos/sys.txt";
      DWORD attributes = GetFileAttributes(fileLPCWSTR);
      SetFileAttributes(fileLPCWSTR, attributes + FILE_ATTRIBUTE_HIDDEN);
      ifstream file("C:/pokeos/sys.txt");
      string str;
      string token;
      vector<string> v;

      while (std::getline(file, str)){
          while(token != str){
            token = str.substr(0,str.find_first_of(":"));
            str = str.substr(str.find_first_of(":") + 1);
            v.push_back(token);
          }
      }
      int a;
      string b;
      BTree* tree = new BTree(3);
      for(int i = 0; i < v.size(); i++){
          if(i==0 || i%2 == 0){
              a = std::stoi(v.at(i));
          }else{
              b = v.at(i);
          }
          tree->insert(a);
      }




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(){
    QString qs = ui->lineEdit->text();
    string data;
    QString data1;

    string cmd = qs.toUtf8().constData();
    ui->lineEdit->setText("");
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");
    stream = popen(cmd.c_str(), "r");
    if(strstr(qs.toUtf8().constData(),"cd")){
        string pathname = qs.toUtf8().constData();
        pathname.erase(0,2);

    }
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
        data1 = QString::fromStdString(data);
        ui->plainTextEdit->appendPlainText(data1);
    }
    ui->plainTextEdit->appendPlainText("------------------------------------");

}


void MainWindow::on_lineEdit_returnPressed()
{
    string data;
    QString data1;
    QString qs = ui->lineEdit->text();
    string cmd = qs.toUtf8().constData();
    ui->lineEdit->setText("");
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");
    stream = popen(cmd.c_str(), "r");
    if(strstr(qs.toUtf8().constData(),"cd")){
        string pathname = qs.toUtf8().constData();
        pathname.erase(0,2);
        chdir(pathname.c_str());
    }
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
        data1 = QString::fromStdString(data);
        ui->plainTextEdit->appendPlainText(data1);
    }
    ui->plainTextEdit->appendPlainText("------------------------------------");
}
