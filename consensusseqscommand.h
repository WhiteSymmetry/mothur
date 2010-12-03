#ifndef CONSENSUSSEQSCOMMAND_H
#define CONSENSUSSEQSCOMMAND_H

/*
 *  consensusseqscommand.h
 *  Mothur
 *
 *  Created by westcott on 11/23/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */


#include "command.hpp"
#include "listvector.hpp"

class ConsensusSeqsCommand : public Command {
public:
	ConsensusSeqsCommand(string);
	ConsensusSeqsCommand();
	~ConsensusSeqsCommand();
	vector<string> getRequiredParameters();
	vector<string> getValidParameters();
	vector<string> getRequiredFiles();
	map<string, vector<string> > getOutputFiles() { return outputTypes; }
	int execute();
	void help();
	
private:
	
	bool abort, allLines;
	string fastafile, listfile, namefile, label, outputDir;
	set<string> labels;
	vector<string> outputNames;
	map<string, vector<string> > outputTypes;
	map<string, string> fastaMap;
	map<string, string> nameMap;
	map<string, string> nameFileMap;
	
	int readFasta();
	int readNames();
	int processList(ListVector*&);
	string getConsSeq(string, ofstream&, string&, int);
	char getBase(vector<int>);
};

#endif



