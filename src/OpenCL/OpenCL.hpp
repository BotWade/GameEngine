#ifndef _OPENCL_
#define _OPENCL_

#include <CL/cl.hpp>
#include <vector>

using namespace std;

#define CLKernel cl::Kernel

class OpenCL {
public:

    static vector<cl::Device> Devices;

    static bool IsAvailable;
    static int SelectedDevice;

    static cl::Context Context;
    static cl::CommandQueue Queue;

    static void LoadDevices();
    static CLKernel LoadOpenCLProgram(const char* CLLocation);
};


#endif