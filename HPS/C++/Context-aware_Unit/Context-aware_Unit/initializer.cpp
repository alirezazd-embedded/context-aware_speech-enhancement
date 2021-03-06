#include "initializer.h"

initializer::initializer()
	:rbf_storage_path(RBF_STORAGE_PATH), classifier_model_path(TFLITE_MODEL_PATH) {
	Initialize();
}

void initializer::Initialize() {
	Printm('c'); Printm('i', "Initialization started...");
	Check_rbf_storage();
	Check_classifier_model();
	mgr = new FPGA_mgr;
	mgr->Check_MSEL();
	IO = new IO_mgr;
	Verify_rbfs();
	Printm('i', "Initialization phase passed.");
	is_initialized = true;
}

void initializer::Check_rbf_storage() {
	Printm('i', "Checking rbf storage:\t");
	Printm('r', rbf_storage_path);
	rbf_list = dir(rbf_storage_path.c_str(), ".rbf");
	if (rbf_list.size() == 0) {
		Printm('e', "No bitsreams available in the bitsream storage.");
	}
	Print_vect(rbf_list, Printm('i', "List of available bitsreams:", 0, false));
}

void initializer::Check_rbf_compatibility() {
	Printm('i', "Checking " + mgr->Get_current_rbf() + " compatibility...");
	IO->Map_FPGA_IO();
	std::vector<int16_t> tmp;
	IO->Read_vect_frame();
	for (int16_t i = 1; i < (int16_t)(tmp.size()); i++) {
		if (tmp[i] * (2 ^ FPGA_INPUT_IF_FRACTIONAL_BITS) != i) {
			Printm('e', mgr->Get_current_rbf() + "compability check failed:\t" + std::to_string(i));
		}
	}
	IO->Umap_FPGA_IO();
	Printm('i', mgr->Get_current_rbf() + " compatibility check passed!");
}

void initializer::Check_classifier_model() {
	Printm('i', "Checking classifier model directory:\t");
	Printm('r', classifier_model_path);
	std::vector<std::string> classifer_model_list = dir(classifier_model_path.c_str(), ".tflite");
	if (classifer_model_list.size() == 0) {
		Printm('e', "No classifier model available in the model directory.");
	}
	if (classifer_model_list.size() > 1) {
		Printm('w', "Multiple Classifier models available in the model directory!");
		Print_vect(classifer_model_list, Printm('i', "List of available Tensorflow lite models:", 0, false));
	}
	Printm('i', "Using following classifier model:\t" + classifer_model_list[0]);
	classifer_model_name = classifer_model_list[0];
	classifier = new Classifier(classifier_model_path + classifer_model_name);
	DU = new decision_unit(classifier);
}

void initializer::Verify_rbfs() {
	Printm('i', "Verifying rbf storage...");
	for (std::vector<std::string>::iterator it = std::begin(rbf_list); it != std::end(rbf_list); ++it) {
		mgr->Reconfig(*it);
		Check_rbf_compatibility();
	}
	mgr->Reset_reconfig_count();
	Printm('i', "All rbfs are compatible and usable, verification passed.");
}

bool initializer::Is_initialized() {
	return is_initialized;
}

std::vector<std::string>& initializer::Get_rbf_list() {
	return rbf_list;
}

FPGA_mgr* initializer::Get_FPGA_mgr() {
	return mgr;
}

IO_mgr* initializer::Get_IO_mgr() {
	return IO;
}

Classifier* initializer::Get_Classifier() {
	return classifier;
}

decision_unit* initializer::Get_decision_unit()
{
	return DU;
}

initializer::~initializer()
{
	delete mgr;
	delete IO;
	delete classifier;
	delete DU;
}