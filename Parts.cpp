#include <cadex/Base_ProgressScope.hxx>
#include <cadex/Base_ProgressStatus.hxx>
#include <cadex/LicenseManager_Activate.h>
#include <cadex/MeshAlgo_MefistoFactory.hxx>
#include <cadex/MeshAlgo_NetgenFactory.hxx>
#include <cadex/ModelData_Body.hxx>
#include <cadex/ModelData_BodyList.hxx>
#include <cadex/ModelAlgo_BRepMesher.hxx>
#include <cadex/ModelAlgo_BRepMesherParameters.hxx>
#include <cadex/ModelData_BRepRepresentation.hxx>
#include <cadex/ModelData_IndexedTriangleSet.hxx>
#include <cadex/ModelData_Model.hxx>
#include <cadex/ModelData_Part.hxx>
#include <cadex/ModelData_PolyRepresentation.hxx>
#include <cadex/ModelData_PolyShapeList.hxx>
#include <cadex/ModelData_SceneGraphElementUniqueVisitor.hxx>
#include <cadex/ModelData_Transformation.hxx>
#include <cadex/ModelData_Vector.hxx>
#include <cadex/STEP_Reader.hxx>
#include <iostream>
#include "cadex_license_1.cxx"
#include<cadex/STL_Writer.hxx>
using namespace std;
using namespace cadex;
static bool Import(const char* theSource, ModelData_Model& theModel)
{
	STEP_Reader aReader;
	return aReader.ReadFile(theSource) && aReader.Transfer(theModel);
}

int i = 0;
void rekursPart(ModelData_Model::ElementIterator anIterator) {
	
	while (anIterator.HasNext()) {
		const ModelData_SceneGraphElement& aSGE = anIterator.Next();
		if (aSGE.TypeId() == ModelData_Part::GetTypeId()) {
			const ModelData_Part& anPart = static_cast<const ModelData_Part&> (aSGE);
			ModelData_BRepRepresentation aBrep = anPart.BRepRepresentation();
			ModelAlgo_BRepMesher aMesher;
			ModelData_PolyRepresentation aPoly = aMesher.Compute(aBrep);
			ModelData_Part aPart(aPoly);
			ModelData_Model aModel1;
			aModel1.AddRoot(aPart);
			STL_Writer aWriter;
			aWriter.Transfer(aModel1);
			i++;
			char *s;
			char s1[] = "abcdefghijklmnopqrstuvwxyz.stl";
			s = &s1[i];
			aWriter.WriteFile(s);
		}
		else
		{
			ModelData_Model::ElementIterator anInterator2(aSGE);
			rekursPart(anInterator2);
		}
	}
}


int main(int argc, char *argv[])
{
	if (argc < 2 || argc > 3) {
		cerr << "Usage: " << argv[0] << " <input_file> [<mesher>], where:" << endl;
		cerr << "    <input_file> is a name of the STEP file to be read" << endl;
		cerr << "    <mesher> is 'm' for Mefisto, 'n' for Netgen computational" << endl;
		cerr << "        mesher. Default is n" << endl;
		return 1;
	}
	//activate the license (aKey must be defined in cadex_license.cxx)
	if (!CADExLicense_Activate(aKey)) {
		cerr << "Failed to activate CAD Exchanger license." << endl;
		return 1;
	}

	//model import
	ModelData_Model aModel;
	const char* aSource = argv[1];
	if (!Import(aSource, aModel)) {
		cerr << "Failed to read the file " << aSource << endl;
		return 1;
	}
	vector<ModelData_Part> Parts;
	ModelData_Model::ElementIterator anIterator(aModel);
	rekursPart(anIterator);

	return 0;
}
