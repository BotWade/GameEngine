#include "OpenCL.hpp"
#include "../Core/Debug.hpp"
#include "../Core/FileManager.hpp"

bool OpenCL::IsAvailable;
int OpenCL::SelectedDevice;

vector<cl::Device> OpenCL::Devices;

cl::Context OpenCL::Context;
cl::CommandQueue OpenCL::Queue;

void OpenCL::LoadDevices() {
    
    vector<cl::Platform> Platforms;

    cl::Platform::get(&Platforms);

    if (Platforms.size() == 0) {
        Debug::Alert("Faild To Find OpenCL Compatible Plataforms");
        IsAvailable = false;
        return;
    }

    cl::Platform Platform = Platforms.front();

    Platform.getDevices(CL_DEVICE_TYPE_ALL, &Devices);

    if (Devices.size() == 0) {
        Debug::Alert("Faild To Find OpenCL Compatible Plataforms");
        IsAvailable = false;
        return;
    }

    SelectedDevice = 0;

    for (long unsigned int Index = 0; Index < Devices.size(); Index++) {
        
        Debug::Alert("OpenCL Device Found: ");
        Debug::Alert("  Vendor Info " + Devices[Index].getInfo<CL_DEVICE_VENDOR>());
        Debug::Alert("  Version Info " + Devices[Index].getInfo<CL_DEVICE_VERSION>());
    }

    Context = cl::Context(Devices[SelectedDevice]);
}

CLKernel OpenCL::LoadOpenCLProgram(const char* CLLocation) {

    ifstream File;
    
    if (!FileManager::GetFile(CLLocation, File)) {
        Debug::Alert("OpenCL File - " + (string)CLLocation + " Not Found");
        
    }

    string src(istreambuf_iterator<char>(File), (std::istreambuf_iterator<char>()));
    cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

    
    cl::Program program(Context, sources);

    cl_int err = program.build("-cl-std=CL1.1");

}
