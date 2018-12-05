
#include "../inc/CLWrapper.hpp"

CLWrapper::CLWrapper(void)
{
	return ;
}

CLWrapper::~CLWrapper(void)
{
    for (int i = 0; i < KERNELS_NUMBER; ++i)
    {
        clReleaseProgram(_programs[i]);
        clReleaseKernel(_kernels[i]);
    }
    clReleaseCommandQueue(_cl_command_queue);
    clReleaseContext(_context);
	return ;
}

cl_int
CLWrapper::createCommandQueue(cl_command_queue *cq)
{
	int						err;

	if (!cq)
		return (printError("Error: Failed to create command queue!", EXIT_FAILURE));
	*cq = clCreateCommandQueue(_context, _device_id, 0, &err);
	if (!(*cq) || err != CL_SUCCESS)
		return (printError("Error: Failed to create command queue!", EXIT_FAILURE));
	return (CL_SUCCESS);
}

cl_int
CLWrapper::init(void)
{
	int							err;
	int							i;
	size_t						len;
	char						build_log_buffer[4096];
	static char const			*options = "-Werror -cl-fast-relaxed-math";
	std::string					file_content;
	char						*file_string;

	_num_entries = 1;
	err = clGetPlatformIDs(_num_entries, &_platform_id, &_num_platforms);
	if (err != CL_SUCCESS)
		return (printError(std::ostringstream().flush() << "Error: Failed to retrieve platform id ! " << err, EXIT_FAILURE));

	err = clGetDeviceIDs(_platform_id, CL_DEVICE_TYPE_GPU, 1, &_device_id, 0);
	if (err != CL_SUCCESS)
		return (printError(std::ostringstream().flush() << "Error: Failed to create a device group ! " << err, EXIT_FAILURE));

	_context = clCreateContext(0, 1, &_device_id, 0, 0, &err);
	if (!_context || err != CL_SUCCESS)
		return (printError("Error: Failed to create a compute context !", EXIT_FAILURE));

	for (i = 0; i < KERNELS_NUMBER; ++i)
	{
		file_content = getFileContents(CLKernelFiles[i]);
		file_string = (char *)file_content.c_str();
		_programs[i] = clCreateProgramWithSource(_context, 1, (char const **)&file_string, 0, &err);
		if (!_programs[i] || err != CL_SUCCESS)
		{
			return (printError(std::ostringstream().flush()
								<< "Error Failed to create compute "
								<< CLProgramNames[i]
								<< " program !",
								EXIT_FAILURE));
		}
		err = clBuildProgram(_programs[i], 0, 0, options, nullptr, nullptr);
		if (err != CL_SUCCESS)
		{
			std::cerr << "Error: Failed to build program executable ! " << err << std::endl;
            for (int j = 0; j < 4096; ++j)
                build_log_buffer[j] = 0;
			clGetProgramBuildInfo(_programs[i], _device_id, CL_PROGRAM_BUILD_LOG, sizeof(build_log_buffer), build_log_buffer, &len);
			std::cerr << build_log_buffer << std::endl;
			return (EXIT_FAILURE);
		}
		_kernels[i] = clCreateKernel(_programs[i], CLProgramNames[i], &err);
		if (!_kernels[i] || err != CL_SUCCESS)
			return (printError("Error: Failed to create compute kernel !", EXIT_FAILURE));
		err = clGetKernelWorkGroupInfo(_kernels[i], _device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &_local[i], NULL);
		if (err != CL_SUCCESS)
			return (printError(std::ostringstream().flush() << "Error: Failed to retrieve kernel work group info! " << err, EXIT_FAILURE));
	}

    err = createCommandQueue(&_cl_command_queue);

	return (err);
}

cl_context
CLWrapper::getContext()
{
	return _context;
}

cl_command_queue
CLWrapper::getCommandQueue()
{
	return _cl_command_queue;
}

cl_program CLWrapper::getProgram(int const &program_id)
{
    return _programs[program_id];
}

cl_kernel CLWrapper::getKernel(int const &kernel_id)
{
    return _kernels[kernel_id];
}

cl_device_id CLWrapper::getDeviceID()
{
    return _device_id;
}
