/*
 * Process_Scan_Data.cpp
 *
 *  Created on: 19.12.2015
 *      Author: DetlevCM
 */


#include<Headers.h>


void Process_Scan_Data_Gaussian(vector< OptPoints >& GaussianData, string filename)
{
	string line;
	OptPoints SingleOptPoint;
	vector< AtomDefinition > SingleMolecule;
	ifstream DataInputFile;
	DataInputFile.open(filename.c_str());

	while(DataInputFile.good())
	{

		getline(DataInputFile,line);

		// after standard orientation we get energy info
		if (line.find("Standard orientation:")!=string::npos) // We have found a Z-Matrix
		{

			/*                           Input orientation:
			 * ---------------------------------------------------------------------
			 * Center     Atomic      Atomic             Coordinates (Angstroms)
			 * Number     Number       Type             X           Y           Z
			 * ---------------------------------------------------------------------
			 *       1          6           0       -1.428283    1.396103    0.000000
			 *       2          1           0       -1.962906    1.016650    0.878242
			 * ...
			 *      19          1           0        0.540584    1.221124   -0.889066
			 *      20          1           0        0.540584    1.221124    0.889066
			 *  ---------------------------------------------------------------------
			 */

			getline(DataInputFile,line);
			getline(DataInputFile,line);
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
				AtomsInMolecule.AtomNumber = (int) temp[1];
				AtomsInMolecule.AtomType = (int) temp[2];
				AtomsInMolecule.X = temp[3];
				AtomsInMolecule.Y = temp[4];
				AtomsInMolecule.Z = temp[5];

				SingleMolecule.push_back(AtomsInMolecule);

				getline(DataInputFile,line); // this is the following line
			}while(line.find("---")==string::npos); // until lower table line
		}



		if (line.find("SCF Done: ")!=string::npos) // We have found energy
		{
			// This one gives us the energy
			/*
			 * SCF Done:  E(RB3LYP) =  -237.072003564     A.U. after    9 cycles
			 * 				Convg  =    0.4441D-08             -V/T =  2.0062
			 * Calling FoFJK, ICntrl=      2127 FMM=F ISym2X=1 I1Cent= 0 IOpClX= 0 NMat=1 NMatS=1 NMatT=0.
			 *
			 */
			vector< double > temp;
			temp = Tokenise_String_To_Double(line,"= 	");

			SingleOptPoint.Energy = temp[4];
			temp.clear();
		}



		// Needs to find after each optimisation
		/*
		 * -----------
		 * For a scan:
		 * -----------
		 * Step number   1 out of a maximum of  111 on scan point     1 out of    26
		 *
		 *------------------------
		 * For an IRC calculation:
		 * -----------------------
		 * # OF POINTS ALONG THE PATH =  64
		 * # OF STEPS =   1
		 *
		 * Calculating another point on the path.
		 * Point Number 65 in FORWARD path direction.
		 * Using LQA Reaction Path Following.
		 *
		 *----------------------------
		 * For a regular Optimisation:
		 * ---------------------------
		 * Berny optimization.
		 * Internal  Forces:  Max     0.000000152 RMS     0.000000028
		 * Search for a saddle point.
		 * Step number  17 out of a maximum of  155
		 *
		 *
		 */

		/*
		 * Could potentially add a check for final geometry:
		 *
		 * --------
		 * Success:
		 * --------
		 *
		 * Optimization completed.
		 * -- Stationary point found.
		 *                           ----------------------------
		 *                           !   Optimized Parameters   !
		 *                           ! (Angstroms and Degrees)  !
		 * --------------------------                            --------------------------
		 * ! Name  Definition              Value          Derivative Info.                !
		 * --------------------------------------------------------------------------------
		 * ! R1    R(1,2)                  1.3583         -DE/DX =    0.0                 !
		 * ..
		 *
		 *
		 * -------
		 * Failed:
		 * -------
		 *
		 * Optimization stopped.
		 * -- Number of steps exceeded,  NStep= 111
		 * -- Flag reset to prevent archiving.
		 * 	                       ----------------------------
		 * 	                       ! Non-Optimized Parameters !
		 * 	                       ! (Angstroms and Degrees)  !
		 * --------------------------                            --------------------------
		 * ! Name  Definition              Value          Derivative Info.                !
		 * --------------------------------------------------------------------------------
		 * ! R1    R(1,2)                  1.0945         -DE/DX =   -0.0002              !
		 * ...
		 */


		if(line.find("Step number")!=string::npos) // We have found Step Identification
		{
			vector< string > temp;
			temp = Tokenise_String_To_String(line," 	");

			// Now pick the points we need
			// 2, 8, 12, 14

			SingleOptPoint.ScanStep = (int) strtod(temp[2].c_str(),NULL);
			SingleOptPoint.OptStep = (int) strtod(temp[8].c_str(),NULL);
			SingleOptPoint.OptStepMax = (int) strtod(temp[12].c_str(),NULL);
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
