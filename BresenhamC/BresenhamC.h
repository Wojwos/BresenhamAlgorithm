// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the BRESENHAMC_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// BRESENHAMC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef BRESENHAMC_EXPORTS
#define BRESENHAMC_API __declspec(dllexport)
#else
#define BRESENHAMC_API __declspec(dllimport)
#endif

extern "C" __declspec(dllimport) void getPixelsPositionC(bool **pixels, int x1, int y1, int x2, int y2);

// This class is exported from the dll
class BRESENHAMC_API CBresenhamC {
public:
	CBresenhamC(void);
	void getPixelsPosition(bool pixels[400][400], int x1, int y1, int x2, int y2);
	int countPixels(int x1, int y1, int x2, int y2);

	// TODO: add your methods here.
};

extern BRESENHAMC_API int nBresenhamC;

BRESENHAMC_API int fnBresenhamC(void);
