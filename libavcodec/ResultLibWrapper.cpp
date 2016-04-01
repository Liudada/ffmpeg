#include "ResultLib.hpp"
#include "ResultLibWrapper.h"

extern "C" {
	ResultLib* ResultLib_constructor() {
		return new ResultLib();
	}
	void ReultLib_destructor(ResultLib* p) {
		delete p;
	}
	uint32_t ResultLib_read(ResultLib* p, const char *filename) {
		return p->read(filename);
	}
	void ResultLib_destroy(ResultLib* p) {
		p->destroy();
	}
	int8_t* ResultLib_getQpMap(ResultLib* p, int frameIdx, int qp1, int qp2) {
		int8_t* x;
		x = p->getQpMap(frameIdx, qp1,qp2);
		return x;
	}
}
