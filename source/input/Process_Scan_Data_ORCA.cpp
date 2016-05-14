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

				char * cstr, *p;
				string str = line;
				vector< double > temp;
				vector< string > temp_str;
				/*
				cstr = new char [str.size()+1];
				strcpy (cstr, str.c_str());

				p=strtok (cstr," 	");
				while (p!=NULL)
				{
					temp.push_back(strtod(p,NULL));
					p=strtok(NULL," 	");
				}
				delete[] cstr;
				delete[] p;
//*/
				temp_str = Tokenise_String(str," 	");

				/*
				// Now sort the vector temp...
				AtomsInMolecule.CenterID = (int) temp[0];
				AtomsInMolecule.AtomNumber = 0;//(int) temp[1];
				AtomsInMolecule.AtomType = 0;//(int) temp[2];
				AtomsInMolecule.X = temp[5];
				AtomsInMolecule.Y = temp[6];
				AtomsInMolecule.Z = temp[7];
//*/
				// Now sort the vector temp...
				AtomsInMolecule.CenterID = atoi(temp_str[0].c_str());
				AtomsInMolecule.AtomNumber = 0;//(int) temp[1];
				AtomsInMolecule.AtomType = 0;//(int) temp[2];
				AtomsInMolecule.X = strtod(temp_str[5].c_str(),NULL);
				AtomsInMolecule.Y = strtod(temp_str[6].c_str(),NULL);
				AtomsInMolecule.Z = strtod(temp_str[7].c_str(),NULL);

				SingleMolecule.push_back(AtomsInMolecule);

				getline(DataInputFile,line); // this is the following line
				//}while(line.find("---")==string::npos); // until lower table line
			}while(!line.empty()); // ORCA terminates cartesians with a blank line

		}


		// Energy in ORCA
		/*
		 * ----------------
		 * TOTAL SCF ENERGY
		 * ----------------
		 * Total Energy       :        -5722.49861017 Eh         -155717.10369 eV
		 */
		found = line.find("Total Energy       :");
		if (found!=string::npos) // We have found energy
		{
			// This one gives us the energy
			/*
			 * SCF Done:  E(RB3LYP) =  -237.072003564     A.U. after    9 cycles
			 * 				Convg  =    0.4441D-08             -V/T =  2.0062
			 * Calling FoFJK, ICntrl=      2127 FMM=F ISym2X=1 I1Cent= 0 IOpClX= 0 NMat=1 NMatS=1 NMatT=0.
			 *
			 */

			char * cstr, *p;
			string str = line;
			vector< double > temp;

			cstr = new char [str.size()+1];
			strcpy (cstr, str.c_str());

			p=strtok (cstr," 	");
			while (p!=NULL)
			{
				temp.push_back(strtod(p,NULL));
				p=strtok(NULL," 	");
			}
			delete[] cstr;
			delete[] p;

			SingleOptPoint.Energy = temp[3];
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
