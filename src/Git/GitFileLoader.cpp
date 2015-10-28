#include "Git/GitVersion.h"
#include "Git/GitFile.h"
#include "Git/GitFileDiffBlock.h"
#include "Git/GitFileDiffBlockLine.h"

#include <QDebug>
Git::GitFileLoader::GitFileLoader() {
    // Inicializacia premennych na defaultne hodnoty
    this->filePath = "";
    this->extensions = "";
}
Git::GitFileLoader::GitFileLoader( QString filepath, QString extensions ) : filePath( filepath ), extensions( extensions ) {
    // Inicializacia lokalnych premmenych

    // Vykona command, vystup ulozi do temp suboru a vrati cestu k temp suboru
    // Kontrola na uspesnost ulozenia vystupu commandu do suboru, ak sa podarilo tak "ok" ostane true
    // Ak je velkost suboru vacsia ako 0 a podarilo sa otvorit subor, tak z neho citame az do konca

        // Pokial neprideme na koniec suboru tak citame po riadkoch
        while( !reader.atEnd() ) {

            // Prvy riadok vstupnu je vzdy commit identifikator
            // Druhy riadok je meno autora
            // Treti riadok je datum commitu
            // Citame dokym nepride prvy prazdny riadok, co znaci koniec commitu

                // Vytvorime si regex na pridane(A), modifikovane(M) a vymazane(D) subory
                // Ak sa najde zhoda s jednym z regexov, tak nahradime regex za "project/" (co bude root pre vsetky subory), vytvorime
                // subor s jeho nazvom, celou cestou a typom podla zisteneho regexu a pridame ho do verzie.
                // V pripade, ze sa nenasiel ani jeden regex, tak skontrolujeme dlzku nacitaneho riadku. Ak je rovny 40, tak ide
                // o viacnasobne uvedenie identifikatoru a autora, vtedy precitame a zahodime. Ak nie je rovny 40, skonci vytvaranie verzie.

            // Nastavim zmenene subory podla zadanych filtrovacich kriterii z appConfig
            version->setChangedFiles( version->getGitFilesByExtension( this->extensions ) );

            // Ak je velkost zmenenych suborov vacsia ako 0, tak pridam verziu
            if( version->getChangedFiles().size() ) {
                versions.append( version );
            }
    // Ak sa mi nepodari vymazat subor, tak vypisem jeho nazov
        qDebug() << "Nepodarilo sa odstranit temp subor" << file.fileName();
    // Vratim list verzii, ktore splnaju filtrovacie kriteria
    // Ulozim si current working directory
    // Nastavim absolutnu cestu k  temp file ako template a zakazem automaticke mazanie

    // Ak sa nepodarilo vytvorit temp subor, tak nastavim flag "ok" na false a vypisem chybu
        qDebug() << "Nepodarilo sa vytvorit tmp subor";
        ok = false;
    // Ak sa podarilo vytvorit temp subor, tak zmenim current working directory
    // Ak sa podarilo zmenit current working directory, tak skontroluje existenciu git repozitara
    // Ak existuje na danej ceste git repozitar, tak vykonam command a vystup ulozim do temp suboru
    // Vratim povodny current working directory, ak sa nepodari zmenit, vypisem do konzoly
    // Vratim absolutnu cestu k temp suboru
void Git::GitFileLoader::readGitShowFile( QString tmpFile, Git::GitFile* gitFile ) {
    // Kontrola na uspesnost ulozenia vystupu commandu do suboru, ak sa podarilo tak "ok" ostane true

    // Ak je velkost suboru vacsia ako 0 a podarilo sa otvorit subor, tak z neho citame az do konca

        // Citam do konca a pridavam jednotlive riadky typu ADDED
            QString lLine = lReader.readLine();

        // Pridam diff blok do suboru
    // Ak sa mi nepodari vymazat subor, tak vypisem jeho nazov
        qDebug() << "Nepodarilo sa odstranit temp subor" << lFile.fileName();
void Git::GitFileLoader::readGitDiffFile( QString tmpFile, Git::GitFile* gitFile ) {
    // Kontrola na uspesnost ulozenia vystupu commandu do suboru, ak sa podarilo tak "ok" ostane true
    // Ak je velkost suboru vacsia ako 0 a podarilo sa otvorit subor, tak z neho citame az do konca
        // Defaultny pocet riadkov diff hlavicky
        int iter = 4;
        // Preskocime hlavicku diffu, ktora ma default pocet riadkov 4

            // Ak sa v diff hlavicke nachadza retazec "new file mode" alebo "deleted file mode" treba preskocit
            // o jeden riadok naviac
        // Pokial neprideme na koniec suboru tak citame po riadkoch

            // Podla hodnoty prveho znaku z nacitaneho riadku rozhodujem, ako dalej spracujem dany riadok

            // Ak prvy znak bol '@', tak zistim zaciatocnu poziciu pridania a pocet riadkov oblasti pridania,
            // zaciatok mazania a pocet riadkov oblasti mazania. Nasledne tieto udaje ulozim do diff bloku.
                // Preskoci poslednu instanciu a prida ju az, ked sa precita cely subor

                // Nastavenie zaciatkov a poctu riadko oblasti
                // Inicializacia pocitadiel pozicie pre pridanie a odstranenie

            // Ak prvy znak bol '+', tak tento znak odstranim, vytvorim diff blok riadok s typom ADDED, pridam ho do diff bloku
            // a zvacsim pocitadlo pozicie pre pridanie

            // Ak prvy znak bol '-', tak tento znak odstranim, vytvorim diff blok riadok s typom REMOVED, pridam ho do diff bloku
            // a zvacsim pocitadlo pozicie pre odstranenie

            // Ak prvy znak bol ' ', tak tento znam odstranim, vytvorim diff block riadok s typom NONE, pridam ho do diff bloku
            // a zvacsim obidve pocitadla, pricom typ NONE ukladam s pozicou pocitadla pre pridanie

        // Pridam poslednu iteraciu diff bloku do suboru
    // Ak sa mi nepodari vymazat subor, tak vypisem jeho nazov
        qDebug() << "Nepodarilo sa odstranit temp subor" << lFile.fileName();
void Git::GitFileLoader::getDiffInfo( Git::GitFile* gitFile, QString currentCommitId, QString oldCommitId ) {
    // Ak bol pridany subor pridany, nemame jeho predchadzajucu verziu, preto nacitame celu verziu suboru
    // Vykona command, vystup ulozi do temp suboru a vrati cestu k temp suboru
    // Ak je typ suboru ADDED, tak nema predchadzajucu verziu a nacitam celu verziu suboru.
    // Ak nie, tak nacitam diff bloky daneho suboru
    if( gitFile->getType() == Git::GitType::ADDED ) {
        readGitShowFile( lTmp, gitFile );
        readGitDiffFile( lTmp, gitFile );