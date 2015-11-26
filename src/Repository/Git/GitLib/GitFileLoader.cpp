#include "GitLib/GitFileLoader.h"
#include "GitLib/GitVersion.h"
#include "GitLib/GitFile.h"
#include "GitLib/GitFileDiffBlock.h"
#include "GitLib/GitFileDiffBlockLine.h"
	: filePath( "" ), extensions( "" )
	QList<Repository::Git::GitVersion*> versions = QList<Repository::Git::GitVersion*>();;
			Repository::Git::GitVersion* version = new Repository::Git::GitVersion( line );
					Repository::Git::GitFile* file = new Repository::Git::GitFile( line.mid( line.lastIndexOf( "/" ) + 1 ), line, Repository::Git::GitType::ADDED );
					Repository::Git::GitFile* file = new Repository::Git::GitFile( line.mid( line.lastIndexOf( "/" ) + 1 ), line, Repository::Git::GitType::MODIFIED );
					Repository::Git::GitFile* file = new Repository::Git::GitFile( line.mid( line.lastIndexOf( "/" ) + 1 ), line, Repository::Git::GitType::REMOVED );
		QProcess process;
		process.setStandardOutputFile( QDir::toNativeSeparators( tempFile.fileName() ) );
		process.start( lCommand );
		process.waitForFinished();
		process.close();
		process.terminate();
		Repository::Git::GitFileDiffBlock* diffBlock = new Repository::Git::GitFileDiffBlock();
			Repository::Git::GitFileDiffBlockLine* lDiffBlockLine = new Repository::Git::GitFileDiffBlockLine( lLine, lLineNumber++, Repository::Git::GitType::ADDED );
		Repository::Git::GitFileDiffBlock* diffBlock = NULL;
			Repository::Git::GitFileDiffBlockLine* blockLine;
					diffBlock = new Repository::Git::GitFileDiffBlock();
					diffBlock->setRemoveStart( lLine.mid( 4, firstComma - 4 ).toInt() );
					diffBlock->setRemoveCount( lLine.mid( firstComma + 1, middleSpace - ( firstComma + 1 ) ).toInt() );
					diffBlock->setAddStart( lLine.mid( middleSpace + 2, secondComma - ( middleSpace + 2 ) ).toInt() );
					diffBlock->setAddCount( lLine.mid( secondComma + 1, endSpace - ( secondComma + 1 ) ).toInt() );
					blockLine = new Repository::Git::GitFileDiffBlockLine( lLine, addPos++ , Repository::Git::GitType::ADDED );
					blockLine = new Repository::Git::GitFileDiffBlockLine( lLine, removePos++ , Repository::Git::GitType::REMOVED );
					blockLine = new Repository::Git::GitFileDiffBlockLine( lLine, addPos++ , Repository::Git::GitType::NONE );
	if ( gitFile->getType() == Repository::Git::GitType::ADDED ) {
		lCommand = "git show " + currentCommitId + ":" + lFile;
		lCommand = "git diff -u " + oldCommitId + " " + currentCommitId + " -- " + lFile;
	if ( gitFile->getType() == Repository::Git::GitType::ADDED ) {