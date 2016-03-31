#ifndef ResultLibWrapper_h
#define ResultLibWrapper_h

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct ResultLib ResultLib;
	ResultLib* ResultLib_constructor();
	void ResultLib_destructor(ResultLib* p);
	uint32_t ResultLib_read(ResultLib* p, const char *filename);
	void ResultLib_destroy(ResultLib* p);
	int8_t* ResultLib_getQpMap(ResultLib* p, int frameIdx, int qp1, int qp2);

#ifdef __cplusplus
}
#endif

#endif
