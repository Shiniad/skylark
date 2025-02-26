/*******************************************************************************
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

#ifndef _EU_CHANGLES_H_
#define _EU_CHANGLES_H_

#define FILE_CHANGE_FILE_YES 1
#define FILE_CHANGE_FILE_NO  2
#define FILE_CHANGE_ALL_YES  3
#define FILE_CHANGE_ALL_NO   4
#define FILE_CHANGE_SEC_YES  5
#define FILE_CHANGE_SEC_NO   6

#ifdef __cplusplus
extern "C" {
#endif

void __stdcall on_changes_window(HWND hwnd);

#ifdef __cplusplus
}
#endif

#endif  // _EU_CHANGLES_H_
