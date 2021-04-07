#pragma once

#include "FAS_EziMOTIONPlusE.h"

#define MAX_AXIS	4

using namespace PE;
using namespace std;

class MotionPAS
{
public:
	MotionPAS();
	//~MotionPAS();
private:
	BOOL m_bMotion;
	CWnd* m_pParent;

public:

	DWORD	m_nVelocity[MAX_AXIS];
	long	m_lScale[MAX_AXIS];

	int InitMotion(CWnd* pParent, int n);	//n : axis count
	BOOL isMotion();

	int moveHome(int nAxis);

	int moveS(int nAxis, int nPos, int vel=0);		//pos um
	int moveR(int nAxis, int nPos);
	int moveSxy(int nX, int nY);
	int moveRxy(int nX, int nY);
	int moveSxy(int nX, int nY, int nDurTime);
	int moveV(int nAxis, int nDir, DWORD Velocity);
	int moveStop(int nAxis);

	int moveSDone(int nAxis, int nPos);		//pos um
	int moveRDone(int nAxis, int nPos);
	int moveSxyDone(int nX, int nY);
	int moveRxyDone(int nX, int nY);

	int wait(int nAxis);
	int waitAll();

	long getAPosition(int nAxis);
	long getCPosition(int nAxis);
	BOOL PeekAndPump();
	template<typename ... Args>
	void writeLog(const string& format, Args ... args);
};



