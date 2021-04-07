#include "pch.h"
#include "MotionPAS.h"
#include "MotionPAS.h"

using namespace std;
using namespace chrono;
MotionPAS::MotionPAS()
{
	for (int i = 0; i < MAX_AXIS; i++) {
		m_lScale[i] = 16;
		m_nVelocity[i] = 50000;
	}
	m_bMotion = FALSE;
}

int MotionPAS::InitMotion(CWnd* pParent, int n)
{
	m_pParent = pParent;
	writeLog(("durTime : %dms"), 20);
	FAS_SetLogPath(_T("c:\\glim"));
	FAS_EnableLog(true);	
	FAS_SetLogLevel(LOG_LEVEL_MOTION);
	for (int i = 0; i < n; i++)
	{
		if (!FAS_IsSlaveExist(i))
		{
			if (!FAS_Connect(192, 168, 0, i+2, i))
				AfxMessageBox(_T("Failed to connect!"));
		}
		else
		{
			FAS_Close(i);
		}
		m_bMotion = FAS_IsSlaveExist(i);

		if (m_bMotion)
		{
			FAS_PrintCustomLog(i, LOG_LEVEL_ALL, _T("[ALL Log]    It's Connected.\n"));	// will not print.
			FAS_PrintCustomLog(i, LOG_LEVEL_MOTION, _T("[Motion Log] It's Connected.\n"));	// will print.
		}

		int nRet = FAS_ServoEnable(i, true);
	}
	return 0;
}

int MotionPAS::moveS(int nAxis, int nPos, int vel)
{
	if (!isMotion())	return 0;
	long lPos = nPos * m_lScale[nAxis];
	if (vel == 0)	vel = m_nVelocity[nAxis];
	return FAS_MoveSingleAxisAbsPos(nAxis, lPos, vel);
}

int MotionPAS::moveR(int nAxis, int nPos)
{
	if (!isMotion())	return 0;
	long lPos = nPos * m_lScale[nAxis];
	return FAS_MoveSingleAxisIncPos(nAxis, lPos, m_nVelocity[nAxis]);
}

int MotionPAS::moveSxy(int nX, int nY)
{
	if (!isMotion())	return 0;
	int		axis[] = { 0, 1 };
	long	posX = nX * m_lScale[0];
	long	posY = nY * m_lScale[1];
	long	lPos[] = { posX, posY };
	return FAS_MoveLinearAbsPos(2, axis, lPos, m_nVelocity[0], 100);
}

int MotionPAS::moveSxy(int targetX, int targetY, int nDurTime)
{
	if (!isMotion())	return 0;
	long curX, curY, distX, distY, velX, velY;
	int timeScale = 1115;
	curX = getPosition(AXIS::x);
	curY = getPosition(AXIS::y);
	distX = abs(targetX - curX);
	distY = abs(targetY - curY);
	if (distX > distY) {
		velX = (m_lScale[AXIS::x] * timeScale * distX / nDurTime);
		velY = (m_lScale[AXIS::y] * timeScale * distY / nDurTime);
	}
	else {
		velX = (m_lScale[AXIS::x] * timeScale * distX / nDurTime);
		velY = (m_lScale[AXIS::y] * timeScale * distY / nDurTime);
	}
	TRACE(_T("\nvelocity : %d / %d"),  velX, velY);

	system_clock::time_point StartTime = system_clock::now();
	moveS(AXIS::x, targetX, velX);
	moveS(AXIS::y, targetY, velY);
	waitAll();
	system_clock::time_point EndTime = system_clock::now();
	milliseconds ms = duration_cast<milliseconds>(EndTime - StartTime);
	writeLog(("durTime : %dms"), ms.count());
	
	return 1;
}

int MotionPAS::moveRxy(int nX, int nY)
{
	if (!isMotion())	return 0;
	int		axis[] = { 0, 1 };
	long	posX = nX * m_lScale[0];
	long	posY = nY * m_lScale[1];
	long	lPos[] = { posX, posY };
	return FAS_MoveLinearIncPos(2, axis, lPos, m_nVelocity[0], 100);
}

int MotionPAS::moveSDone(int nAxis, int nPos)
{
	moveS(nAxis, nPos);
	return wait(nAxis);
}

int MotionPAS::moveRDone(int nAxis, int nPos)
{
	moveR(nAxis, nPos);
	return wait(nAxis);
}

int MotionPAS::moveSxyDone(int nX, int nY)
{
	moveSxy(nX, nY);
	wait(0);
	return wait(1);
}

int MotionPAS::moveRxyDone(int nX, int nY)
{
	moveRxy(nX, nY);
	wait(0);
	return  wait(1);
}

int MotionPAS::moveV(int nAxis, int nDir)
{
	return FAS_MoveVelocity(nAxis, m_nVelocity[nAxis], nDir);
}

int MotionPAS::moveStop(int nAxis)
{
	return FAS_MoveStop(nAxis);
}

BOOL MotionPAS::PeekAndPump()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage()) {
			::PostQuitMessage(0);
			return FALSE;
		}
	}
	return TRUE;
}

int MotionPAS::wait(int nAxis)
{
	if (!isMotion())	return 0;
	int nRet;
	DWORD dwAxisStatus;
	EZISERVO_AXISSTATUS stAxisStatus;
	do
	{
		//Sleep(1);
		nRet = FAS_GetAxisStatus(nAxis, &dwAxisStatus);
		_ASSERT(nRet == FMM_OK);
		stAxisStatus.dwValue = dwAxisStatus;
		if (!PeekAndPump())
			break;
	} while (stAxisStatus.FFLAG_MOTIONING);

	return nRet;
}

int MotionPAS::waitAll()
{
	if (!isMotion())	return 0;
	wait(AXIS::x);
	return wait(AXIS::y);
}


long MotionPAS::getPosition(int nAxis)
{
	if (!isMotion())	return 0;
	long pos;
	FAS_GetCommandPos(nAxis, &pos);
	pos = long(pos / m_lScale[nAxis]);
	return pos;
}

int MotionPAS::moveHome(int nAxis)
{
	FAS_MoveOriginSingleAxis(nAxis);

	int nRet;
	DWORD dwAxisStatus;
	EZISERVO_AXISSTATUS stAxisStatus;
	do
	{
		Sleep(100);
		nRet = FAS_GetAxisStatus(nAxis, &dwAxisStatus);
		_ASSERT(nRet == FMM_OK);
		stAxisStatus.dwValue = dwAxisStatus;
		if (!PeekAndPump())
			break;
		TRACE(_T("%08X"),stAxisStatus.dwValue);
	} while (!stAxisStatus.FFLAG_ORIGINRETOK);
	return nRet;
}

BOOL MotionPAS::isMotion()
{
	return m_bMotion;
}
///*


template<typename ... Args>
void MotionPAS::writeLog(const string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size <= 0) { throw std::runtime_error("Error during formatting."); }
	unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	string str = string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
	writeLog(str);
}
//*/