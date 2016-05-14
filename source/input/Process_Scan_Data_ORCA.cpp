/*
 * Process_Scan_Data.cpp
 *
 *  Created on: 19.12.2015
 *      Author: DetlevCM
 */


#include<Headers.h>


void Process_Scan_Data_ORCA(vector< OptPoints >& GaussianData, string filename)
{
	string line;
	OptPoints SingleOptPoint;
	vector< AtomDefinition > SingleMolecule;
	ifstream DataInputFile;
	DataInputFile.open(filename.c_str());

	while(DataInputFile.good())
	{

		getline(DataInputFile,line);

		//size_t found = line.find("Input orientation:");
		size_t found = line.find("CARTESIAN COORDINATES (A.U.)");
		// after standard orientation we get energy info

		if (found!=string::npos) // We have found a Z-Matrix
		{

			/*
			 * ----------------------------
			 * CARTESIAN COORDINATES (A.U.)
			 * ----------------------------
			 *   NO LB      ZA    FRAG    MASS        X           Y           Z
			 *   0 Al   13.0000    0    26.982         -0.130428198654859         -3.453158189291107         -2.445491104698375
			 *
			 */


			getline(DataInputFile,line);
			getline(DataInputFile,line);
			// pass the "gibberish" (header & table setup)


			getline(DataInputFile,line); // this is our first data setup




			SingleMolecule.clear();

			do{
				// now work on line
				AtomDefinition AtomsInMolecule;

				vector< double > temp;
				temp = Tokenise_String_To_Double(line," 	");

				// Now sort the vector temp...
				AtomsInMolecule.CenterID = (int) temp[0];
				AtomsInMolecule.AtomNumber = 0;//(int) temp[1];
				AtomsInMolecule.AtomType = 0;//(int) temp[2];
				AtomsInMolecule.X = temp[5];
				AtomsInMolecule.Y = temp[6];
				AtomsInMolecule.Z = temp[7];

				SingleMolecule.push_back(AtomsInMolecule);
				temp.clear(); // just in case

				getline(DataInputFile,line); // this is the following line
			}while(!line.empty()); // ORCA terminates cartesians with a blank line

		}


		// Energy in ORCA
		/*
		 * ----------------
		 * TOTAL SCF ENERGY
		 * ----------------
		 * Total Energy       :        -5722.49861017 Eh         -155717.10369 eV
		 */
		if (line.find("Total Energy       :")!=string::npos) // We have found energy
		{
			vector< double > temp;
			temp = Tokenise_String_To_Double(line," 	");
			SingleOptPoint.Energy = temp[3];
		}



		// Needs to find after each optimisation

		found = line.find("RELAXED SURFACE SCAN STEP");
		/*
		 *          *************************************************************
		 *          *               RELAXED SURFACE SCAN STEP   1               *
		 *          *                                                           *
		 *          *                 Bond ( 33,   6)  :   3.40909000           *
		 *          *************************************************************
		 */

		/*
		if (found!=string::npos) // We have found Step Identification
		{
			char * cstr, *p;
			string str = line;
			vector< string > temp;

			cstr = new char [str.size()+1];
			strcpy (cstr, str.c_str());

			p=strtok (cstr," 	");
			while (p!=NULL)
			{
				temp.push_back(p);
				//cout << p << "\n";
				p=strtok(NULL," 	");
			}
			delete[] cstr;
			delete[] p;


			// Now pick the points we need
			// 2, 8, 12, 14

			SingleOptPoint.ScanStep = (int) strtod(temp[5].c_str(),NULL);
			SingleOptPoint.OptStep = 0;//(int) strtod(temp[8].c_str(),NULL);
			SingleOptPoint.OptStepMax = 0;//(int) strtod(temp[12].c_str(),NULL);
			SingleOptPoint.Molecule = SingleMolecule;
			//SingleOptPoint.Energy = energy;

			GaussianData.push_back(SingleOptPoint);
			SingleMolecule.clear();
		}
//*/

		if(SingleMolecule.size() > 0)
		{
			// Now pick the points we need
			// 2, 8, 12, 14
			//		SingleOptPoint.ScanStep = strtod(temp[2].c_str(),NULL);
			//		SingleOptPoint.OptStep = strtod(temp[8].c_str(),NULL);
			//		SingleOptPoint.OptStepMax = strtod(temp[12].c_str(),NULL);
			SingleOptPoint.Molecule = SingleMolecule;
			//SingleOptPoint.Energy = energy;

			GaussianData.push_back(SingleOptPoint);
			SingleMolecule.clear();

		}


	}


	if((int) SingleMolecule.size() > 0)
	{
		SingleOptPoint.ScanStep = 0;
		SingleOptPoint.OptStep = 0;
		SingleOptPoint.OptStepMax = 0;
		SingleOptPoint.Molecule = SingleMolecule;

		GaussianData.push_back(SingleOptPoint);
		SingleMolecule.clear();
	}

}
