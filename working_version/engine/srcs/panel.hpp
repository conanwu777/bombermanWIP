#ifndef PANEL_HPP
#define PANEL_HPP
#include "RenderObject.hpp"

class Panel : public RenderObject{
	public:
		Panel(Shader& sha, Camera & c, float x, float y, float z,
			float sX, float sY, float sZ, unsigned int);
		
		virtual void	SetCurrentRender();

		int width, height, nrChannels;
		unsigned int texture;
};

#endif
