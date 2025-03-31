
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "analyse.h"
#include <QMessageBox>
#include <QItemSelectionModel>
#include <QSqlQueryModel>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QVBoxLayout>

#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tab_affichage_2->setStyleSheet(
        "QTableView::item:selected { "
        "background-color: #e27396; "  // Rose
        "color: black; "
        "}");

    // Charger les données de la table lors de l'ouverture de l'application
    ui->tab_affichage_2->setModel(Analyse().afficher_analyse());

    connect(ui->tab_affichage_2, &QTableView::clicked, this, &MainWindow::on_tableView_clicked);
    connect(ui->exportPDFButton, &QPushButton::clicked, this, &MainWindow::on_exportPDFButton_clicked);
    connect(ui->button_chercher, &QPushButton::clicked, this, &MainWindow::rechercherParPatient);
    connect(ui->trierDureeButton, &QPushButton::clicked, this, &MainWindow::trierParDuree);
    // Assuming ui->statButton is the pointer to your QPushButton in the UI
    connect(ui->statButton, &QPushButton::clicked, this, &MainWindow::on_statButton_clicked);
     connect(ui->envoyerEmailButton, &QPushButton::clicked, this, &MainWindow::on_envoyerEmailButton_clicked);
    connect(ui->genererRapportButton, &QPushButton::clicked, this, &MainWindow::on_genererRapportButton_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::validateDate(const QString &date) {
    bool valid;
    date.toInt(&valid); // Convert string to integer
    return valid && date.toInt() > 0; // Check if the date is a valid positive integer
}

// Validation for text fields to allow only letters
bool MainWindow::validateText(const QString &text) {
    QRegularExpression regex("^[A-Za-zÀ-ÿ]+$");  // Regex to allow only letters (including accented characters)
    QRegularExpressionMatch match = regex.match(text);
    return match.hasMatch();  // Check if the text matches the regex pattern
}
bool MainWindow::validateEmail(const QString &email) {
    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex.match(email).hasMatch();
}
// SUPPRIMER UNE ANALYSE
void MainWindow::on_supprimerL_clicked()
{
    QItemSelectionModel *selection = ui->tab_affichage_2->selectionModel();

    if (!selection->hasSelection()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ligne à supprimer.");
        return;
    }

    QModelIndex index = selection->currentIndex();
    int id = ui->tab_affichage_2->model()->data(index.siblingAtColumn(0)).toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide. Veuillez sélectionner une ligne valide.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer cette analyse ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Analyse analyse;
        bool test = analyse.supprimer_analyse(id);

        if (test) {
            QMessageBox::information(this, "Succès", "Suppression effectuée.");
            ui->tab_affichage_2->setModel(analyse.afficher_analyse());
        } else {
            QMessageBox::critical(this, "Erreur", "Suppression non effectuée. Vérifiez l'ID.");
        }
    }
}

// MODIFIER UNE ANALYSE
void MainWindow::on_modifierL_clicked() {
    QItemSelectionModel *selection = ui->tab_affichage_2->selectionModel();

    if (!selection->hasSelection()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ligne à modifier.");
        return;
    }

    QModelIndex index = selection->currentIndex();
    int id = ui->tab_affichage_2->model()->data(index.siblingAtColumn(0)).toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide.");
        return;
    }


    // Récupérer les champs remplis
    QString patient = ui->patient_a->text().trimmed();
    QString nom_analyse = ui->nom_analyse_a->text().trimmed();
    QString type = ui->type_a->text().trimmed();
    QString caracteristique = ui->caracteristique_a->text().trimmed();
    QString medecin = ui->medecin_a->text().trimmed();
    QString duree = ui->duree_a->text().trimmed();
    QString resultat = ui->resultat_a->text().trimmed();

    // Vérifier qu'au moins un champ est rempli
    if (patient.isEmpty() && nom_analyse.isEmpty() && type.isEmpty() && caracteristique.isEmpty() &&
        medecin.isEmpty() && duree.isEmpty() && resultat.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir au moins un champ à modifier.");
        return;
    }
    if (!validateDate(duree)) {
        QMessageBox::warning(this, "Erreur", "La durée doit être un entier positif.");
        return;
    }

    // Validate text fields to allow only letters
    if (!validateText(patient)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Patient' ne doit contenir que des lettres.");
        return;
    }
    if (!validateText(nom_analyse)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Nom analyse' ne doit contenir que des lettres.");
        return;
    }
    if (!validateText(type)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Type' ne doit contenir que des lettres.");
        return;
    }
    if (!validateText(caracteristique)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Caractéristique' ne doit contenir que des lettres.");
        return;
    }
    /* if (!validateText(medecin)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Médecin' ne doit contenir que des lettres.");
        return;
    }*/
    if (!validateText(resultat)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Résultat' ne doit contenir que des lettres.");
        return;
    }
    if (!validateEmail(medecin)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Médecin' doit être une adresse e-mail valide.");
        return;
    }

    // Modifier uniquement les champs remplis
    Analyse analyse;
    bool test = analyse.modifier_analyse(id, patient, nom_analyse, type, caracteristique, medecin, duree, resultat);

    if (test) {
        QMessageBox::information(this, "Succès", "Modification effectuée.");
        ui->tab_affichage_2->setModel(analyse.afficher_analyse());

        // Réinitialiser les champs après la modification
        ui->patient_a->clear();
        ui->nom_analyse_a->clear();
        ui->type_a->clear();
        ui->caracteristique_a->clear();
        ui->medecin_a->clear();
        ui->duree_a->clear();
        ui->resultat_a->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
}





// AJOUTER UNE ANALYSE
void MainWindow::on_ajouterL_clicked()
{
    bool validInput = true;
    int id = ui->id_a->text().toInt(&validInput);

    // Vérifier si l'ID est valide
    if (!validInput || id <= 0) {
        QMessageBox::warning(this, "Erreur", "L'ID doit être un entier positif.");
        return;
    }

    QString patient = ui->patient_a->text().trimmed();
    QString nom_analyse = ui->nom_analyse_a->text().trimmed();
    QString type = ui->type_a->text().trimmed();
    QString caracteristique = ui->caracteristique_a->text().trimmed();
    QString medecin = ui->medecin_a->text().trimmed();
    QString duree = ui->duree_a->text().trimmed();
    QString resultat = ui->resultat_a->text().trimmed();

    // Valider tous les champs
    if (patient.isEmpty() || nom_analyse.isEmpty() || type.isEmpty() || caracteristique.isEmpty() ||
        medecin.isEmpty() || duree.isEmpty() || resultat.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs sont obligatoires et doivent être valides.");
        return;
    }
    if (!validateDate(duree)) {
        QMessageBox::warning(this, "Erreur", "La durée doit être un entier positif.");
        return;
    }

    // Validate text fields to allow only letters
    if (!validateText(patient)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Patient' ne doit contenir que des lettres.");
        return;
    }
    if (!validateText(nom_analyse)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Nom analyse' ne doit contenir que des lettres.");
        return;
    }
    if (!validateText(type)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Type' ne doit contenir que des lettres.");
        return;
    }
    if (!validateText(caracteristique)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Caractéristique' ne doit contenir que des lettres.");
        return;
    }
   /* if (!validateText(medecin)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Médecin' ne doit contenir que des lettres.");
        return;
    }*/
    if (!validateText(resultat)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Résultat' ne doit contenir que des lettres.");
        return;
    }
    if (!validateEmail(medecin)) {
        QMessageBox::warning(this, "Erreur", "Le champ 'Médecin' doit être une adresse e-mail valide.");
        return;
    }

    // Créer un objet Analyse et ajouter l'enregistrement
    Analyse analyse(id, patient, nom_analyse, type, caracteristique, medecin, duree, resultat);
    bool test = analyse.ajouter_analyse();

    if (test) {
        QMessageBox::information(this, "Succès", "Ajout effectué.");

        // Réinitialiser les champs après l'ajout
        ui->id_a->clear();
        ui->patient_a->clear();
        ui->nom_analyse_a->clear();
        ui->type_a->clear();
        ui->caracteristique_a->clear();
        ui->medecin_a->clear();
        ui->duree_a->clear();
        ui->resultat_a->clear();

        // Recharger les données dans le tableau
        ui->tab_affichage_2->setModel(analyse.afficher_analyse());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout.");
    }
}
void MainWindow::on_tableView_clicked(const QModelIndex &index) {
    if (!index.isValid()) return;

    // Récupérer l'ID de la ligne sélectionnée
    int row = index.row();
    QModelIndex idIndex = ui->tab_affichage_2->model()->index(row, 0); // Supposons que l'ID est dans la colonne 0
    int id = idIndex.data().toInt();

    // Récupérer les autres valeurs de la ligne sélectionnée
    QModelIndex patientIndex = ui->tab_affichage_2->model()->index(row, 1);
    QModelIndex nomAnalyseIndex = ui->tab_affichage_2->model()->index(row, 2);
    QModelIndex typeIndex = ui->tab_affichage_2->model()->index(row, 3);
    QModelIndex caracteristiqueIndex = ui->tab_affichage_2->model()->index(row, 4);
    QModelIndex medecinIndex = ui->tab_affichage_2->model()->index(row, 5);
    QModelIndex dureeIndex = ui->tab_affichage_2->model()->index(row, 6);
    QModelIndex resultatIndex = ui->tab_affichage_2->model()->index(row, 7);

    // Remplir les champs correspondants
    ui->patient_a->setText(patientIndex.data().toString());
    ui->nom_analyse_a->setText(nomAnalyseIndex.data().toString());
    ui->type_a->setText(typeIndex.data().toString());
    ui->caracteristique_a->setText(caracteristiqueIndex.data().toString());
    ui->medecin_a->setText(medecinIndex.data().toString());
    ui->duree_a->setText(dureeIndex.data().toString());
    ui->resultat_a->setText(resultatIndex.data().toString());
}

void MainWindow::on_exportPDFButton_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save as PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }

    // Création du document PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QTextDocument document;
    QString htmlContent;

    // Ajout d'un titre au document
    htmlContent.append("<h2 style='text-align: center; color: #e27396;'>Liste des Analyses</h2>");

    // Début de la table HTML
    htmlContent.append("<table border='1' cellspacing='0' cellpadding='5' width='100%'>");
    htmlContent.append("<tr bgcolor='#e27396' style='color: black; text-align: center;'>");

    // Récupération du nombre de colonnes et de leur en-tête
    int columnCount = ui->tab_affichage_2->model()->columnCount();
    for (int col = 0; col < columnCount; ++col) {
        htmlContent.append("<th>" + ui->tab_affichage_2->model()->headerData(col, Qt::Horizontal).toString() + "</th>");
    }
    htmlContent.append("</tr>");

    // Récupération des données de la table
    int rowCount = ui->tab_affichage_2->model()->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        htmlContent.append("<tr>");

        for (int col = 0; col < columnCount; ++col) {
            QModelIndex index = ui->tab_affichage_2->model()->index(row, col);
            htmlContent.append("<td style='text-align: center;'>" + index.data().toString() + "</td>");
        }

        htmlContent.append("</tr>");
    }

    htmlContent.append("</table>");

    // Charger le contenu HTML dans le document et l'imprimer en PDF
    document.setHtml(htmlContent);
    document.print(&printer);

    QMessageBox::information(this, "Export PDF", "Le fichier PDF a été créé avec succès.");
}

void MainWindow::rechercherParPatient() {
    QString patientRecherche = ui->lineEdit_recherche_patient->text().trimmed();

    if (patientRecherche.isEmpty()) {
        QMessageBox::warning(this, "Recherche", "Veuillez entrer un nom de patient.");
        return;
    }

    // Préparer la requête SQL
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM analyse WHERE patient LIKE :patient");
    query.bindValue(":patient", "%" + patientRecherche + "%");
    query.exec();

    // Appliquer le résultat au tableau
    model->setQuery(query);
    ui->tab_affichage_2->setModel(model);
}

void MainWindow::trierParDuree()
{
    // Create a SQL query to sort the data by "duree" column in ascending order
    QString queryStr = "SELECT * FROM analyse ORDER BY duree ASC";

    // Create a new model to hold the sorted data
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    // Prepare and execute the query
    query.prepare(queryStr);
    query.exec();

    // Set the query result to the model and update the table view
    model->setQuery(query);
    ui->tab_affichage_2->setModel(model);
}

void MainWindow::on_statButton_clicked()
{
    // Création d'une série pour le diagramme circulaire
    QPieSeries *series = new QPieSeries();

    // Créer la requête pour récupérer le nombre d'analyses par type
    QSqlQuery query;
    query.prepare("SELECT type, COUNT(*) AS count FROM analyse GROUP BY type");

    // Exécuter la requête
    if (query.exec()) {
        // Parcourir les résultats de la requête
        while (query.next()) {
            QString type = query.value("type").toString();
            int count = query.value("count").toInt();

            // Ajouter chaque type au graphique
            series->append(type, count);
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération des données pour les statistiques.");
        return;
    }

    // Personnalisation des tranches
    for (QPieSlice *slice : series->slices()) {
        slice->setLabel(slice->label() + " " + QString::number(slice->value()));
    }

    // Création du graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des analyses médicales");
    chart->legend()->setVisible(true);

    // Création de la fenêtre d'affichage
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Affichage du graphique dans une boîte de dialogue
    QDialog *dialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(chartView);
    dialog->setLayout(layout);
    dialog->resize(600, 400);
    dialog->exec();
}
/*void MainWindow::on_envoyerEmailButton_clicked()
{
    QString recipient = ui->medecin_a->text().trimmed(); // Récupérer l'email du champ "Médecin"
    QString subject = "Résultats d'analyse";
    QString body = "Bonjour,\n\nVoici les résultats de l'analyse pour votre patient.\n\nCordialement,\nVotre équipe médicale.";

    if (recipient.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une adresse e-mail valide.");
        return;
    }

    // Encodage des paramètres dans l'URL
    QString mailto = "mailto:" + recipient + "?subject=" + subject + "&body=" + body;
    QDesktopServices::openUrl(QUrl(mailto));
}*/
void MainWindow::on_envoyerEmailButton_clicked()
{
    QString recipient = ui->medecin_a->text().trimmed(); // Récupérer l'email du champ "Médecin"
    QString subject = "Résultats d'analyse";

    // Récupérer les informations du patient et du nom de l'analyse
    QString patientName = ui->patient_a->text().trimmed(); // Supposons que le nom du patient est dans un champ "nom_patient"
    QString analysisName = ui->nom_analyse_a->text().trimmed(); // Supposons que le nom de l'analyse est dans un champ "nom_analyse"

    // Exemple de récupération dynamique des résultats d'analyse
    QString analyseResult = ui->resultat_a->text().trimmed(); // Supposons que le résultat d'analyse est dans un champ QTextEdit appelé "resultat_a"

    // Vérification des champs obligatoires
    if (patientName.isEmpty() || analysisName.isEmpty() || analyseResult.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer toutes les informations nécessaires (patient, analyse, résultats).");
        return;
    }

    // Construction du corps de l'email avec les informations ajoutées
    QString body = "Bonjour,\n\nVoici les résultats de l'analyse '" + analysisName + "' pour le patient " + patientName + " :\n\n" + analyseResult + "\n\nCordialement,\nVotre équipe médicale.";

    // Vérification si l'email est valide
    if (recipient.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une adresse e-mail valide.");
        return;
    }

    // Encodage des paramètres dans l'URL
    QString mailto = "mailto:" + recipient + "?subject=" + subject + "&body=" + QUrl::toPercentEncoding(body);
    QDesktopServices::openUrl(QUrl(mailto));
}


void MainWindow::on_genererRapportButton_clicked() {
    // Sélectionner un patient
    QModelIndex index = ui->tab_affichage_2->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un patient.");
        return;
    }

    // Récupérer les données du patient
    QString nomPatient = ui->tab_affichage_2->model()->data(ui->tab_affichage_2->model()->index(index.row(), 0)).toString(); // Colonne 0 = Nom
    QString description = "Le patient " + nomPatient + " présente les résultats des analyses suivantes : "; // Personnalisez cette phrase
    QString analyses;

    // Ajouter les analyses et résultats sous forme de paragraphes
    int columnCount = ui->tab_affichage_2->model()->columnCount();
    for (int col = 1; col < columnCount; col += 2) { // Supposons que les colonnes impaires contiennent les analyses et paires les résultats
        QString nomAnalyse = ui->tab_affichage_2->model()->data(ui->tab_affichage_2->model()->index(index.row(), col)).toString();
        QString resultat = ui->tab_affichage_2->model()->data(ui->tab_affichage_2->model()->index(index.row(), col + 1)).toString();
        analyses += "<p><strong>Analyse : </strong>" + nomAnalyse + "<br><strong>Résultat : </strong>" + resultat + "</p>";
    }

    // Demander le chemin du fichier
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le rapport", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    // Configurer l'impression
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // Construire le contenu HTML
    QString htmlContent = "<h2 style='text-align: center; color: #e27396;'>Rapport Médical</h2>";
    htmlContent += "<h3>Nom du Patient : " + nomPatient + "</h3>";
    htmlContent += "<p><strong>État du Patient :</strong> " + description + "</p>";
    htmlContent += analyses;  // Ajouter les analyses dans le rapport

    // Ajouter la signature Innovax
    htmlContent += "<br><p style='text-align: right;'><strong>Signature :</strong> <br><img src='signature_innovax.png' width='150'></p>";

    // Générer le PDF
    QTextDocument document;
    document.setHtml(htmlContent);
    document.print(&printer);

    QMessageBox::information(this, "Succès", "Le rapport médical a été généré avec succès.");
}




