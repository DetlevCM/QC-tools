/*
 * Process_Scan_Data.cpp
 *
 *  Created on: 19.12.2015
 *      Author: DetlevCM
 */


#include<Headers.h>


void Process_Opt_Data_ORCA(vector< OptPoints >& GaussianData, string filename)
{
	string line;
	OptPoints SingleOptPoint;
	vector< AtomDefinition > SingleMolecule;
	ifstream DataInputFile;
	DataInputFile.open(filename.c_str());

	while(DataInputFile.good())
	{
		getline(DataInputFile,line);

		/*
		 * ----------------------------
		 * CARTESIAN COORDINATES (A.U.)
		 * ----------------------------
		 *   NO LB      ZA    FRAG    MASS        X           Y           Z
		 *   0 Al   13.0000    0    26.982         -0.130428198654859         -3.453158189291107         -2.445491104698375
		 */

		/*
		 * ---------------------------------
		 * CARTESIAN COORDINATES (ANGSTROEM)
		 * ---------------------------------
		 *   Al    -0.121057   -1.926636   -1.228221
		 *   Al    -2.931145   -1.943680   -0.234795
		 */

		// Speaking of Units...
		// http://www.translatorscafe.com/cafe/EN/units-converter/length/62-63/angstrom-a_u__of_length/
		// 1 Angstroem = 1.889725988579 atomic unit of length

		if (line.find("CARTESIAN COORDINATES (A.U.)")!=string::npos) // xyz matrix starts here
		//if (line.find("CARTESIAN COORDINATES (ANGSTROEM)")!=string::npos) // xyz matrix starts here
		{
			getline(DataInputFile,line);
			getline(DataInputFile,line);
			// pass the header & table setup

			getline(DataInputFile,line); // this is our first data setup

			SingleMolecule.clear();

			do{
				// now work on line
				AtomDefinition AtomsInMolecule;

				vector< double > temp;
				temp = Tokenise_String_To_Double(line," 	");

				// Now sort the vector temp...
				AtomsInMolecule.CenterID = (int) temp[0];
				AtomsInMolecule.AtomNumber = 0;
				AtomsInMolecule.AtomType = 0;
				AtomsInMolecule.X = temp[5];
				AtomsInMolecule.Y = temp[6];
				AtomsInMolecule.Z = temp[7];

				SingleMolecule.push_back(AtomsInMolecule);
				temp.clear(); // just in case

				getline(DataInputFile,line); // this is the following line
			}while(!line.empty()); // ORCA terminates cartesians with a blank line

		}

		if(line.find("Total Energy       ")!=string::npos) // We have found energy
		{
			vector< double > temp;
			temp = Tokenise_String_To_Double(line," 	");
			SingleOptPoint.Energy = temp[3];
		}

		// Energy in ORCA
		/*
		 * ----------------
		 * TOTAL SCF ENERGY
		 * ----------------
		 * Total Energy       :        -5722.49861017 Eh         -155717.10369 eV
		 */
		if(SingleMolecule.size() > 0)
		{
			// No longer used data points are set to zero
			SingleOptPoint.ScanStep = 0;
			SingleOptPoint.OptStep = 0;
			SingleOptPoint.OptStepMax = 0;
			SingleOptPoint.Molecule = SingleMolecule;

			GaussianData.push_back(SingleOptPoint);
			SingleMolecule.clear();
		}

	}
}
