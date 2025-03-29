#ifndef ANALYSE_H
#define ANALYSE_H

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QString>
#include <QDate>
#include <QRegularExpression>
class Analyse
{
public:
    Analyse();
    Analyse(int, QString, QString, QString, QString, QString, QString, QString);
    Analyse( QString, QString, QString, QString, QString, QString, QString);

    // Getters
    int get_id_analyse();
    QString get_patient();
    QString get_nom_analyse();
    QString get_type();
    QString get_caracteristique();
    QString get_medecin();
    QString get_duree();
    QString get_resultat();

    // Setters
    void set_id_analyse(int);
    void set_patient(QString);
    void set_nom_analyse(QString);
    void set_type(QString);
    void set_caracteristique(QString);
    void set_medecin(QString);
    void set_duree(QString);
    void set_resultat(QString);

    // CRUD
    bool ajouter_analyse();
    QSqlQueryModel* afficher_analyse();
   // bool modifier_analyse(int);
    bool modifier_analyse(int id, QString patient, QString nom_analyse, QString type,
                                   QString caracteristique, QString medecin, QString duree, QString resultat);
    bool supprimer_analyse(int);

    QSqlQuery get_analyse();

    // Validation Methods
    bool validerId(int d);
    bool validerNom(QString );
    bool validerType(QString );
    bool validerCaracteristiques(QString );
    bool validerDuree(QString);
    bool validerResultat(QString);


private:
    int id_analyse;
    QString  patient, nom_analyse, type, caracteristique, medecin,duree,resultat;
};

#endif

/*#ifndef ANALYSE_H
#define ANALYSE_H

class Analyse
{
public:
    Analyse();
};

#endif // ANALYSE_H*/
