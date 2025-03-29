#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "analyse.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_supprimerL_clicked();   // Delete record
    void on_modifierL_clicked();    // Modify record
    void on_ajouterL_clicked();     // Add record
    void on_tableView_clicked(const QModelIndex &index);
    bool validateDate(const QString &date);
    bool validateText(const QString &text);
    void on_exportPDFButton_clicked();
    void rechercherParPatient();
     //void trierAnalyses();
    void trierParDuree();
     void on_statButton_clicked();
   // bool validateEmail(const QString &email);
     bool validateEmail(const QString &email) ;
     void on_envoyerEmailButton_clicked();

private:
    Ui::MainWindow *ui;
    Analyse a;

};
#endif // MAINWINDOW_H
