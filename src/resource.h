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

#ifndef _EU_RESOURCE_H_
#define _EU_RESOURCE_H_

#ifndef IDC_STATIC
#define IDC_STATIC -1
#endif

#define ID_COPY                         2
#define ID_FINDPREV                     3
#define ID_FINDNEXT                     4
#define IDC_REPLACEALL                  5
#define ID_REPLACEALL                   5
#define IDD_SEARCH_FIND                 9
#define IDD_SEARCH_FIND_DIALOG          9
#define ID_COPYTO_CLIPBOARD             10
#define IDI_SKYLARK                     20
#define IDI_SMALL                       21
#define IDI_TXT_48                      22
#define IDI_TXT_32                      23
#define IDD_SKYLARK_DIALOG              102
#define IDS_APP_TITLE                   103
#define IDD_ABOUTBOX                    103
#define IDM_ABOUT                       104
#define IDD_SEARCH_REPLACE              104
#define IDD_SEARCH_REPLACE_DIALOG       104
#define IDM_EXIT                        105

#define IDC_SKYLARK                     109
#define IDB_NEWFILE_C                   110
#define IDB_NEWFILE_D                   111
#define IDB_OPENFILE_C                  112
#define IDB_OPENFILE_D                  113
#define IDB_SAVEFILE_C                  114
#define IDB_SAVEFILE_D                  115
#define IDB_SAVEFILEAS_C                116
#define IDB_SAVEFILEAS_D                117
#define IDB_CUTTEXT_C                   118
#define IDB_CUTTEXT_D                   119
#define IDB_COPYTEXT_C                  120
#define IDB_COPYTEXT_D                  121
#define IDB_PASTETEXT_C                 122
#define IDB_PASTETEXT_D                 123
#define IDR_MAINFRAME                   128
#define IDB_CLSDFOLD                    130
#define IDB_DOC                         131
#define IDB_DRIVE                       132
#define IDB_EXE                         133
#define IDB_OPENFOLD                    134
#define IDB_TXT                         135
#define IDR_SYMBOLLIST_POPUPMENU        136
#define IDR_FILETREE_FILE_POPUPMENU     137

#define IDR_TABPAGE_POPUPMENU           140
#define IDR_EDITOR_POPUPMENU            141
#define IDD_STYLETHEME_DIALOG           142
#define IDR_FILETREE_DIRECTORY_POPUPMENU 143
#define IDD_SEARCH_FOUND_DIALOG         144
#define IDB_BITMAP1                     146
#define IDB_FIND_C                      147
#define IDB_FIND_D                      148
#define IDB_FINDPREV_C                  149
#define IDB_FINDPREV_D                  150
#define IDB_FINDNEXT_C                  151
#define IDB_FINDNEXT_D                  152
#define IDB_STYLETHEME_C                153
#define IDB_STYLETHEME_D                154
#define IDB_ZOOMOUT_C                   155
#define IDB_ZOOMOUT_D                   156
#define IDB_ZOOMIN_C                    157
#define IDB_ZOOMIN_D                    158
#define IDB_ABOUT_C                     159
#define IDB_ABOUT_D                     160
#define IDB_SAVEALLFILES_C              161
#define IDB_SAVEALLFILES_D              162
#define IDB_CLOSEFILE_C                 163
#define IDB_CLOSEFILE_D                 164
#define IDB_REMOTEFILESERVERS_C         165
#define IDB_REMOTEFILESERVERS_D         166
#define IDB_UNDO_C                      167
#define IDB_UNDO_D                      168
#define IDB_REDO_C                      169
#define IDB_REDO_D                      170
#define IDB_FOUNDLIST_C                 171
#define IDB_FOUNDLIST_D                 172
#define IDB_REPLACE_C                   173
#define IDB_REPLACE_D                   174
#define IDB_TOGGLE_BOOKMARK_C           175
#define IDB_TOGGLE_BOOKMARK_D           176
#define IDB_GOTO_PREV_BOOKMARK_C        177
#define IDB_GOTO_PREV_BOOKMARK_D        178
#define IDB_GOTO_NEXT_BOOKMARK_C        179
#define IDB_GOTO_NEXT_BOOKMARK_D        180
#define IDB_NAVIGATEBACK_C              181
#define IDB_NAVIGATEBACK_D              182
#define IDB_FILETREE_C                  183
#define IDB_FILETREE_D                  184
#define IDB_HEXEDITMODE_C		        185
#define IDB_HEXEDITMODE_D		        186
#define IDC_SEARCH_TEXT                 1000
#define IDC_SEARCH_TEXT2                1001
#define IDC_TEXTTYPE_GENERAL            1002
#define IDC_IDC_TEXTTYPE_REGEXP         1003
#define IDC_TEXTTYPE_REGEXP             1003
#define IDC_OPTIONS_WHOLEWORD           1004
#define IDC_TEXTTYPE_POSIXREGEXP        1005
#define ID_REPLACE                      1006
#define IDC_AREATYPE_THISFILE           1007
#define IDC_AREATYPE_SELECTSECTION      1008
#define IDC_SEARCHLOCATE_TO_TOP_OR_BOTTOM 1008
#define IDC_SEARCH_FROMTEXT             1009
#define IDC_AREATYPE_ALLOPENEDFILES     1010
#define IDC_AREATYPE_ALLFILES           1011
#define IDC_SEARCHLOCATE_LOOP_TOP_OR_BOTTOM 1011
#define IDC_SEARCHLOCATE_LOOP           1011
#define IDC_TEXTTYPE_POSIX              1012
#define IDC_AREATYPE_ALLOPENEDFILES3    1012
#define IDC_AREATYPE_CURRENTDIRECTORY   1012
#define IDC_OPTIONS_MATCHCASE           1013
#define IDC_OPTIONS_WORDSTART           1014
#define IDC_SEARCH_REPLACETEXT          1015
#define IDC_SEARCH_TOTEXT               1016
#define ID_REPLACEPREV                  1017
#define ID_REPLACENEXT                  1018

#define IDC_SETFONT_KEYWORD_BUTTON      1032
#define IDC_SETFONT_KEYWORDS_BUTTON     1033
#define IDC_KEYWORDS_STATIC             1034
#define IDC_KEYWORDS2_STATIC            1035
#define IDC_STRING_STATIC               1036
#define IDC_CHARACTER_STATIC            1037
#define IDC_NUMBER_STATIC               1038
#define IDC_PREPROCESSOR_STATIC         1039
#define IDC_COMMENT_STATIC              1040
#define IDC_SETTEXTCOLOR_KEYWORD_BUTTON 1041
#define IDC_SETTEXTCOLOR_KEYWORDS_BUTTON 1042
#define IDC_SETFONT_KEYWORD2_BUTTON     1043
#define IDC_SETFONT_KEYWORDS2_BUTTON    1044
#define IDC_SETTEXTCOLOR_KEYWORD2_BUTTON 1045
#define IDC_SETTEXTCOLOR_KEYWORDS2_BUTTON 1046
#define IDC_SETFONT_STRING_BUTTON       1047
#define IDC_SETTEXTCOLOR_STRING_BUTTON  1048
#define IDC_SETFONT_CHARACTER_BUTTON    1049
#define IDC_SETTEXTCOLOR_CHARACTER_BUTTON 1050
#define IDC_COMMENTLINE_STATIC          1051
#define IDC_COMMENTDOC_STATIC           1052
#define IDC_SETFONT_NUMBER_BUTTON       1053
#define IDC_SETTEXTCOLOR_NUMBER_BUTTON  1054
#define IDC_SETFONT_PREPROCESSOR_BUTTON 1055
#define IDC_SETTEXTCOLOR_PREPROCESSOR_BUTTON 1056
#define IDC_KEYWORDS_STATIC2            1057
#define IDC_TEXT_STATIC                 1058
#define IDC_SETFONT_TEXT_BUTTON         1059
#define IDC_SETFONT_COMMENT_BUTTON      1060
#define IDC_SETTEXTCOLOR_COMMENT_BUTTON 1061
#define IDC_SETFONT_COMMENTLINE_BUTTON  1062
#define IDC_SETTEXTCOLOR_COMMENTLINE_BUTTON 1063
#define IDC_SETTEXTCOLOR_TEXT_BUTTON    1064
#define IDC_SETDEFAULT_BUTTON           1065
#define IDC_SETFONT_COMMENTDOC_BUTTON   1066
#define IDC_BUTTON19                    1067
#define IDC_SETTEXTCOLOR_COMMENTDOC_BUTTON 1067
#define IDC_OPERATOR_STATIC            1068
#define IDC_SETFONT_OPERATOR_BUTTON    1069
#define IDC_SETTEXTCOLOR_OPERATOR_BUTTON 1070
#define IDC_CARETLINE_STATIC            1071
#define IDC_SETBGCOLOR_TEXT_BUTTON2     1072
#define IDC_SETBGCOLOR_TEXT_BUTTON      1072
#define IDC_SETTEXTCOLOR_CARETLINE_BUTTON 1073
#define IDC_SETBGCOLOR_CARETLINE_BUTTON 1073
#define IDC_INDICATOR_STATIC            1074
#define IDC_SETBGCOLOR_CARETLINE_BUTTON2 1075
#define IDC_SETBGCOLOR_INDICATOR_BUTTON 1075
#define IDC_LINENUMBER_STATIC           1076
#define IDC_SETTEXTCOLOR_LINENUMBER_BUTTON 1077
#define IDC_SETBGCOLOR_LINENUMBER_BUTTON 1078
#define IDC_FOLDMARGIN_STATIC           1079
#define IDC_SETBGCOLOR_FOLDMARGIN_BUTTON 1080
#define IDC_SETTEXTCOLOR_LINENUMBER_BUTTON2 1081

#define IDC_UNKNOWTAGS_STATIC           1094
#define IDC_ATTRIBUTES_STATIC           1095
#define IDC_ATTRIBUTE_STATIC            1095
#define IDC_UNKNOWATTRIBUTES_STATIC     1096
#define IDC_ENTITIES_STATIC             1097
#define IDC_PHPSTART_STATIC             1098
#define IDC_PHPSECTION_STATIC           1098
#define IDC_TAGENDS_STATIC              1099
#define IDC_PHPEND_STATIC               1100
#define IDC_ASPEND_STATIC               1101
#define IDC_ASPSTART_STATIC             1102
#define IDC_ASPSECTION_STATIC           1102
#define IDC_SETFONT_UNKNOWTAGS_BUTTON   1103
#define IDC_SETTEXTCOLOR_UNKNOWTAGS_BUTTON 1104
#define IDC_SETFONT_ATTRIBUTES_BUTTON   1105
#define IDC_SETTEXTCOLOR_ATTRIBUTES_BUTTON 1106
#define IDC_SETFONT_UNKNOWATTRIBUTES_BUTTON 1107
#define IDC_SETTEXTCOLOR_UNKNOWATTRIBUTES_BUTTON 1108
#define IDC_SETFONT_ENTITIES_BUTTON     1109
#define IDC_SETTEXTCOLOR_ENTITIES_BUTTON 1110
#define IDC_SETFONT_TAGENDS_BUTTON      1111
#define IDC_SETTEXTCOLOR_TAGENDS_BUTTON 1112
#define IDC_SETFONT_PHPSTART_BUTTON     1113
#define IDC_SETFONT_PHPSECTION_BUTTON   1113
#define IDC_SETTEXTCOLOR_PHPSTART_BUTTON 1114
#define IDC_SETTEXTCOLOR_PHPSECTION_BUTTON 1114
#define IDC_SETFONT_PHPEND_BUTTON       1115
#define IDC_SETTEXTCOLOR_PHPEND_BUTTON  1116
#define IDC_SETFONT_ASPSTART_BUTTON     1117
#define IDC_SETFONT_ASPSECTION_BUTTON   1117
#define IDC_SETTEXTCOLOR_ASPSTART_BUTTON 1118
#define IDC_SETTEXTCOLOR_ASPSECTION_BUTTON 1118
#define IDC_SETFONT_ASPEND_BUTTON       1119
#define IDC_SETTEXTCOLOR_ASPEND_BUTTON  1120
#define IDC_TAGS_STATIC                 1121
#define IDC_SETFONT_TAGS_BUTTON         1122
#define IDC_SETTEXTCOLOR_TAGS_BUTTON    1123
#define IDC_CDATA_STATIC                1124
#define IDC_SETFONT_CDATA_BUTTON        1125
#define IDC_SETTEXTCOLOR_CDATA_BUTTON   1126
#define IDC_FOUNDLIST                   1127
#define ID_FOUNDLIST                    1128
#define IDC_LIST2                       1129
#define IDC_THEME_LANGUAGE_STATIC       1130
#define IDC_THEME_MARKUP_STATIC         1131
#define IDC_THEME_EDIT_STATIC           1132
#define IDC_THEME_MARGIN_STATIC         1133

#endif  // _EU_RESOURCE_H_
