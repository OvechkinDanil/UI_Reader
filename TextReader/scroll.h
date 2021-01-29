#include "model.h"
#include <math.h>

/** \brief
 * устанавливает позицию скролов
 * \param hwnd - структура HWND
 * \param wind -  указатель на стрктуру, где хранятся окна и отображаемый текст
 * \param workWindow - указатель на структуру рабочего окна
 */

void SetScroll(HWND hwnd, windows_t* wind, workWindow_t* workWind);

/** \brief
 * Обновляет вертикальный скрол
 * \param wind -  указатель на стрктуру, где хранятся окна и отображаемый текст
 * \param workWindow - указатель на структуру рабочего окна
 *
 */

void UpdateVScroll(windows_t* wind, workWindow_t* workWind);

/** \brief
 * Обновляет данные о скроле
 * \param hwnd - структура HWND
 * \param wind -  указатель на стрктуру, где хранятся окна и отображаемый текст
 * \param workWindow - указатель на структуру рабочего окна
 */

void UpdateScroll(HWND hwnd, windows_t* wind, workWindow_t* workWind);
