// 06분반 4조

#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <time.h>

#define I_COLOR RGB(135, 206, 235)
#define O_COLOR RGB(255, 255, 0)
#define T_COLOR RGB(128, 0, 128)
#define J_COLOR RGB(0, 0, 255)
#define L_COLOR RGB(255, 165, 0)
#define S_COLOR RGB(144, 238, 144)
#define Z_COLOR RGB(255, 0, 0)

typedef enum _tetromino_type { N, I, O, T, J, L, S, Z } TetrominoType;

typedef enum _direction { UPWARD, DOWNWARD, LEFT, RIGHT } Direction;

typedef struct _location
{
	int x, y;
} Location;

typedef struct _field
{
	TetrominoType space[20][10];
	Location curLoc[4];
	TetrominoType curT;
	int rotation;
	int score;
	BOOL isT_SPIN;
	BOOL isO_Power;
} Field;

void switchLocation(Location* l1, Location* l2, BOOL b)
{
	if (b) {
		int tmp = l1->y - l2->y;
		l1->y = l2->y - (l1->x - l2->x);
		l1->x = l2->x + tmp;
	}
	else {
		int tmp = l1->y - l2->y;
		l1->y = l2->y + (l1->x - l2->x);
		l1->x = l2->x - tmp;
	}
}

void setInitLocation(Field* pfield)
{
	switch (pfield->curT) {
	case I:
		pfield->curLoc[0].x = 4;
		pfield->curLoc[0].y = 1;
		pfield->curLoc[1].x = 5;
		pfield->curLoc[1].y = 1;
		pfield->curLoc[2].x = 3;
		pfield->curLoc[2].y = 1;
		pfield->curLoc[3].x = 6;
		pfield->curLoc[3].y = 1;
		break;
	case O:
		pfield->curLoc[0].x = 4;
		pfield->curLoc[0].y = 0;
		pfield->curLoc[1].x = 5;
		pfield->curLoc[1].y = 0;
		pfield->curLoc[2].x = 4;
		pfield->curLoc[2].y = 1;
		pfield->curLoc[3].x = 5;
		pfield->curLoc[3].y = 1;
		break;
	case T:
		pfield->curLoc[0].x = 4;
		pfield->curLoc[0].y = 1;
		pfield->curLoc[1].x = 5;
		pfield->curLoc[1].y = 1;
		pfield->curLoc[2].x = 3;
		pfield->curLoc[2].y = 1;
		pfield->curLoc[3].x = 4;
		pfield->curLoc[3].y = 2;
		break;
	case J:
		pfield->curLoc[0].x = 5;
		pfield->curLoc[0].y = 1;
		pfield->curLoc[1].x = 5;
		pfield->curLoc[1].y = 2;
		pfield->curLoc[2].x = 5;
		pfield->curLoc[2].y = 0;
		pfield->curLoc[3].x = 4;
		pfield->curLoc[3].y = 2;
		break;
	case L:
		pfield->curLoc[0].x = 4;
		pfield->curLoc[0].y = 1;
		pfield->curLoc[1].x = 4;
		pfield->curLoc[1].y = 2;
		pfield->curLoc[2].x = 4;
		pfield->curLoc[2].y = 0;
		pfield->curLoc[3].x = 5;
		pfield->curLoc[3].y = 2;
		break;
	case S:
		pfield->curLoc[0].x = 4;
		pfield->curLoc[0].y = 1;
		pfield->curLoc[1].x = 4;
		pfield->curLoc[1].y = 2;
		pfield->curLoc[2].x = 5;
		pfield->curLoc[2].y = 1;
		pfield->curLoc[3].x = 3;
		pfield->curLoc[3].y = 2;
		break;
	case Z:
		pfield->curLoc[0].x = 4;
		pfield->curLoc[0].y = 1;
		pfield->curLoc[1].x = 4;
		pfield->curLoc[1].y = 2;
		pfield->curLoc[2].x = 3;
		pfield->curLoc[2].y = 1;
		pfield->curLoc[3].x = 5;
		pfield->curLoc[3].y = 2;
	}
}

void initField(Field* pfield)
{
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			pfield->space[i][j] = N;
		}
	}
	pfield->isT_SPIN = FALSE;
	pfield->isO_Power = FALSE;
	srand((int)time(NULL));
	pfield->rotation = 0;
	pfield->curT = rand() % 7 + 1;
	setInitLocation(pfield);
}

void copyLocationArray(Location* lArrayCopy, Location* lArrayOrignal)
{
	for (int i = 0; i < 4; i++) {
		lArrayCopy[i].x = lArrayOrignal[i].x;
		lArrayCopy[i].y = lArrayOrignal[i].y;
	}
}

int distance(Field* pfield, Location* l, Direction direction)
{
	int ret = 0;
	TetrominoType t = N;
	int x = l->x;
	int y = l->y;
	if (x < 0 || x > 9 || y < 0 || y > 19) return -1;
	if (pfield->space[y][x] != N) return -1;
	switch (direction) {
	case UPWARD:
		y--;
		while (y >= 0) {
			if (pfield->space[y][x] != N) break;
			y--;
			ret++;
		}
		return ret;
	case DOWNWARD:
		y++;
		while (y < 20) {
			if (pfield->space[y][x] != N) break;
			y++;
			ret++;
		}
		return ret;
	case LEFT:
		x--;
		while (x >= 0) {
			if (pfield->space[y][x] != N) break;
			x--;
			ret++;
		}
		return ret;
	case RIGHT:
		x++;
		while (x < 10) {
			if (pfield->space[y][x] != N) break;
			x++;
			ret++;
		}
		return ret;
	default:
		return ret;
	}
}

int goLeft(Field* pfield)
{
	for (int i = 0; i < 4; i++)
		if (distance(pfield, &(pfield->curLoc[i]), LEFT) < 1) return 0;
	for (int i = 0; i < 4; i++)
		(pfield->curLoc[i].x)--;
	return 1;
}

int goRight(Field* pfield)
{
	for (int i = 0; i < 4; i++)
		if (distance(pfield, &(pfield->curLoc[i]), RIGHT) < 1) return 0;
	for (int i = 0; i < 4; i++)
		(pfield->curLoc[i].x)++;
	return 1;
}

int goDown(Field* pfield)
{
	for (int i = 0; i < 4; i++) {
		if (distance(pfield, &(pfield->curLoc[i]), DOWNWARD) < 1) {
			if (distance(pfield, &(pfield->curLoc[i]), UPWARD) < 0)
				return -1;
			else return 0;
		}
	}
	for (int i = 0; i < 4; i++)
		(pfield->curLoc[i].y)++;
	return 1;
}

int goUp(Field* pfield)
{
	for (int i = 0; i < 4; i++)
		if (distance(pfield, &(pfield->curLoc[i]), UPWARD) < 1) return 0;
	for (int i = 0; i < 4; i++)
		(pfield->curLoc[i].y)--;
	return 1;
}

void rotateCounterClockwise(Field* pfield)
{
	Location tmp[4];
	copyLocationArray(tmp, pfield->curLoc);
	BOOL chk = FALSE;
	int memo = 0;
	switch (pfield->curT) {
	case I:
		if (pfield->rotation == 0) {
			pfield->rotation = 1;
			for (int i = 1; i < 4; i++) switchLocation(pfield->curLoc + i, pfield->curLoc, TRUE);
			for (int i = 1; i < 4; i++) {
				if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
					chk = TRUE;
					break;
				}
			}
			if (chk) {
				goRight(pfield);
				for (int i = 1; i < 4; i++) {
					if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
						copyLocationArray(pfield->curLoc, tmp);
						pfield->rotation = 0;
						break;
					}
				}
			}
		}
		else {
			pfield->rotation = 0;
			for (int i = 1; i < 4; i++) switchLocation(pfield->curLoc + i, pfield->curLoc, FALSE);
			goLeft(pfield);
			for (int i = 1; i < 4; i++) {
				if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
					chk = TRUE;
					break;
				}
			}
			if (chk) {
				goRight(pfield);
				for (int i = 1; i < 4; i++) {
					if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
						copyLocationArray(pfield->curLoc, tmp);
						pfield->rotation = 1;
						break;
					}
				}
			}
		}
		return;
	case O:
		return;
	case S:
	case Z:
		if (pfield->rotation == 0) {
			pfield->rotation = 1;
			for (int i = 1; i < 4; i++) switchLocation(pfield->curLoc + i, pfield->curLoc, FALSE);
			for (int i = 1; i < 4; i++) {
				if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
					chk = TRUE;
					break;
				}
			}
			if (chk) {
				goLeft(pfield);
				for (int i = 1; i < 4; i++) {
					if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
						copyLocationArray(pfield->curLoc, tmp);
						pfield->rotation = 0;
						break;
					}
				}
			}
		}
		else {
			pfield->rotation = 0;
			for (int i = 1; i < 4; i++) switchLocation(pfield->curLoc + i, pfield->curLoc, TRUE);
			for (int i = 1; i < 4; i++) {
				if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
					chk = TRUE;
					break;
				}
			}
			if (chk) {
				goRight(pfield);
				for (int i = 1; i < 4; i++) {
					if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
						copyLocationArray(pfield->curLoc, tmp);
						pfield->rotation = 1;
						break;
					}
				}
			}
		}
		return;
	default:
		for (int i = 1; i < 4; i++) switchLocation(pfield->curLoc + i, pfield->curLoc, TRUE);
		pfield->rotation++;
		for (int i = 1; i < 4; i++) {
			if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
				pfield->rotation--;
				copyLocationArray(pfield->curLoc, tmp);
				break;
			}
		}
		if (pfield->rotation > 4) pfield->rotation -= 4;
		return;
	}
}

void rotateClockwise(Field* pfield)
{
	Location tmp[4];
	copyLocationArray(tmp, pfield->curLoc);
	BOOL chk = FALSE;
	int memo = 0;
	switch (pfield->curT) {
	case I:
		if (pfield->rotation == 0) {
			pfield->rotation = 1;
			for (int i = 1; i < 4; i++) switchLocation(pfield->curLoc + i, pfield->curLoc, TRUE);
			goRight(pfield);
			for (int i = 1; i < 4; i++) {
				if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
					chk = TRUE;
					break;
				}
			}
			if (chk) {
				goLeft(pfield);
				for (int i = 1; i < 4; i++) {
					if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
						copyLocationArray(pfield->curLoc, tmp);
						pfield->rotation = 0;
						break;
					}
				}
			}
			return;
		}
		else {
			pfield->rotation = 0;
			for (int i = 1; i < 4; i++) switchLocation(pfield->curLoc + i, pfield->curLoc, FALSE);
			for (int i = 1; i < 4; i++) {
				if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
					chk = TRUE;
					break;
				}
			}
			if (chk) {
				goLeft(pfield);
				for (int i = 1; i < 4; i++) {
					if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
						copyLocationArray(pfield->curLoc, tmp);
						pfield->rotation = 1;
						break;
					}
				}
			}
			return;
		}
	case O:
		return;
	case S:
	case Z:
		if (pfield->rotation == 0) {
			pfield->rotation = 1;
			for (int i = 1; i < 4; i++) switchLocation(pfield->curLoc + i, pfield->curLoc, TRUE);
			for (int i = 1; i < 4; i++) {
				if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
					chk = TRUE;
					break;
				}
			}
			if (chk) {
				goRight(pfield);
				for (int i = 1; i < 4; i++) {
					if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
						copyLocationArray(pfield->curLoc, tmp);
						pfield->rotation = 0;
						break;
					}
				}
			}
		}
		else {
			pfield->rotation = 0;
			for (int i = 1; i < 4; i++) switchLocation(pfield->curLoc + i, pfield->curLoc, TRUE);
			for (int i = 1; i < 4; i++) {
				if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
					chk = TRUE;
					break;
				}
			}
			if (chk) {
				goRight(pfield);
				for (int i = 1; i < 4; i++) {
					if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
						copyLocationArray(pfield->curLoc, tmp);
						pfield->rotation = 1;
						break;
					}
				}
			}
		}
		return;
	default:
		for (int i = 1; i < 4; i++) switchLocation(pfield->curLoc + i, pfield->curLoc, FALSE);
		pfield->rotation--;
		for (int i = 1; i < 4; i++) {
			if (distance(pfield, pfield->curLoc + i, DOWNWARD) == -1) {
				pfield->rotation++;
				copyLocationArray(pfield->curLoc, tmp);
				break;
			}
		}
		if (pfield->rotation < 0) pfield->rotation += 4;
		return;
	}
}

int deleteFilledLine(Field* pfield)
{
	int cnt = 0;
	int temp = 0;
	for (int i = 0; i < 20; i++) {
		BOOL chk = TRUE;
		TetrominoType tType = pfield->space[i][0];
		int local_cnt = 0;
		for (int j = 0; j < 10; j++) {
			if (pfield->space[i][j] == N) {
				chk = FALSE;
				break;
			}
			if (tType != pfield->space[i][j]) {
				local_cnt++;
				tType = pfield->space[i][j];
			}
		}
		if (chk) {
			cnt += local_cnt + 2;
			for (int j = i; j > 0; j--) {
				for (int k = 0; k < 10; k++)
					pfield->space[j][k] = pfield->space[j - 1][k];
			}
			temp++;
		}
	}
	return cnt * temp;
}

int t_spin(Field* pfield)
{
	if (pfield->curT != T)
		return 0;
	int x = pfield->curLoc[0].x;
	int y = pfield->curLoc[0].y;
	int num = distance(pfield, pfield->curLoc, DOWNWARD);
	if (pfield->space[y + num][x - 1] == N || pfield->space[y + num][x + 1] == N || \
		pfield->space[y + num - 1][x - 1] != N || pfield->space[y + num][x + 1] == N)
		return 0;
	if (pfield->space[y + num - 2][x - 1] != N && pfield->space[y + num - 2][x + 1] != N) {
		pfield->isT_SPIN = TRUE;
		for (int i = 0; i < 4; i++)
			pfield->curLoc[i].y += num - 1;
		rotateCounterClockwise(pfield);
		return 2;
	}
	else if (pfield->space[y + num - 2][x - 1] != N || pfield->space[y + num - 2][x + 1] != N) {
		pfield->isT_SPIN = TRUE;
		for (int i = 0; i < 4; i++)
			pfield->curLoc[i].y += num - 1;
		rotateCounterClockwise(pfield);
		return 1;
	}
	else return 0;
}

int o_power_count(Field* pfield) {
	if (pfield->curT != O)
		return 0;
	int x = pfield->curLoc[0].x;
	int y = pfield->curLoc[0].y;
	int num = distance(pfield, pfield->curLoc, DOWNWARD);
	int cnt = 0;
	if (x - 1 < 0) {
		while (TRUE) {
			if (pfield->space[y + num - cnt][x + 2] == N || pfield->space[y + num - cnt][x] != N || pfield->space[y + num - cnt][x + 1] != N) break;
			cnt++;
		}
	}
	else if (x + 2 > 9) {
		while (TRUE) {
			if (pfield->space[y + num - cnt][x - 1] == N || pfield->space[y + num - cnt][x] != N || pfield->space[y + num - cnt][x + 1] != N) break;
			cnt++;
		}
	}
	else {
		while (TRUE) {
			if (pfield->space[y + num - cnt][x - 1] == N || pfield->space[y + num - cnt][x + 2] == N || pfield->space[y + num - cnt][x] != N || pfield->space[y + num - cnt][x + 1] != N) break;
			cnt++;
		}
	}
	return cnt;
}

BOOL newPieceStart(Field* pfield)
{
	for (int i = 0; i < 4; i++) if (distance(pfield, pfield->curLoc + i, UPWARD) < 0) return FALSE;
	for (int i = 0; i < 4; i++)
		pfield->space[pfield->curLoc[i].y][pfield->curLoc[i].x] = pfield->curT;
	pfield->curT = rand() % 7 + 1;
	pfield->rotation = 0;
	setInitLocation(pfield);
	return TRUE;
}

BOOL newPieceStartWhenTS(Field* pfield)
{
	if (!pfield->isT_SPIN) return FALSE;
	for (int i = pfield->curLoc[0].y - 1; i < pfield->curLoc[0].y + 2; i++) {
		for (int j = 0; j < 10; j++)
			pfield->space[i][j] = I;
	}
	deleteFilledLine(pfield);
	pfield->isT_SPIN = FALSE;
	pfield->curT = rand() % 7 + 1;
	pfield->rotation = 0;
	setInitLocation(pfield);
	return TRUE;
}

BOOL newPieceStartWhenOP(Field* pfield, int* pop)
{
	if (!pfield->isO_Power) return FALSE;
	int x = pfield->curLoc[0].x;
	int y = pfield->curLoc[0].y;
	for (int i = y; i < y + *pop; i++) {
		for (int j = 0; j < 10; j++) {
			pfield->space[i][j] = I;
		}
	}
	deleteFilledLine(pfield);
	*pop = 0;
	pfield->isO_Power = FALSE;
	setInitLocation(pfield);
	return TRUE;
}

HBRUSH getTetrominoHBrush(TetrominoType t)
{
	if (t == N) (HBRUSH)GetStockObject(WHITE_BRUSH);
	if (t == I) return CreateSolidBrush(I_COLOR);
	if (t == O) return CreateSolidBrush(O_COLOR);
	if (t == T) return CreateSolidBrush(T_COLOR);
	if (t == J) return CreateSolidBrush(J_COLOR);
	if (t == L) return CreateSolidBrush(L_COLOR);
	if (t == S) return CreateSolidBrush(S_COLOR);
	if (t == Z) return CreateSolidBrush(Z_COLOR);
	return (HBRUSH)GetStockObject(WHITE_BRUSH);
}

void showField(Field* pfield, HWND* phwnd, HDC* phdc, PAINTSTRUCT* pps, HBRUSH* phBrush, HBRUSH* poldBrush)
{
	TCHAR buffer[80];
	static RECT rect = {250, 200, 500, 500};
	*phdc = BeginPaint(*phwnd, pps);
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			if (pfield->space[i][j] == N) continue;
			else *phBrush = getTetrominoHBrush(pfield->space[i][j]);
			*poldBrush = (HBRUSH)SelectObject(*phdc, *phBrush);
			Rectangle(*phdc, 20 * j, 20 * i, 20 * j + 20, 20 * i + 20);
			SelectObject(*phdc, *poldBrush);
			DeleteObject(*phBrush);
		}
	}
	for (int i = 0; i < 4; i++) {
		BOOL chk = FALSE;
		*phBrush = getTetrominoHBrush(pfield->curT);
		if (chk) continue;
		*poldBrush = (HBRUSH)SelectObject(*phdc, *phBrush);
		Rectangle(*phdc, 20 * pfield->curLoc[i].x, 20 * pfield->curLoc[i].y, 20 * pfield->curLoc[i].x + 20, 20 * pfield->curLoc[i].y + 20);
		SelectObject(*phdc, *poldBrush);
		DeleteObject(*phBrush);
	}
	*phBrush = CreateSolidBrush(BLACK_BRUSH);
	*poldBrush = (HBRUSH)SelectObject(*phdc, *phBrush);
	Rectangle(*phdc, 0, 400, 200, 405);
	SelectObject(*phdc, *poldBrush);
	DeleteObject(*phBrush);
	_stprintf_s(buffer, _countof(buffer), _T("SCORE: %d"), pfield->score);
	TextOut(*phdc, 0, 420, buffer, _tcslen(buffer));
	DrawText(*phdc, _T("UP: spin\n\nT: stop\n\nG: 5min\n\nA: speed up\nW: speed down\nE: speed to default\n\nC: creative-mode"), \
		_tcslen(_T("UP: spin\n\nT: STOP\n\nG: 5min\n\nA: speed up\nW: speed down\nE: speed to default\n\nC: creative-mode")), &rect, DT_LEFT | DT_TOP | DT_NOCLIP);
	EndPaint(*phwnd, pps);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	static Field field;
	static int o_power = 0;
	static int speed = 700;
	static int opc = 0;
	static BOOL isCMode = FALSE;
	static BOOL isStop = FALSE;
	switch (iMsg) {
	case WM_CREATE:
		SetTimer(hwnd, 1, speed, NULL);
		initField(&field);
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (field.isT_SPIN) newPieceStartWhenTS(&field);
			else if (field.isO_Power) {
				newPieceStartWhenOP(&field, &o_power);
				opc = 0;
			}
			else if (goDown(&field) <= 0) {
				newPieceStart(&field);
				field.score += 20 * deleteFilledLine(&field);
			}
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case 2:
			if (isStop) break;
			isStop = TRUE;
			KillTimer(hwnd, 1);
			KillTimer(hwnd, 2);
			break;
		}
	case WM_PAINT:
		showField(&field, &hwnd, &hdc, &ps, &hBrush, &oldBrush);
		break;
	case WM_CHAR:
		if (isCMode) {
			switch (wParam) {
			case 's':
				rotateCounterClockwise(&field);
				InvalidateRgn(hwnd, NULL, TRUE);
				break;
			case 'z':
				rotateClockwise(&field);
				InvalidateRgn(hwnd, NULL, TRUE);
				break;
			case 'x':
				field.curT = rand() % 7 + 1;
				field.rotation = 0;
				setInitLocation(&field);
				InvalidateRgn(hwnd, NULL, TRUE);
				break;
			case 'c':
				isCMode = FALSE;
				SetTimer(hwnd, 1, speed, NULL);
				break;
			}
			break;
		}
		switch (wParam) {
		case 'a':
			if (speed > 100)
				speed -= 100;
			KillTimer(hwnd, 1);
			SetTimer(hwnd, 1, speed, NULL);
			break;
		case 'w':
			speed += 100;
			KillTimer(hwnd, 1);
			SetTimer(hwnd, 1, speed, NULL);
			break;
		case 'e':
			speed = 700;
			KillTimer(hwnd, 1);
			SetTimer(hwnd, 1, speed, NULL);
			break;
		case 'c':
			isCMode = TRUE;
			KillTimer(hwnd, 1);
			break;
		case 't':
			if (!isStop) {
				isStop = TRUE;
				KillTimer(hwnd, 1);
				break;
			}
			else {
				isStop = FALSE;
				SetTimer(hwnd, 1, speed, NULL);
				break;
			}
			break;
		case 'g':
			SetTimer(hwnd, 2, 300000, NULL);
			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			goLeft(&field);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case VK_RIGHT:
			goRight(&field);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case VK_UP:
			if (isCMode) {
				goUp(&field);
				InvalidateRgn(hwnd, NULL, TRUE);
				break;
			}
			rotateCounterClockwise(&field);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case VK_DOWN:
			if (isCMode) {
				goDown(&field);
				InvalidateRgn(hwnd, NULL, TRUE);
				break;
			}
			while (goDown(&field) > 0) {}
			SetTimer(hwnd, 1, speed, NULL);
			newPieceStart(&field);
			field.score += 20 * deleteFilledLine(&field);
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		case VK_SPACE:
			if (isCMode) {
				newPieceStart(&field);
				InvalidateRgn(hwnd, NULL, TRUE);
				break;
			}
			field.score += 2000 * t_spin(&field);
			if (o_power_count(&field) > 7 && field.isO_Power == FALSE) {
				field.isO_Power = TRUE;
				while (goDown(&field) > 0) {}
				o_power += 2;
				opc = o_power_count(&field) - 2;
				field.score += 1000;
			}
			else if (field.isO_Power == TRUE && opc > 0) {
				for (int i = 0; i < 4; i++)
					field.curLoc[i].y -= 2;
				o_power += 2;
				opc -= 2;
				field.score += 1000;
			}
			InvalidateRgn(hwnd, NULL, TRUE);
			break;
		}
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW; // 출력 형태
	WndClass.lpfnWndProc = WndProc; // 프로시저 함수
	WndClass.cbClsExtra = 0; // 클래스 여분 메모리
	WndClass.cbWndExtra = 0; // 윈도우 여분 메모리
	WndClass.hInstance = hInstance; // 윈도우 인스턴스
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 아이콘
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // 커서
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 배경 색
	WndClass.lpszMenuName = NULL; // 메뉴 이름
	WndClass.lpszClassName = _T("Window Class Name"); // 클래스 이름
	RegisterClass(&WndClass);
	hwnd = CreateWindow( // 윈도우 핸들 값 반환
		_T("WIndow Class Name"), // 윈도우 클래스 이름
		_T("Tetris"), // 윈도우 타이틀 이름
		WS_OVERLAPPEDWINDOW, // 윈도우 스타일
		CW_USEDEFAULT, // 윈도우 위치 x 좌표
		CW_USEDEFAULT, // 윈도우 위치 y 좌표
		400, // 윈도우 가로 크기
		600, // 윈도우 세로 크기
		NULL, // 부모 윈도우 핸들
		NULL, // 메뉴 핸들
		hInstance, // 응용프로그램 인스턴스
		NULL // 생성 윈도우 정보
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}