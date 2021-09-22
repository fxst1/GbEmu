#include "graphics/Graphx.hh"

graphx::DriverAdapter::DriverAdapter(void):
	_title("untitled"),
	_w(256),
	_h(256),
	_viewport({0, 0, (int)_w, (int)_h})
{}
graphx::DriverAdapter::~DriverAdapter(void) {}

/*******************************/

void	graphx::DriverAdapter::setScreenTitle(std::string const &title) {
	this->_title = title;
}
void	graphx::DriverAdapter::resize(std::size_t w, std::size_t h) {
	this->_w = w;
	this->_h = h;
}

/*******************************/

void				graphx::DriverAdapter::setViewport(graphx::Viewport const &viewport) {
	this->_viewport = viewport;
}
graphx::Viewport	graphx::DriverAdapter::getViewport(void) const {
	return (this->_viewport);
}

/*******************************/

int					graphx::DriverAdapter::scaleX(int x) const {
	float			scale = (this->_viewport.w - this->_viewport.x) / this->_w;
	return (this->_viewport.x + (x * scale));
}
int					graphx::DriverAdapter::scaleY(int y) const {
	float			scale = (this->_viewport.h - this->_viewport.y) / this->_h;
	return (this->_viewport.y + (y * scale));
}
