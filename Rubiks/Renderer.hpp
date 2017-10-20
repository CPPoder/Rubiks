#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Cube.hpp"



class Renderer
{
public:
	Renderer() = delete;
	Renderer(Renderer const &) = delete;
	~Renderer() = delete;
	Renderer& operator=(Renderer const &) = delete;

public:
	static void showCube(Cube const & cube);


private:
	static void Init();
	static void DeInit();


};





#endif //RENDERER_HPP

