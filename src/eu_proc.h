/******************************************************************************
 * This file is part of Skylark project
 * Copyright ©2021 Hua andy <hua.andy@gmail.com>

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *******************************************************************************/

#ifndef _EU_WNDPROC_H_
#define _EU_WNDPROC_H_

#ifdef __cplusplus
extern "C" 
{
#endif

int eu_dpi_scale_font(void);
int eu_dpi_scale_xy(int adpi, int m);
void eu_reset_drag_line(void);
void eu_window_resize(HWND hwnd);
void eu_clear_undo_off(void);
void eu_window_layout_dpi(HWND hwnd, const RECT *pnew_rect, const uint32_t adpi);

#ifdef __cplusplus
}
#endif

#endif  // _EU_WNDPROC_H_
