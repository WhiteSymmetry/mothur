/*
 *  splitabundcommand.cpp
 *  Mothur
 *
 *  Created by westcott on 5/17/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "splitabundcommand.h"

//**********************************************************************************************************************
vector<string> SplitAbundCommand::setParameters(){	
	try {		
		CommandParameter pfasta("fasta", "InputTypes", "", "", "none", "none", "none",false,true); parameters.push_back(pfasta);
		CommandParameter pname("name", "InputTypes", "", "", "none", "FNGLT", "none",false,false); parameters.push_back(pname);
		CommandParameter pgroup("group", "InputTypes", "", "", "none", "none", "none",false,false); parameters.push_back(pgroup);
		CommandParameter plist("list", "InputTypes", "", "", "none", "FNGLT", "none",false,false); parameters.push_back(plist);
		CommandParameter plabel("label", "String", "", "", "", "", "",false,false); parameters.push_back(plabel);
		CommandParameter pcutoff("cutoff", "Number", "", "0", "", "", "",false,true); parameters.push_back(pcutoff);
		CommandParameter pgroups("groups", "String", "", "", "", "", "",false,false); parameters.push_back(pgroups);
		CommandParameter paccnos("accnos", "Boolean", "", "F", "", "", "",false,false); parameters.push_back(paccnos);
		CommandParameter pinputdir("inputdir", "String", "", "", "", "", "",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string SplitAbundCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The split.abund command reads a fasta file and a list or a names file splits the sequences into rare and abundant groups. \n";
		helpString += "The split.abund command parameters are fasta, list, name, cutoff, group, label, groups, cutoff and accnos.\n";
		helpString += "The fasta and a list or name parameter are required, and you must provide a cutoff value.\n";
		helpString += "The cutoff parameter is used to qualify what is abundant and rare.\n";
		helpString += "The group parameter allows you to parse a group file into rare and abundant groups.\n";
		helpString += "The label parameter is used to read specific labels in your listfile you want to use.\n";
		helpString += "The accnos parameter allows you to output a .rare.accnos and .abund.accnos files to use with the get.seqs and remove.seqs commands.\n";
		helpString += "The groups parameter allows you to parse the files into rare and abundant files by group.  \n";
		helpString += "For example if you set groups=A-B-C, you will get a .A.abund, .A.rare, .B.abund, .B.rare, .C.abund, .C.rare files.  \n";
		helpString += "If you want .abund and .rare files for all groups, set groups=all.  \n";
		helpString += "The split.abund command should be used in the following format: split.abund(fasta=yourFasta, list=yourListFile, group=yourGroupFile, label=yourLabels, cutoff=yourCutoff).\n";
		helpString += "Example: split.abund(fasta=abrecovery.fasta, list=abrecovery.fn.list, group=abrecovery.groups, label=0.03, cutoff=2).\n";
		helpString += "Note: No spaces between parameter labels (i.e. list), '=' and parameters (i.e.yourListfile).\n";
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "getHelpString");
		exit(1);
	}
}

//**********************************************************************************************************************
SplitAbundCommand::SplitAbundCommand(){	
	try {
		abort = true; calledHelp = true; 
		setParameters();
		vector<string> tempOutNames;
		outputTypes["list"] = tempOutNames;
		outputTypes["name"] = tempOutNames;
		outputTypes["accnos"] = tempOutNames;
		outputTypes["group"] = tempOutNames;
		outputTypes["fasta"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "SplitAbundCommand");
		exit(1);
	}
}
//**********************************************************************************************************************
SplitAbundCommand::SplitAbundCommand(string option)  {
	try {
		abort = false; calledHelp = false;   
		allLines = 1;
			
		//allow user to run help
		if(option == "help") { help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		else {
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string, string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			map<string, string>::iterator it;
		
			//check to make sure all parameters are valid for command
			for (it = parameters.begin(); it != parameters.end(); it++) { 
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
			
			//initialize outputTypes
			vector<string> tempOutNames;
			outputTypes["list"] = tempOutNames;
			outputTypes["name"] = tempOutNames;
			outputTypes["accnos"] = tempOutNames;
			outputTypes["group"] = tempOutNames;
			outputTypes["fasta"] = tempOutNames;			
												
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.validFile(parameters, "inputdir", false);		
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("list");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["list"] = inputDir + it->second;		}
				}
				
				it = parameters.find("group");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["group"] = inputDir + it->second;		}
				}
				
				it = parameters.find("fasta");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["fasta"] = inputDir + it->second;		}
				}
				
				it = parameters.find("name");
				//user has given a template file
				if(it != parameters.end()){ 
					path = m->hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["name"] = inputDir + it->second;		}
				}

			}

			
			//if the user changes the output directory command factory will send this info to us in the output parameter 
			outputDir = validParameter.validFile(parameters, "outputdir", false);		if (outputDir == "not found"){	outputDir = "";	}

			//check for required parameters
			listfile = validParameter.validFile(parameters, "list", true);
			if (listfile == "not open") { abort = true; }
			else if (listfile == "not found") { listfile = ""; }
			else{ inputFile = listfile; m->setListFile(listfile); }	
			
			namefile = validParameter.validFile(parameters, "name", true);
			if (namefile == "not open") { abort = true; }
			else if (namefile == "not found") { namefile = ""; }	
			else{ inputFile = namefile; m->setNameFile(namefile); }	
		
			fastafile = validParameter.validFile(parameters, "fasta", true);
			if (fastafile == "not open") { abort = true; }
			else if (fastafile == "not found") { 				
				fastafile = m->getFastaFile(); 
				if (fastafile != "") { m->mothurOut("Using " + fastafile + " as input file for the fasta parameter."); m->mothurOutEndLine(); }
				else { 	m->mothurOut("You have no current fastafile and the fasta parameter is required."); m->mothurOutEndLine(); abort = true; }
			}else { m->setFastaFile(fastafile); }	
			
			groupfile = validParameter.validFile(parameters, "group", true);
			if (groupfile == "not open") {  groupfile = ""; abort = true; }	
			else if (groupfile == "not found") { groupfile = ""; }
			else {  
				groupMap = new GroupMap(groupfile);
				
				int error = groupMap->readMap();
				if (error == 1) { abort = true; }
				m->setGroupFile(groupfile);
			}
			
			groups = validParameter.validFile(parameters, "groups", false);		
			if (groups == "not found") { groups = ""; }
			else if (groups == "all") { 
				if (groupfile != "") {  Groups = groupMap->getNamesOfGroups();  } 
				else {  m->mothurOut("You cannot select groups without a valid groupfile, I will disregard your groups selection. "); m->mothurOutEndLine(); groups = "";   }
			}else { 
				m->splitAtDash(groups, Groups);
			}
			
			if ((groupfile == "") && (groups != "")) {  m->mothurOut("You cannot select groups without a valid groupfile, I will disregard your groups selection. "); m->mothurOutEndLine(); groups = "";  Groups.clear(); }
			
			//do you have all files needed
			if ((listfile == "") && (namefile == "")) { 
				namefile = m->getNameFile(); 
				if (namefile != "") { m->mothurOut("Using " + namefile + " as input file for the name parameter."); m->mothurOutEndLine(); }
				else { 				
					listfile = m->getListFile(); 
					if (listfile != "") { m->mothurOut("Using " + listfile + " as input file for the list parameter."); m->mothurOutEndLine(); }
					else { 	m->mothurOut("You have no current list or namefile and the list or name parameter is required."); m->mothurOutEndLine(); abort = true; }
				}
			}
			
			//check for optional parameter and set defaults
			// ...at some point should added some additional type checking...
			label = validParameter.validFile(parameters, "label", false);			
			if (label == "not found") { label = "";  allLines = 1; }
			else { 
				if(label != "all") {  m->splitAtDash(label, labels);  allLines = 0;  }
				else { allLines = 1;  }
			}
			
			string temp = validParameter.validFile(parameters, "accnos", false);		if (temp == "not found") { temp = "F"; }
			accnos = m->isTrue(temp); 
			
			temp = validParameter.validFile(parameters, "cutoff", false);				if (temp == "not found") { temp = "0"; }
			m->mothurConvert(temp, cutoff); 

			if (cutoff == 0) {  m->mothurOut("You must provide a cutoff to qualify what is abundant for the split.abund command. "); m->mothurOutEndLine(); abort = true;  }

		}

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "SplitAbundCommand");
		exit(1);
	}
}
//**********************************************************************************************************************
SplitAbundCommand::~SplitAbundCommand(){ 
	if (groupfile != "") {  delete groupMap;  } 
}
//**********************************************************************************************************************
int SplitAbundCommand::execute(){
	try {
	
		if (abort == true) { if (calledHelp) { return 0; }  return 2;	}
		
		if (listfile != "") { //you are using a listfile to determine abundance
			if (outputDir == "") { outputDir = m->hasPath(listfile); }
			
			//if the users enters label "0.06" and there is no "0.06" in their file use the next lowest label.
			set<string> processedLabels;
			set<string> userLabels = labels;	
			
			input = new InputData(listfile, "list");
			list = input->getListVector();
			string lastLabel = list->getLabel();
			
			//do you have a namefile or do we need to similate one?
			if (namefile != "") {  readNamesFile();		}
			else				{ createNameMap(list);	}
			
			if (m->control_pressed) { delete input; delete list; for (int i = 0; i < outputNames.size(); i++) {	m->mothurRemove(outputNames[i]); } return 0; }
			
			while((list != NULL) && ((allLines == 1) || (userLabels.size() != 0))) {
			
				if (m->control_pressed) { delete input; delete list; for (int i = 0; i < outputNames.size(); i++) {	m->mothurRemove(outputNames[i]); } return 0; }
				
				if(allLines == 1 || labels.count(list->getLabel()) == 1){
						
						m->mothurOut(list->getLabel()); m->mothurOutEndLine();
						splitList(list);
											
						processedLabels.insert(list->getLabel());
						userLabels.erase(list->getLabel());
				}
				
				if ((m->anyLabelsToProcess(list->getLabel(), userLabels, "") == true) && (processedLabels.count(lastLabel) != 1)) {
						string saveLabel = list->getLabel();
						
						delete list;
						list = input->getListVector(lastLabel); //get new list vector to process
						
						m->mothurOut(list->getLabel()); m->mothurOutEndLine();
						splitList(list);
						
						processedLabels.insert(list->getLabel());
						userLabels.erase(list->getLabel());
						
						//restore real lastlabel to save below
						list->setLabel(saveLabel);
				}
				
			
				lastLabel = list->getLabel();
					
				delete list;
				list = input->getListVector(); //get new list vector to process
			}
			
			if (m->control_pressed) { delete input;  for (int i = 0; i < outputNames.size(); i++) {	m->mothurRemove(outputNames[i]); } return 0; }
			
			//output error messages about any remaining user labels
			set<string>::iterator it;
			bool needToRun = false;
			for (it = userLabels.begin(); it != userLabels.end(); it++) {  
				m->mothurOut("Your file does not include the label " + *it); 
				if (processedLabels.count(lastLabel) != 1) {
					m->mothurOut(". I will use " + lastLabel + "."); m->mothurOutEndLine();
					needToRun = true;
				}else {
					m->mothurOut(". Please refer to " + lastLabel + "."); m->mothurOutEndLine();
				}

			}
			
			if (m->control_pressed) { delete input;  for (int i = 0; i < outputNames.size(); i++) {	m->mothurRemove(outputNames[i]); } return 0; }
			
			//run last label if you need to
			if (needToRun == true)  {
				if (list != NULL) {	delete list;	}
				list = input->getListVector(lastLabel); //get new list vector to process
				
				m->mothurOut(list->getLabel()); m->mothurOutEndLine();
				splitList(list);		
				
				delete list;
			}
			
			delete input;
			
			if (m->control_pressed) { for (int i = 0; i < outputNames.size(); i++) {	m->mothurRemove(outputNames[i]); }	return 0;	}
									
		}else { //you are using the namefile to determine abundance
			if (outputDir == "") { outputDir = m->hasPath(namefile); }
			
			splitNames(); 
			writeNames();
			
			string tag = "";
			if (groupfile != "")				{  parseGroup(tag);		}
			if (accnos)							{  writeAccnos(tag);	}
			if (fastafile != "")				{  parseFasta(tag);		}
		}
		
		//set fasta file as new current fastafile
		string current = "";
		itTypes = outputTypes.find("fasta");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { current = (itTypes->second)[0]; m->setFastaFile(current); }
		}
		
		itTypes = outputTypes.find("name");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { current = (itTypes->second)[0]; m->setNameFile(current); }
		}
		
		itTypes = outputTypes.find("group");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { current = (itTypes->second)[0]; m->setGroupFile(current); }
		}
		
		itTypes = outputTypes.find("list");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { current = (itTypes->second)[0]; m->setListFile(current); }
		}
		
		itTypes = outputTypes.find("accnos");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { current = (itTypes->second)[0]; m->setAccnosFile(current); }
		}
		
		m->mothurOutEndLine();
		m->mothurOut("Output File Names: "); m->mothurOutEndLine();
		for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i]); m->mothurOutEndLine();	}
		m->mothurOutEndLine();
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "execute");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::splitList(ListVector* thisList) {
	try {
		rareNames.clear();
		abundNames.clear();
		
		//get rareNames and abundNames
		for (int i = 0; i < thisList->getNumBins(); i++) {
			if (m->control_pressed) { return 0; }
			
			string bin = thisList->get(i);
						
			vector<string> names;
			m->splitAtComma(bin, names);  //parses bin into individual sequence names
			int size = names.size();
				
			if (size <= cutoff) {
				for (int j = 0; j < names.size(); j++) {  rareNames.insert(names[j]);  }
			}else{
				for (int j = 0; j < names.size(); j++) {  abundNames.insert(names[j]);  }
			}
		}//end for

		
		string tag = thisList->getLabel() + ".";
		
		writeList(thisList, tag);
		
		if (groupfile != "")				{  parseGroup(tag);		}
		if (accnos)							{  writeAccnos(tag);	}
		if (fastafile != "")				{  parseFasta(tag);		}
		
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "splitList");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::writeList(ListVector* thisList, string tag) { 
	try {
		
		map<string, ofstream*> filehandles;
		
		if (Groups.size() == 0) {
			SAbundVector* sabund = new SAbundVector();
			*sabund = thisList->getSAbundVector();
		
			//find out how many bins are rare and how many are abundant so you can process the list vector one bin at a time
			// and don't have to store the bins until you are done with the whole vector, this save alot of space.
			int numRareBins = 0;
			for (int i = 0; i <= sabund->getMaxRank(); i++) {
				if (i > cutoff) { break; }
				numRareBins += sabund->get(i);
			}
			int numAbundBins = thisList->getNumBins() - numRareBins;
			delete sabund;

			ofstream aout;
			ofstream rout;
			
			string rare = outputDir + m->getRootName(m->getSimpleName(listfile)) + tag + "rare.list";
			m->openOutputFile(rare, rout);
			outputNames.push_back(rare); outputTypes["list"].push_back(rare);
			
			string abund = outputDir + m->getRootName(m->getSimpleName(listfile)) + tag + "abund.list";
			m->openOutputFile(abund, aout);
			outputNames.push_back(abund); outputTypes["list"].push_back(abund);

			if (rareNames.size() != 0)	{  rout << thisList->getLabel() << '\t' << numRareBins << '\t';		}
			if (abundNames.size() != 0) {	aout << thisList->getLabel() << '\t' << numAbundBins << '\t';	}

			for (int i = 0; i < thisList->getNumBins(); i++) {
				if (m->control_pressed) { break; }
			
				string bin = list->get(i); 
			
				int size = m->getNumNames(bin);
			
				if (size <= cutoff) {  rout << bin << '\t';  }
				else				{  aout << bin << '\t'; }
			}
			
			if (rareNames.size() != 0)	{ rout << endl; }
			if (abundNames.size() != 0) { aout << endl; }
			
			rout.close();
			aout.close();
			
		}else{ //parse names by abundance and group
			string fileroot =  outputDir + m->getRootName(m->getSimpleName(listfile));
			ofstream* temp;
			ofstream* temp2;
			//map<string, bool> wroteFile;
			map<string, ofstream*> filehandles;
			map<string, ofstream*>::iterator it3;

			for (int i=0; i<Groups.size(); i++) {
				temp = new ofstream;
				filehandles[Groups[i]+".rare"] = temp;
				temp2 = new ofstream;
				filehandles[Groups[i]+".abund"] = temp2;
				
				m->openOutputFile(fileroot + Groups[i] + tag + ".rare.list", *(filehandles[Groups[i]+".rare"]));
				m->openOutputFile(fileroot + Groups[i] + tag + ".abund.list", *(filehandles[Groups[i]+".abund"]));
				outputNames.push_back(fileroot + Groups[i] + tag + ".rare.list"); outputTypes["list"].push_back(fileroot + Groups[i] + tag + ".rare.list");
				outputNames.push_back(fileroot + Groups[i] + tag + ".abund.list"); outputTypes["list"].push_back(fileroot + Groups[i] + tag + ".abund.list");
			}
			
			map<string, string> groupVector;
			map<string, string>::iterator itGroup;
			map<string, int> groupNumBins;
		
			for (it3 = filehandles.begin(); it3 != filehandles.end(); it3++) {
				groupNumBins[it3->first] = 0;
				groupVector[it3->first] = "";
			}
		
			for (int i = 0; i < thisList->getNumBins(); i++) {
				if (m->control_pressed) { break; }
			
				map<string, string> groupBins;
				string bin = list->get(i); 
			
				vector<string> names;
				m->splitAtComma(bin, names);  //parses bin into individual sequence names
			
				//parse bin into list of sequences in each group
				for (int j = 0; j < names.size(); j++) {
					string rareAbund;
					if (rareNames.count(names[j]) != 0) { //you are a rare name
						rareAbund = ".rare";
					}else{ //you are a abund name
						rareAbund = ".abund";
					}
					
					string group = groupMap->getGroup(names[j]);
				
					if (m->inUsersGroups(group, Groups)) { //only add if this is in a group we want
						itGroup = groupBins.find(group+rareAbund);
						if(itGroup == groupBins.end()) {
							groupBins[group+rareAbund] = names[j];  //add first name
							groupNumBins[group+rareAbund]++;
						}else{ //add another name
							groupBins[group+rareAbund] +=  "," + names[j];
						}
					}else if(group == "not found") {
						m->mothurOut(names[j] + " is not in your groupfile. Ignoring."); m->mothurOutEndLine();
					}
				}
			
			
				for (itGroup = groupBins.begin(); itGroup != groupBins.end(); itGroup++) {
					groupVector[itGroup->first] +=  itGroup->second + '\t'; 
				}
			}
			
			//end list vector
			for (it3 = filehandles.begin(); it3 != filehandles.end(); it3++) {
				(*(filehandles[it3->first])) << thisList->getLabel() << '\t' << groupNumBins[it3->first] << '\t' << groupVector[it3->first] << endl;  // label numBins  listvector for that group
				(*(filehandles[it3->first])).close();
				delete it3->second;
			}
		}
		
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "writeList");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::splitNames() { //namefile
	try {
		
		rareNames.clear();
		abundNames.clear();	
			
		//open input file
		ifstream in;
		m->openInputFile(namefile, in);
		
		while (!in.eof()) {
			if (m->control_pressed) { break; }
			
			string firstCol, secondCol;
			in >> firstCol >> secondCol; m->gobble(in);
			
			nameMap[firstCol] = secondCol;
			
			int size = m->getNumNames(secondCol);
				
			if (size <= cutoff) {
				rareNames.insert(firstCol); 
			}else{
				abundNames.insert(firstCol); 
			}
		}
		in.close();
				
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "splitNames");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::readNamesFile() { 
	try {
		//open input file
		ifstream in;
		m->openInputFile(namefile, in);
		
		while (!in.eof()) {
			if (m->control_pressed) { break; }
			
			string firstCol, secondCol;
			in >> firstCol >> secondCol; m->gobble(in);
			
			nameMap[firstCol] = secondCol;
		}
		in.close();
				
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "readNamesFile");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::createNameMap(ListVector* thisList) {
	try {
		
		if (thisList != NULL) {
			for (int i = 0; i < thisList->getNumBins(); i++) {
				if (m->control_pressed) { return 0; }
				
				string bin = thisList->get(i);
							
				vector<string> names;
				m->splitAtComma(bin, names);  //parses bin into individual sequence names
				
				for (int j = 0; j < names.size(); j++) {  nameMap[names[j]] = names[j];  }
			}//end for
		}
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "createNameMap");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::writeNames() { //namefile
	try {
		
		map<string, ofstream*> filehandles;

		if (Groups.size() == 0) {
			ofstream aout;
			ofstream rout;
			
			string rare = outputDir + m->getRootName(m->getSimpleName(namefile))  + "rare.names";
			m->openOutputFile(rare, rout);
			outputNames.push_back(rare); outputTypes["name"].push_back(rare);
			
			string abund = outputDir + m->getRootName(m->getSimpleName(namefile))  + "abund.names";
			m->openOutputFile(abund, aout);
			outputNames.push_back(abund); outputTypes["name"].push_back(abund);
			
			if (rareNames.size() != 0) {
				for (set<string>::iterator itRare = rareNames.begin(); itRare != rareNames.end(); itRare++) {
					rout << (*itRare) << '\t' << nameMap[(*itRare)] << endl;
				}
			}
			rout.close();
			
			if (abundNames.size() != 0) {
				for (set<string>::iterator itAbund = abundNames.begin(); itAbund != abundNames.end(); itAbund++) {
					aout << (*itAbund) << '\t' << nameMap[(*itAbund)] << endl;
				}
			}
			aout.close();
			
		}else{ //parse names by abundance and group
			string fileroot =  outputDir + m->getRootName(m->getSimpleName(namefile));
			ofstream* temp;
			ofstream* temp2;
			map<string, ofstream*> filehandles;
			map<string, ofstream*>::iterator it3;

			for (int i=0; i<Groups.size(); i++) {
				temp = new ofstream;
				filehandles[Groups[i]+".rare"] = temp;
				temp2 = new ofstream;
				filehandles[Groups[i]+".abund"] = temp2;
				
				m->openOutputFile(fileroot + Groups[i] + ".rare.names", *(filehandles[Groups[i]+".rare"]));
				m->openOutputFile(fileroot + Groups[i] + ".abund.names", *(filehandles[Groups[i]+".abund"]));
			}
			
			for (map<string, string>::iterator itName = nameMap.begin(); itName != nameMap.end(); itName++) {				
				vector<string> names;
				m->splitAtComma(itName->second, names);  //parses bin into individual sequence names
				
				string rareAbund;
				if (rareNames.count(itName->first) != 0) { //you are a rare name
						rareAbund = ".rare";
				}else{ //you are a abund name
						rareAbund = ".abund";
				}
				
				map<string, string> outputStrings;
				map<string, string>::iterator itout;
				for (int i = 0; i < names.size(); i++) {
					
					string group = groupMap->getGroup(names[i]);
					
					if (m->inUsersGroups(group, Groups)) { //only add if this is in a group we want
						itout = outputStrings.find(group+rareAbund);
						if (itout == outputStrings.end()) {  
							outputStrings[group+rareAbund] = names[i] + '\t' + names[i];
						}else {   outputStrings[group+rareAbund] += "," + names[i]; }
					}else if(group == "not found") {
						m->mothurOut(names[i] + " is not in your groupfile. Ignoring."); m->mothurOutEndLine();
					}
				}
				
				for (itout = outputStrings.begin(); itout != outputStrings.end(); itout++) { *(filehandles[itout->first]) << itout->second << endl;	}
			}
			
			
			for (it3 = filehandles.begin(); it3 != filehandles.end(); it3++) { 
				(*(filehandles[it3->first])).close();
				outputNames.push_back(fileroot + it3->first + ".names");  outputTypes["name"].push_back(fileroot + it3->first + ".names");
				delete it3->second;
			}
		}
				
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "writeNames");
		exit(1);
	}
}
/**********************************************************************************************************************/
//just write the unique names - if a namesfile is given
int SplitAbundCommand::writeAccnos(string tag) { 
	try {
		
		map<string, ofstream*> filehandles;
		
		if (Groups.size() == 0) {
			ofstream aout;
			ofstream rout;
			
			
			string rare = outputDir + m->getRootName(m->getSimpleName(inputFile))  + tag + "rare.accnos";
			m->openOutputFile(rare, rout);
			outputNames.push_back(rare); outputTypes["accnos"].push_back(rare); 
			
			for (set<string>::iterator itRare = rareNames.begin(); itRare != rareNames.end(); itRare++) {
				rout << (*itRare) << endl;
			}
			rout.close();
		
			string abund = outputDir + m->getRootName(m->getSimpleName(inputFile)) + tag  + "abund.accnos";
			m->openOutputFile(abund, aout);
			outputNames.push_back(abund); outputTypes["accnos"].push_back(abund);
			
			for (set<string>::iterator itAbund = abundNames.begin(); itAbund != abundNames.end(); itAbund++) {
				aout << (*itAbund) << endl;
			}
			aout.close();
			
		}else{ //parse names by abundance and group
			string fileroot =  outputDir + m->getRootName(m->getSimpleName(inputFile));
			ofstream* temp;
			ofstream* temp2;
			map<string, ofstream*> filehandles;
			map<string, ofstream*>::iterator it3;
			
			for (int i=0; i<Groups.size(); i++) {
				temp = new ofstream;
				filehandles[Groups[i]+".rare"] = temp;
				temp2 = new ofstream;
				filehandles[Groups[i]+".abund"] = temp2;
				
				m->openOutputFile(fileroot + tag + Groups[i] + ".rare.accnos", *(filehandles[Groups[i]+".rare"]));
				m->openOutputFile(fileroot + tag + Groups[i] + ".abund.accnos", *(filehandles[Groups[i]+".abund"]));
			}
			
			//write rare
			for (set<string>::iterator itRare = rareNames.begin(); itRare != rareNames.end(); itRare++) {
					string group = groupMap->getGroup(*itRare);
					
					if (m->inUsersGroups(group, Groups)) { //only add if this is in a group we want
						*(filehandles[group+".rare"]) << *itRare << endl;
					}
			}
				
			//write abund	
			for (set<string>::iterator itAbund = abundNames.begin(); itAbund != abundNames.end(); itAbund++) {
					string group = groupMap->getGroup(*itAbund);
					
					if (m->inUsersGroups(group, Groups)) { //only add if this is in a group we want
						*(filehandles[group+".abund"]) << *itAbund << endl;
					}
			}
			
			//close files
			for (it3 = filehandles.begin(); it3 != filehandles.end(); it3++) { 
				(*(filehandles[it3->first])).close();
				outputNames.push_back(fileroot + tag + it3->first + ".accnos");  outputTypes["accnos"].push_back(fileroot + tag + it3->first + ".accnos");
				delete it3->second;
			}
		}
				
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "writeAccnos");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::parseGroup(string tag) { //namefile
	try {
		
		map<string, ofstream*> filehandles;
	
		if (Groups.size() == 0) {
			ofstream aout;
			ofstream rout;
			
			string rare = outputDir + m->getRootName(m->getSimpleName(groupfile))  + tag + "rare.groups";
			m->openOutputFile(rare, rout);
			outputNames.push_back(rare); outputTypes["group"].push_back(rare);
		
			string abund = outputDir + m->getRootName(m->getSimpleName(groupfile))  + tag + "abund.groups";
			m->openOutputFile(abund, aout);
			outputNames.push_back(abund); outputTypes["group"].push_back(abund);
			
			for (map<string, string>::iterator itName = nameMap.begin(); itName != nameMap.end(); itName++) {				
				vector<string> names;
				m->splitAtComma(itName->second, names);  //parses bin into individual sequence names
				
				for (int i = 0; i < names.size(); i++) {
				
					string group = groupMap->getGroup(names[i]);
				
					if (group == "not found") { 
						m->mothurOut(names[i] + " is not in your groupfile, ignoring, please correct."); m->mothurOutEndLine();
					}else {
						if (rareNames.count(itName->first) != 0) { //you are a rare name
							rout << names[i] << '\t' << group << endl;
						}else{ //you are a abund name
							aout << names[i] << '\t' << group << endl;
						}
					}
				}
			}
			
			rout.close(); 
			aout.close(); 

		}else{ //parse names by abundance and group
			string fileroot =  outputDir + m->getRootName(m->getSimpleName(groupfile));
			ofstream* temp;
			ofstream* temp2;
			map<string, ofstream*> filehandles;
			map<string, ofstream*>::iterator it3;

			for (int i=0; i<Groups.size(); i++) {
				temp = new ofstream;
				filehandles[Groups[i]+".rare"] = temp;
				temp2 = new ofstream;
				filehandles[Groups[i]+".abund"] = temp2;
				
				m->openOutputFile(fileroot + tag + Groups[i] + ".rare.groups", *(filehandles[Groups[i]+".rare"]));
				m->openOutputFile(fileroot + tag + Groups[i] + ".abund.groups", *(filehandles[Groups[i]+".abund"]));
			}
			
			for (map<string, string>::iterator itName = nameMap.begin(); itName != nameMap.end(); itName++) {				
				vector<string> names;
				m->splitAtComma(itName->second, names);  //parses bin into individual sequence names
				
				string rareAbund;
				if (rareNames.count(itName->first) != 0) { //you are a rare name
					rareAbund = ".rare";
				}else{ //you are a abund name
					rareAbund = ".abund";
				}
				
				for (int i = 0; i < names.size(); i++) {
				
					string group = groupMap->getGroup(names[i]);
									
					if (m->inUsersGroups(group, Groups)) { //only add if this is in a group we want
						*(filehandles[group+rareAbund]) << names[i] << '\t' << group << endl;
					}
				}
			}
			
			for (it3 = filehandles.begin(); it3 != filehandles.end(); it3++) { 
				(*(filehandles[it3->first])).close();
				outputNames.push_back(fileroot + tag + it3->first + ".groups");  outputTypes["group"].push_back(fileroot + tag + it3->first + ".groups");
				delete it3->second;
			}
		}
				
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "parseGroups");
		exit(1);
	}
}
/**********************************************************************************************************************/
int SplitAbundCommand::parseFasta(string tag) { //namefile
	try {
		
		map<string, ofstream*> filehandles;
		
		if (Groups.size() == 0) {
			ofstream aout;
			ofstream rout;
			
			string rare = outputDir + m->getRootName(m->getSimpleName(fastafile))  + tag + "rare.fasta";
			m->openOutputFile(rare, rout);
			outputNames.push_back(rare); outputTypes["fasta"].push_back(rare);
		
			string abund = outputDir + m->getRootName(m->getSimpleName(fastafile))  + tag + "abund.fasta";
			m->openOutputFile(abund, aout);
			outputNames.push_back(abund); outputTypes["fasta"].push_back(abund);
		
			//open input file
			ifstream in;
			m->openInputFile(fastafile, in);
	
			while (!in.eof()) {
				if (m->control_pressed) { break; }
		
				Sequence seq(in); m->gobble(in);
				
				if (seq.getName() != "") { 
					
					map<string, string>::iterator itNames;
					
					itNames = nameMap.find(seq.getName());
					
					if (itNames == nameMap.end()) {
						m->mothurOut(seq.getName() + " is not in your namesfile, ignoring."); m->mothurOutEndLine();
					}else{
						if (rareNames.count(seq.getName()) != 0) { //you are a rare name
							seq.printSequence(rout);
						}else{ //you are a abund name
							seq.printSequence(aout);
						}
					}
				}
			}
			in.close();
			rout.close(); 
			aout.close(); 

		}else{ //parse names by abundance and group
			string fileroot =  outputDir + m->getRootName(m->getSimpleName(fastafile));
			ofstream* temp;
			ofstream* temp2;
			map<string, ofstream*> filehandles;
			map<string, ofstream*>::iterator it3;

			for (int i=0; i<Groups.size(); i++) {
				temp = new ofstream;
				filehandles[Groups[i]+".rare"] = temp;
				temp2 = new ofstream;
				filehandles[Groups[i]+".abund"] = temp2;
				
				m->openOutputFile(fileroot + tag + Groups[i] + ".rare.fasta", *(filehandles[Groups[i]+".rare"]));
				m->openOutputFile(fileroot + tag + Groups[i] + ".abund.fasta", *(filehandles[Groups[i]+".abund"]));
			}
			
			//open input file
			ifstream in;
			m->openInputFile(fastafile, in);
	
			while (!in.eof()) {
				if (m->control_pressed) { break; }
		
				Sequence seq(in); m->gobble(in);
				
				if (seq.getName() != "") { 
					map<string, string>::iterator itNames = nameMap.find(seq.getName());
					
					if (itNames == nameMap.end()) {
						m->mothurOut(seq.getName() + " is not in your namesfile, ignoring."); m->mothurOutEndLine();
					}else{
						vector<string> names;
						m->splitAtComma(itNames->second, names);  //parses bin into individual sequence names
				
						string rareAbund;
						if (rareNames.count(itNames->first) != 0) { //you are a rare name
							rareAbund = ".rare";
						}else{ //you are a abund name
							rareAbund = ".abund";
						}
				
						for (int i = 0; i < names.size(); i++) {
				
							string group = groupMap->getGroup(seq.getName());
					
							if (m->inUsersGroups(group, Groups)) { //only add if this is in a group we want
								seq.printSequence(*(filehandles[group+rareAbund]));
							}else if(group == "not found") {
								m->mothurOut(seq.getName() + " is not in your groupfile. Ignoring."); m->mothurOutEndLine();
							}
						}
					}
				}
			}
			in.close();
			
			for (it3 = filehandles.begin(); it3 != filehandles.end(); it3++) { 
				(*(filehandles[it3->first])).close();
				outputNames.push_back(fileroot + tag + it3->first + ".fasta");  outputTypes["fasta"].push_back(fileroot + tag + it3->first + ".fasta");
				delete it3->second;
			}
		}
				
		return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "SplitAbundCommand", "parseFasta");
		exit(1);
	}
}
/**********************************************************************************************************************/

