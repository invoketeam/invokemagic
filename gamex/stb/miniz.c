#pragma warning (disable : 4786)
#pragma warning (disable : 4244) //int64 to uint
#pragma warning (disable : 4761) //integral size whatever

//note: this is the implementation, for files that use it define:
//#define MINIZ_HEADER_FILE_ONLY

//#define MINIZ_NO_STDIO
#define MINIZ_NO_TIME
#include "miniz.h"