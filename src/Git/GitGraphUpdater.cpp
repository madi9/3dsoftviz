#include "Git/GitGraphUpdater.h"

#include "Data/Graph.h"
#include "Data/Node.h"
#include "Data/Edge.h"

#include "Manager/Manager.h"

#include "Git/GitEvolutionGraph.h"
#include "Git/GitFile.h"
#include "Git/GitType.h"
#include "Git/GitVersion.h"

#include <QDebug>

Git::GitGraphUpdater::GitGraphUpdater( int currentVersion, Git::GitEvolutionGraph *evolutionGraph, Data::Graph *activeGraph ) : currentVersion( currentVersion ), evolutionGraph( evolutionGraph ), activeGraph( activeGraph ) {

    // Inicializacia spracovanie cez viacere verzie
    this->moreVersionChanged = false;
}

void Git::GitGraphUpdater::nextVersion() {
    // zistime zmenene subory v dalsej verzii, s ktorymi budeme dalej pracovat
    QList<Git::GitFile*> gitFiles = this->getEvolutionGraph()->getVersion( this->getCurrentVersion() + 1 )->getChangedFiles();
    Data::Node* lAuthorNode = nullptr;
    QString lAuthorName = nullptr;

    // Ak ide o zmenu len cez jednu verziu, tak vymazem hrany od autora a uzol autora z grafu a pridam autora dalsej verzie
    if( !moreVersionChanged ) {
        // Vymazanie hran od autora a vymazanie autorovho uzla
        removeAuthorEdgesFromGraph( this->getEvolutionGraph()->getVersion( this->getCurrentVersion() )->getAuthor(), this->getEvolutionGraph()->getVersion( this->getCurrentVersion() )->getChangedFiles() );
        this->getActiveGraph()->removeNode( this->getActiveGraph()->findNodeByName( this->getEvolutionGraph()->getVersion( this->getCurrentVersion() )->getAuthor() ) );

        // Ziskame meno autora
        lAuthorName = this->getEvolutionGraph()->getVersion( this->currentVersion + 1 )->getAuthor();

        lAuthorNode = this->getActiveGraph()->findNodeByName( lAuthorName );
        // Ak autor neexistuje v grafe, tak ho vytvorim
        if( !lAuthorNode ) {
            lAuthorNode = this->getActiveGraph()->addNode( lAuthorName, this->getActiveGraph()->getTypesByName( "author" ).at( 0 ) );
            lAuthorNode->setLabelText( lAuthorNode->Data::AbsNode::getName() );
            lAuthorNode->showLabel( true );
        }
    }

    bool ok = true;

    // Pre kazdy zmeneny subor vo verzii, zistim jeho ciastocne cesty a podla typu suboru, vykonam akciu nad tymito cestami
    foreach( Git::GitFile* gitFile, gitFiles ){
        QStringList lList = gitFile->getFilepath().split( "/" );
        QString lPom = "";

        for( int i = 0; i < lList.size(); i++ ) {
            lPom += lList.at( i );
            lList.replace( i, lPom );
            lPom += "/";
        }
        switch( gitFile->getType() ) {
        // Ak bol pridany v dalsej verzii, tak ho pridame do grafu
        case Git::GitType::ADDED :
            addNodesToGraph( lList );
            addEdgesToGraph( lList );
            break;
        // Ak bol vymazany v dalsej verzii, tak ho vymazeme z grafu
        case Git::GitType::REMOVED :
            removeEdgesFromGraph( lList );
            removeNodesFromGraph( lList );
            break;
        case Git::GitType::MODIFIED :
            modifyNodesInGraph( lList );
            break;
        }
    }

    // Ak ide o zmenu len cez jednu verziu, tak pridam pre vsetky zmenene subory hrany od autora
    if( !moreVersionChanged ) {
        addAuthorEdgesToGraph( lAuthorName, this->getEvolutionGraph()->getVersion( this->getCurrentVersion() + 1 )->getChangedFiles() );
    }
}

void Git::GitGraphUpdater::previousVersion() {
    // zistime si zmenene subory v aktualnej verzii, s ktorymi budeme dalej pracovat
    QList<Git::GitFile*> gitFiles = this->getEvolutionGraph()->getVersion( this->getCurrentVersion() )->getChangedFiles();
    Data::Node* lAuthorNode = nullptr;
    QString lAuthorName = nullptr;

    // Ak ide o zmenu len cez jednu verziu, tak vymazem hrany od autora a uzol autora z grafu a pridam autora dalsej verzie
    if( !moreVersionChanged ) {
        // Vymazanie hran od autora a vymazanie autorovho uzla
        removeAuthorEdgesFromGraph( this->getEvolutionGraph()->getVersion( this->getCurrentVersion() )->getAuthor(), this->getEvolutionGraph()->getVersion( this->getCurrentVersion() )->getChangedFiles() );
        this->getActiveGraph()->removeNode( this->getActiveGraph()->findNodeByName( this->getEvolutionGraph()->getVersion( this->getCurrentVersion() )->getAuthor() ) );

        // Ziskame meno autora
        lAuthorName = this->getEvolutionGraph()->getVersion( this->currentVersion - 1 )->getAuthor();

        lAuthorNode = this->getActiveGraph()->findNodeByName( lAuthorName );
        // Ak autor neexistuje v grafe, tak ho vytvorim
        if( !lAuthorNode ) {
            lAuthorNode = this->getActiveGraph()->addNode( lAuthorName, this->getActiveGraph()->getTypesByName( "author" ).at( 0 ) );
            lAuthorNode->setLabelText( lAuthorNode->Data::AbsNode::getName() );
            lAuthorNode->showLabel( true );
        }
    }

    bool ok = true;

    // Pre kazdy zmeneny subor vo verzii, zistim jeho ciastocne cesty a podla typu suboru, vykonam akciu nad tymito cestami
    foreach( Git::GitFile* gitFile, gitFiles ){
        QStringList lList = gitFile->getFilepath().split( "/" );
        QString lPom = "";

        for( int i = 0; i < lList.size(); i++ ) {
            lPom += lList.at( i );
            lList.replace( i, lPom );
            lPom += "/";
        }

        switch( gitFile->getType() ) {
        // Ak bol pridany v aktualnej verzii, tak ho vymazeme z grafu
        case Git::GitType::ADDED :
            removeEdgesFromGraph( lList );
            removeNodesFromGraph( lList );
            break;
        // Ak bol vymazany v aktualnej verzii, tak ho pridame do grafu
        case Git::GitType::REMOVED :
            addNodesToGraph( lList );
            addEdgesToGraph( lList );
            break;
        case Git::GitType::MODIFIED :
            modifyNodesInGraph( lList );
            break;
        }
    }

    // Ak ide o zmenu len cez jednu verziu, tak pridam pre vsetky zmenene subory hrany od autora
    if( !moreVersionChanged ) {
        addAuthorEdgesToGraph( lAuthorName, this->getEvolutionGraph()->getVersion( this->getCurrentVersion() - 1 )->getChangedFiles() );
    }
}

void Git::GitGraphUpdater::changeToVersion( int toVersion ) {
    int inc;

    // Ak sucasna verzie vacsia ako verzia, ktoru chceme, tak nastavime inkrement na -1, opacne na 1.
    if( this->getCurrentVersion() > toVersion ) {
        inc = -1;
    } else {
        inc = 1;
    }

    int startVersion = this->getCurrentVersion();
    Manager::GraphManager::getInstance()->showProgressBar();

    // Vymazanie hran od autora a uzla autora zaciatocnej verzie
    removeAuthorEdgesFromGraph( this->getEvolutionGraph()->getVersion( this->getCurrentVersion() )->getAuthor(), this->getEvolutionGraph()->getVersion( this->getCurrentVersion() )->getChangedFiles() );
    this->getActiveGraph()->removeNode( this->getActiveGraph()->findNodeByName( this->getEvolutionGraph()->getVersion( this->getCurrentVersion() )->getAuthor() ) );

    // Nastavime na true, aby sa pre kazdu verziu nevytvarali autory a ich hrany ku zmenenym uzlov
    this->moreVersionChanged = true;

    // Prechadzame po verziach az dokym dokym neprideme k volanej verzie
    for( int i = this->getCurrentVersion(); i != toVersion; i += inc ) {
        // Ak je inkrement -1, tak sa vraciame vo verziach, opacne ideme na dalsiu verziu.
        if( inc == -1 ) {
            this->previousVersion();
        } else {
            this->nextVersion();
        }

        // Nastavenie aktualnej verzie grafu a aktualizacia progressbaru
        this->setCurrentVersion( this->getCurrentVersion() + inc );
        this->getActiveGraph()->setCurrentVersion( this->getCurrentVersion() );
        Manager::GraphManager::getInstance()->setProgressBarValue( (int)( ( (double)( abs( i - startVersion ) ) / (double) abs( toVersion - startVersion ) ) * 100 ) );
    }

    // Nastavime na false, aby sa pre dalsie spracovanie mohol pridat autor a jeho hrany k zmenenych uzlom.
    this->moreVersionChanged = false;

    // Ziskam meno volanej verzie a skontrolujem ci existuje v grafe.
    QString lAuthorName = this->getEvolutionGraph()->getVersion( this->getCurrentVersion() )->getAuthor();
    Data::Node* lAuthorNode = this->getActiveGraph()->findNodeByName( lAuthorName );

    // Ak autor v grafe neexistuje, tak ho pridam.
    if( !lAuthorNode ) {
        lAuthorNode = this->getActiveGraph()->addNode( lAuthorName, this->getActiveGraph()->getTypesByName( "author" ).at( 0 ) );
        lAuthorNode->setLabelText( lAuthorNode->Data::AbsNode::getName() );
        lAuthorNode->showLabel( true );
    }

    // Pridam hrany od autora k zmenenych uzlom vo vyslednej verzii
    addAuthorEdgesToGraph( lAuthorName, this->getEvolutionGraph()->getVersion( this->getCurrentVersion() )->getChangedFiles() );

    Manager::GraphManager::getInstance()->closeProgressBar();
}

void Git::GitGraphUpdater::addNodesToGraph( QStringList list ) {
    bool ok = true;

    // Pre kazdu ciastocnu cestu zachytenu v liste stringov skontrolujem, ci sa uz nenachadza v grafe. Ak sa nenachadza
    // tak potom zistim typ uzla(koren adresar, subor) a nasledne danu cestu pridam do grafu ako uzol.
    for( int i = 0; i < list.size(); i++ ) {
        bool exist = false;
        QString lNodeName = list.at( i );

        // Skontrolujem ci dany QString nie je prazdny
        ok = !lNodeName.isEmpty();

        osg::ref_ptr<Data::Node> lNode( nullptr );
        // Ak sa cesta v grafe uz nachadza, tak nastavim exist na true
        if( this->getActiveGraph()->findNodeByName( lNodeName) ) {
            exist = true;
        }

        // Ak sa cesta v grafe nenachadzala, tak zistim typ ciastocnej cesty a pridam ho do grafu s jeho labelom.
        if( !exist ) {
            Data::Type* lType( nullptr );

            QString lVal;
            // Ak je ciastocna cesta z listu na pozicie n - 1, tak ide o subor, ak na pozici 0 tak ide o root ostatne su adresare
            if( i == list.size() - 1 ) {
                lVal = "file";
            } else {
                lVal = "dir";
                if( i == 0 ) {
                    lVal = "root";
                }
            }

            // Ziskam typ ulozeny v grafe so ziskanym nazvom typu
            lType = this->getActiveGraph()->getTypesByName( lVal ).at( 0 );

            // Vytvorim uzol pre danu cestu s danym typom
            lNode = this->getActiveGraph()->addNode( lNodeName, lType );

            // Ak ide o korenovy uzol, tak ho fixnem na jedno miesto
            if( QString::compare( lVal, "root" ) == 0 ) {
                lNode->setFixed( true );
            }

            // Nastavim label a pridam do mnoziny uzlov v grafe
            lNode->setLabelText( lNodeName );
            lNode->showLabel( true );
        }
    }
}

void Git::GitGraphUpdater::addEdgesToGraph( QStringList list ) {
    bool ok = true;

    Data::Type* lType = this->getActiveGraph()->getTypesByName( "Edge" ).at( 0 );

    // Pre ciastocne cesty vytvorim hranu medzi dvojicami i a i+1, vtedy ak dana hrana v grafe neexistuje
    // a uzly, ktore hranu definuju, existuju v grafe.
    for( int i = 0; i < list.size() - 1; i++ ) {
        bool exist = false;
        osg::ref_ptr<Data::Edge> lEdge( nullptr );

        QString lNodeNameFrom = list.at( i );
        QString lNodeNameTo = list.at( i + 1 );

        // Vytvorim nazov hrany kombinaciou uzlov, ktore definuju hranu
        QString lEdgeName = lNodeNameFrom + lNodeNameTo;

        // Pre kazdu hranu sa inkrementuje pocitadlo, kolkokrat je dana hrana naviazana v grafe.
        this->getActiveGraph()->addEdgeOccurence( lEdgeName );

        // Ak sa dana hrana v grafe nachadza, tak potom nastavim exist na true, co zamedzi dalsie spracovanie.
        if( this->getActiveGraph()->findEdgeByName( lEdgeName ) ) {
            exist =  true;
        }

        // Ak sa hrana v grafe nenachadza, tak zistim, ci jej uzly sa nachadzaju v grafe. Ak nie, tak sa hrana neprida.
        // V opacnom pripade sa prida.
        if( !exist ) {
            if( ok ) {
                // Ak sa uzol nenachadza v grafe, tak skonci dalsie spracovanie
                ok = this->getActiveGraph()->findNodeByName( lNodeNameFrom );
            }

            if( ok ) {
                // Ak sa uzol nenachadza v grafe, tak skonci dalsie spracovanie
                ok = this->getActiveGraph()->findNodeByName( lNodeNameTo );
            }

            bool oriented =  false;

            // Ak sa obidva uzly nachadzaju v grafe, tak vytvor medzi nimi hranu.
            if( ok ) {
                lEdge = this->getActiveGraph()->addEdge( lEdgeName, this->getActiveGraph()->findNodeByName( lNodeNameFrom), this->getActiveGraph()->findNodeByName( lNodeNameTo), lType, oriented );
            }
        }
    }
}

void Git::GitGraphUpdater::addAuthorEdgesToGraph( QString authorName, QList<Git::GitFile*> gitFiles ){
    // Pre kazdy subor zisti, ci existuje hrana medzi autorom a uzlom, ak nie, tak pridam hranu do grafu
    foreach ( Git::GitFile* gitFile, gitFiles ) {
        // Vyskladam nazov hrany spojenim mena autora a cesty k uzlu
        QString lEdgeName = authorName + gitFile->getFilepath();
        bool exist = true;

        if( exist ) {
            // Ak sa uzol k danemu suboru nachadza v grafe, tak je true, inak false.
            exist = this->getActiveGraph()->findNodeByName( gitFile->getFilepath() );
        }

        // Ak sa cielovy uzol nachadza v grafe a v grafe neexistuje dana hrana
        if( exist && !this->getActiveGraph()->findEdgeByName( lEdgeName ) ) {
            Data::Type* lType = this->getActiveGraph()->getTypesByName( "authorEdge" ).at( 0 );
            this->getActiveGraph()->addEdge( lEdgeName, this->getActiveGraph()->findNodeByName( authorName ), this->getActiveGraph()->findNodeByName( gitFile->getFilepath() ), lType, true );
        }
    }
}

void Git::GitGraphUpdater::removeAuthorEdgesFromGraph( QString authorName, QList<Git::GitFile*> gitFiles ){
    // Pre kazdy subor zisti, odstran hranu medzi autorom a uzlov, ak existuje v grafe
    foreach ( Git::GitFile* gitFile, gitFiles ) {
        // Vyskladam nazov hrany spojenim mena autora a cesty k uzlu
        QString lEdgeName = authorName + gitFile->getFilepath();

        Data::Edge* lEdge = this->getActiveGraph()->findEdgeByName( lEdgeName );
        // Ak sa hrana nachadza v grafe, tak vymazem danu hranu
        if( lEdge ) {
            this->getActiveGraph()->removeEdge( lEdge );
        }
    }

}

void Git::GitGraphUpdater::removeNodesFromGraph( QStringList list ) {
    // Pre ciastocne cesty zisti, ci sa hrana uz v grafe nenachadza a nasledne vymaz cielovy uzol
    for( int i =  0; i < list.size() - 1; i++ ) {
        // Ziskanie cesty zdrojoveho a cieloveho uzla
        QString lNodeNameFrom = list.at( i );
        QString lNodeNameTo = list.at( i + 1 );

        // Vyskladam nazov hrany spojenim zdrojoveho a cieloveho uzla
        QString lEdgeName = lNodeNameFrom + lNodeNameTo;

        // Ak hrana v grafe uz neexistuje, tak vymaz cielovy uzol
        if( !this->getActiveGraph()->getEdgeOccurence().value( lEdgeName ) ) {
            this->getActiveGraph()->removeNode( this->getActiveGraph()->findNodeByName( lNodeNameTo ) );
        }
    }
}

void Git::GitGraphUpdater::removeEdgesFromGraph( QStringList list ) {
    // Pre ciastocne cesty zisti, ci sa hrana uz v grafe nenachadza a nasledne vymaz cielovy uzol
    for( int i = 0; i < list.size() - 1; i++ ) {
        // Ziskanie cesty zdrojoveho a cieloveho uzla
        QString lNodeNameFrom = list.at( i );
        QString lNodeNameTo =  list.at( i + 1 );

        // Vyskladam nazov hrany spojenim zdrojoveho a cieloveho uzla
        QString lEdgeName =  lNodeNameFrom + lNodeNameTo;

        // Ak hrana v grafe uz neexistuje, tak vymaz hranu
        if( !this->getActiveGraph()->removeEdgeOccurence( lEdgeName ) ) {
            this->getActiveGraph()->removeEdge( this->getActiveGraph()->findEdgeByName( lEdgeName ) );
        }
    }
}

void Git::GitGraphUpdater::modifyNodesInGraph( QStringList list ) {

}