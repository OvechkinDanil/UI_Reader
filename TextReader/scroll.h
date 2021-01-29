#include "model.h"
#include <math.h>

/** \brief
 * ������������� ������� �������
 * \param hwnd - ��������� HWND
 * \param wind -  ��������� �� ��������, ��� �������� ���� � ������������ �����
 * \param workWindow - ��������� �� ��������� �������� ����
 */

void SetScroll(HWND hwnd, windows_t* wind, workWindow_t* workWind);

/** \brief
 * ��������� ������������ �����
 * \param wind -  ��������� �� ��������, ��� �������� ���� � ������������ �����
 * \param workWindow - ��������� �� ��������� �������� ����
 *
 */

void UpdateVScroll(windows_t* wind, workWindow_t* workWind);

/** \brief
 * ��������� ������ � ������
 * \param hwnd - ��������� HWND
 * \param wind -  ��������� �� ��������, ��� �������� ���� � ������������ �����
 * \param workWindow - ��������� �� ��������� �������� ����
 */

void UpdateScroll(HWND hwnd, windows_t* wind, workWindow_t* workWind);
