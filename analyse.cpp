#include "analyse.h"
#include <QSqlError>
#include<QtDebug>
#include <QSqlQuery>
Analyse::Analyse() {

}

Analyse::Analyse(int id_analyse, QString patient, QString nom_analyse,
                 QString type, QString caracteristique, QString medecin,
                 QString duree, QString resultat) {
    this->id_analyse = id_analyse;
    this->patient = patient;
    this->nom_analyse = nom_analyse;
    this->type = type;
    this->caracteristique = caracteristique;
    this->medecin = medecin;
    this->duree = duree;
    this->resultat = resultat;
}

// Validation Methods
// Validation Methods
bool validerId(int id) { return id > 0; }
bool validerNom(QString nom) { return QRegularExpression("^[A-Za-z ]+$").match(nom).hasMatch(); }
bool validerType(QString type) { return QRegularExpression("^[A-Za-z ]+$").match(type).hasMatch(); }
bool validerCaracteristiques(QString caract) { return QRegularExpression("^[A-Za-z0-9 ]*$").match(caract).hasMatch(); }
bool validerDuree(QString duree) { return duree.toInt() > 0; }
bool validerResultat(QString resultat) { return !resultat.isEmpty(); }

// Getters
int Analyse::get_id_analyse() {
    return id_analyse;
}

QString Analyse::get_patient() {
    return patient;
}

QString Analyse::get_nom_analyse() {
    return nom_analyse;
}

QString Analyse::get_type() {
    return type;
}

QString Analyse::get_caracteristique() {
    return caracteristique;
}

QString Analyse::get_medecin() {
    return medecin;
}

QString Analyse::get_duree() {
    return duree;
}

QString Analyse::get_resultat() {
    return resultat;
}

// Setters
void Analyse::set_id_analyse(int id_analyse) {
    this->id_analyse = id_analyse;
}

void Analyse::set_patient(QString patient) {
    this->patient = patient;
}

void Analyse::set_nom_analyse(QString nom_analyse) {
    this->nom_analyse = nom_analyse;
}

void Analyse::set_type(QString type) {
    this->type = type;
}

void Analyse::set_caracteristique(QString caracteristique) {
    this->caracteristique = caracteristique;
}

void Analyse::set_medecin(QString medecin) {
    this->medecin = medecin;
}

void Analyse::set_duree(QString duree) {
    this->duree = duree;
}

void Analyse::set_resultat(QString resultat) {
    this->resultat = resultat;
}

// CRUD Methods
bool Analyse::ajouter_analyse() {
    QSqlQuery query;

    query.prepare("INSERT INTO ANALYSE (id_analyse, patient, nom_analyse, type, caracteristique, medecin, duree, resultat) "
                  "VALUES (:id_analyse, :patient, :nom_analyse, :type, :caracteristique, :medecin, :duree, :resultat)");
    query.bindValue(":id_analyse", id_analyse);
    query.bindValue(":patient", patient);
    query.bindValue(":nom_analyse", nom_analyse);
    query.bindValue(":type", type);
    query.bindValue(":caracteristique", caracteristique);
    query.bindValue(":medecin", medecin);
    query.bindValue(":duree", duree);
    query.bindValue(":resultat", resultat);
    return query.exec();
}

QSqlQueryModel* Analyse::afficher_analyse() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM analyse");
    return model;
}

QSqlQuery Analyse::get_analyse() {
    QSqlQuery query("SELECT PATIENT, NOM_ANALYSE, TYPE, CARACTERISTIQUE, MEDECIN, DUREE, RESULTAT");
    return query;
}


bool Analyse::modifier_analyse(int id, QString patient, QString nom_analyse, QString type,
                               QString caracteristique, QString medecin, QString duree, QString resultat) {
    QSqlQuery query;
    QString updateQuery = "UPDATE analyse SET ";
    bool first = true;

    if (!patient.isEmpty()) {
        updateQuery += "patient=:patient";
        first = false;
    }
    if (!nom_analyse.isEmpty()) {
        updateQuery += (first ? "" : ", ") + QString("nom_analyse=:nom_analyse");
        first = false;
    }
    if (!type.isEmpty()) {
        updateQuery += (first ? "" : ", ") + QString("type=:type");
        first = false;
    }
    if (!caracteristique.isEmpty()) {
        updateQuery += (first ? "" : ", ") + QString("caracteristique=:caracteristique");
        first = false;
    }
    if (!medecin.isEmpty()) {
        updateQuery += (first ? "" : ", ") + QString("medecin=:medecin");
        first = false;
    }
    if (!duree.isEmpty()) {
        updateQuery += (first ? "" : ", ") + QString("duree=:duree");
        first = false;
    }
    if (!resultat.isEmpty()) {
        updateQuery += (first ? "" : ", ") + QString("resultat=:resultat");
    }

    updateQuery += " WHERE id_analyse=:id_analyse";
    query.prepare(updateQuery);
    query.bindValue(":id_analyse", id);

    if (!patient.isEmpty()) query.bindValue(":patient", patient);
    if (!nom_analyse.isEmpty()) query.bindValue(":nom_analyse", nom_analyse);
    if (!type.isEmpty()) query.bindValue(":type", type);
    if (!caracteristique.isEmpty()) query.bindValue(":caracteristique", caracteristique);
    if (!medecin.isEmpty()) query.bindValue(":medecin", medecin);
    if (!duree.isEmpty()) query.bindValue(":duree", duree);
    if (!resultat.isEmpty()) query.bindValue(":resultat", resultat);

    return query.exec();
}


bool Analyse::supprimer_analyse(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM analyse WHERE ID_ANALYSE = :id");
    query.bindValue(":id", id);

    if(query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur de suppression :" << query.lastError().text();
        return false;
    }
}
