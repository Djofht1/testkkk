#include "connection.h"
#include <QSqlError>
#include <QMessageBox>
#include <QtDebug>

Connection::Connection() {}

bool Connection::createconnect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("2a");
    db.setUserName("jawher");
    db.setPassword("esprit18");

    if (db.open()) {
        return true;
    } else {
        qDebug() << "❌ Erreur de connexion : " << db.lastError().text();
        QMessageBox::critical(nullptr, QObject::tr("Database Connection"),
                              QObject::tr("Connection failed: %1.\nClick OK to continue.")
                                  .arg(db.lastError().text()), QMessageBox::Ok);
        return false;  // Ne pas quitter l'application
    }

}



























/*#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("jawher");
    db.setPassword("esprit18");

    if (db.open()) {
        test = true;
        qDebug() << "Connexion réussie à la base de données.";
    } else {
        qDebug() << "Erreur de connexion : " << db.lastError().text();
        QMessageBox::critical(nullptr, QObject::tr("Database Connection"),
                              QObject::tr("Connection failed: %1.\nClick Cancel to exit.")
                                  .arg(db.lastError().text()), QMessageBox::Cancel);
    }
    return test;
}*/
