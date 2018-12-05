
#ifndef CLWRAPPER_HPP
# define CLWRAPPER_HPP

# if defined(__APPLE__)
#  include <OpenCL/opencl.h>
#  include <OpenGL/OpenGL.h>
#  include <OpenGL/OpenGL.h>
# elif defined(_WIN32)
#  include <CL/cl.h>
#  include <CL/cl_gl.h>
# else
#  include <CL/cl.h>
#  include <CL/cl_gl.h>
#  include <GL/glx.h>
# endif

# include <iostream>
# include "Utils.hpp"
#include "Constant.hpp"

class CLWrapper
{
public:
	CLWrapper();
	~CLWrapper();

	cl_int					init();
	cl_int					createCommandQueue(cl_command_queue *cq);

    cl_context              getContext();
	cl_program 				getProgram(int const &program_id);
	cl_kernel 				getKernel(int const &kernel_id);
	cl_command_queue        getCommandQueue();
	cl_device_id            getDeviceID();

private:
	size_t					_local[KERNELS_NUMBER];
	cl_uint					_num_entries;
	cl_platform_id			_platform_id;
	cl_uint					_num_platforms;
	cl_device_id			_device_id;
	cl_context				_context;
	cl_command_queue		_cl_command_queue;
	cl_program				_programs[KERNELS_NUMBER];
	cl_kernel				_kernels[KERNELS_NUMBER];
};

#endif
