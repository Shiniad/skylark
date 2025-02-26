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
#include "framework.h"

#ifndef ANY_WORD
#define ANY_WORD "///*///"
#endif

/* XPM */
static const char *minus_xpm[] = 
{
    /* width height num_colors chars_per_pixel */
    "     9     9       16            1",
    /* colors */
    "` c #8c96ac",
    ". c #c4d0da",
    "# c #daecf4",
    "a c #ccdeec",
    "b c #eceef4",
    "c c #e0e5eb",
    "d c #a7b7c7",
    "e c #e4ecf0",
    "f c #d0d8e2",
    "g c #b7c5d4",
    "h c #fafdfc",
    "i c #b4becc",
    "j c #d1e6f1",
    "k c #e4f2fb",
    "l c #ecf6fc",
    "m c #d4dfe7",
    /* pixels */
    "hbc.i.cbh",
    "bffeheffb",
    "mfllkllfm",
    "gjkkkkkji",
    "da`````jd",
    "ga#j##jai",
    "f.k##k#.a",
    "#..jkj..#",
    "hemgdgc#h"
};

static const char *plus_xpm[] = 
{
    /* width height num_colors chars_per_pixel */
    "     9     9       16            1",
    /* colors */
    "` c #242e44",
    ". c #8ea0b5",
    "# c #b7d5e4",
    "a c #dcf2fc",
    "b c #a2b8c8",
    "c c #ccd2dc",
    "d c #b8c6d4",
    "e c #f4f4f4",
    "f c #accadc",
    "g c #798fa4",
    "h c #a4b0c0",
    "i c #cde5f0",
    "j c #bcdeec",
    "k c #ecf1f6",
    "l c #acbccc",
    "m c #fcfefc",
    /* pixels */
    "mech.hcem",
    "eldikille",
    "dlaa`akld",
    ".#ii`ii#.",
    "g#`````fg",
    ".fjj`jjf.",
    "lbji`ijbd",
    "khb#idlhk",
    "mkd.ghdkm"
};

static doctype_t* g_doc_config;

static int
init_sc_fold(eu_tabpage *pnode)
{
    // 启用折叠
    eu_sci_call(pnode, SCI_SETPROPERTY, (sptr_t) "fold", (sptr_t) "1");
    // by adonais
    eu_sci_call(pnode, SCI_SETPROPERTY, (sptr_t) "fold.comment", (sptr_t) "1");
    eu_sci_call(pnode, SCI_SETPROPERTY, (sptr_t) "fold.preprocessor", (sptr_t) "1");
    eu_sci_call(pnode, SCI_SETPROPERTY, (sptr_t) "fold.compact", (sptr_t) "0");
    if (pnode->doc_ptr && pnode->doc_ptr->doc_type > 0)
    {
        switch (pnode->doc_ptr->doc_type)
        {
        case DOCTYPE_HTML:
        case DOCTYPE_XML:
            eu_sci_call(pnode, SCI_SETPROPERTY, (sptr_t) "fold.html", (sptr_t) "1");
            eu_sci_call(pnode, SCI_SETPROPERTY, (sptr_t) "fold.hypertext.comment", (sptr_t) "1");
            eu_sci_call(pnode, SCI_SETPROPERTY, (sptr_t) "fold.hypertext.heredoc", (sptr_t) "1");
            break;
        case DOCTYPE_CSS:
            eu_sci_call(pnode, SCI_SETPROPERTY, (sptr_t) "lexer.css.scss.language", (sptr_t) ((_tcsicmp(pnode->extname, _T(".scss")) == 0)? "1" : "0"));
            eu_sci_call(pnode, SCI_SETPROPERTY, (sptr_t) "lexer.css.less.language", (sptr_t) ((_tcsicmp(pnode->extname, _T(".less")) == 0)? "1" : "0"));
            eu_sci_call(pnode, SCI_SETPROPERTY, (sptr_t) "lexer.css.hss.language", (sptr_t) ((_tcsicmp(pnode->extname, _T(".hss")) == 0)? "1" : "0"));
            break;
        default:
            break;
        }
    }
    eu_sci_call(pnode, SCI_SETMARGINTYPEN, MARGIN_FOLD_INDEX, SC_MARGIN_SYMBOL); // 页边类型
    eu_sci_call(pnode, SCI_SETMARGINMASKN, MARGIN_FOLD_INDEX, SC_MASK_FOLDERS);  // 页边掩码
    // 页边宽度
    eu_sci_call(pnode, SCI_SETMARGINWIDTHN, MARGIN_FOLD_INDEX, (eu_get_config()->block_fold ? MARGIN_FOLD_WIDTH : 0));
    eu_sci_call(pnode, SCI_SETMARGINSENSITIVEN, MARGIN_FOLD_INDEX, true); //响应鼠标消息
    /* 折叠标签样式 */
    eu_sci_call(pnode, SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PIXMAP);
    eu_sci_call(pnode, SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_PIXMAP);
    eu_sci_call(pnode, SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_PIXMAP);
    eu_sci_call(pnode, SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_PIXMAP);
    eu_sci_call(pnode, SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNERCURVE);
    eu_sci_call(pnode, SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
    eu_sci_call(pnode, SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNERCURVE);
    eu_sci_call(pnode, SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDER, (sptr_t) plus_xpm);
    eu_sci_call(pnode, SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEROPEN, (sptr_t) minus_xpm);
    eu_sci_call(pnode, SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEREND, (sptr_t) plus_xpm);
    eu_sci_call(pnode, SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEROPENMID, (sptr_t) minus_xpm);
    /* 折叠标签颜色 */
    eu_sci_call(pnode, SCI_MARKERSETBACK, SC_MARKNUM_FOLDERSUB, 0xa0a0a0);
    eu_sci_call(pnode, SCI_MARKERSETBACK, SC_MARKNUM_FOLDERMIDTAIL, 0xa0a0a0);
    eu_sci_call(pnode, SCI_MARKERSETBACK, SC_MARKNUM_FOLDERTAIL, 0xa0a0a0);
    
    // 折叠时不要在下面画折叠线
    eu_sci_call(pnode, SCI_SETFOLDFLAGS, 0, 0);
    eu_sci_call(pnode, SCI_FOLDDISPLAYTEXTSETSTYLE, SC_FOLDDISPLAYTEXT_BOXED, 0);
    eu_sci_call(pnode, SCI_SETDEFAULTFOLDDISPLAYTEXT, 0, (LPARAM)"\xC2\xB7\xC2\xB7\xC2\xB7");
    // 高亮显示当前折叠块
    eu_sci_call(pnode, SCI_MARKERENABLEHIGHLIGHT, (sptr_t) eu_get_config()->light_fold, 0);
    // 指示出不匹配的大括号
    eu_sci_call(pnode, SCI_BRACEBADLIGHTINDICATOR, true, INDIC_STRIKE);
    // 不产生鼠标悬浮消息(SCN_DWELLSTART, SCN_DWELLEND, 设置SC_TIME_FOREVER>0则产生
    eu_sci_call(pnode, SCI_SETMOUSEDWELLTIME, SC_TIME_FOREVER, 0);    
    return 0;
}

// (*init_before_ptr)
int
on_doc_init_list(eu_tabpage *pnode)
{
    if (!pnode)
    {
        return 1;
    }
    return on_symlist_create(pnode);
}

int
on_doc_init_list_sh(eu_tabpage *pnode)
{
    TCHAR *sp = on_doc_get_ext(pnode);
    if (sp && _tcsstr(_T(";*.bat;*.cmd;*.nt;"), sp))
    {
        return 0;
    }
    return on_doc_init_list(pnode);
}

int
on_doc_init_tree(eu_tabpage *pnode)
{
    if (!pnode)
    {
        return 1;
    }
    return on_symtree_create(pnode);
}

int
on_doc_init_result_list(eu_tabpage *pnode)
{
    if (!pnode)
    {
        return 1;
    }
    if (on_doc_init_list(pnode))
    {
        return 1;
    }
    if (on_result_create_dlg(pnode))
    {
        return 1;
    }
    return 0;
}

int
on_doc_init_result(eu_tabpage *pnode)
{
    if (!pnode)
    {
        return 1;
    }
    if (on_symtree_create(pnode))
    {
        return 1;
    }
    if (on_result_create_dlg(pnode))
    {
        return 1;
    }
    if (on_table_create_query_box(pnode))
    {
        return 1;
    }
    return 0;
}

static void
init_systree_theme(eu_tabpage *pnode)
{
    if (pnode && pnode->hwnd_symtree)
    {
        SendMessage(pnode->hwnd_symtree, WM_SETFONT, (WPARAM) on_theme_font_hwnd(), 0);
        SendMessage(pnode->hwnd_symtree, TVM_SETTEXTCOLOR, 0, eu_get_theme()->item.text.color);
        SendMessage(pnode->hwnd_symtree, TVM_SETLINECOLOR, 0, eu_get_theme()->item.text.color);
        SendMessage(pnode->hwnd_symtree, TVM_SETBKCOLOR, 0, eu_get_theme()->item.text.bgcolor);
    }
}

static void
on_doc_set_keyword(eu_tabpage *pnode, int index)
{
    if (!(pnode && pnode->doc_ptr))
    {
        return;
    }
    switch (index)
    {
        case 0:
            if (pnode->doc_ptr->keywords0)
            {   // keyword?
                eu_sci_call(pnode, SCI_SETKEYWORDS, 0, (sptr_t)(pnode->doc_ptr->keywords0));
                break;
            }
        case 1:
            if (pnode->doc_ptr->keywords1)
            {   // function?
                eu_sci_call(pnode, SCI_SETKEYWORDS, 1, (sptr_t)(pnode->doc_ptr->keywords1));
                break;
            }
        case 2:
            if (pnode->doc_ptr->keywords2)
            {   // macro?
                eu_sci_call(pnode, SCI_SETKEYWORDS, 2, (sptr_t)(pnode->doc_ptr->keywords2));
                break;
            }
        case 3:
            if (pnode->doc_ptr->keywords3)
            {   // SENT?
                eu_sci_call(pnode, SCI_SETKEYWORDS, 3, (sptr_t)(pnode->doc_ptr->keywords3));
                break;
            }
        case 4:
            if (pnode->doc_ptr->keywords4)
            {   // PREPROCESSOR?
                eu_sci_call(pnode, SCI_SETKEYWORDS, 4, (sptr_t)(pnode->doc_ptr->keywords4));
                break;
            }
        case 5:
            if (pnode->doc_ptr->keywords5)
            {   // SPECIAL?
                eu_sci_call(pnode, SCI_SETKEYWORDS, 5, (sptr_t)(pnode->doc_ptr->keywords5));
                break;
            }
        default:
            break;
    }
}

// (*init_after_ptr)
int
on_doc_enable_scilexer(eu_tabpage *pnode, int lex)
{
    int res = 0;
    if (pnode)
    {
        eu_sci_call(pnode, SCI_SETLEXER, lex, 0);
        if (pnode->doc_ptr && pnode->doc_ptr->fn_reload_symlist)
        {
            res = pnode->doc_ptr->fn_reload_symlist(pnode);
        }        
        init_sc_fold(pnode);
    }
    return res;
}

void
on_doc_default_light(eu_tabpage *pnode, int lex, int64_t rgb)
{
    if (pnode)
    {
        if (rgb)
        {
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
        }
        else
        {
            eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.text.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.text.fontsize));
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.text.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.text.bold));
        }    
    }
}

void
on_doc_keyword_light(eu_tabpage *pnode, int lex, int index, int64_t rgb) 
{
    if (pnode)
    {
        on_doc_set_keyword(pnode, index);
        if (rgb)
        {
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
            return;
        }
        switch (index)
        {
            case 0:
                eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.keywords0.font));
                eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.keywords0.fontsize));
                eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.keywords0.color));
                eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.keywords0.bold)); 
                break;
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.keywords1.font));
                eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.keywords1.fontsize));
                eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.keywords1.color));
                eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.keywords1.bold)); 
                break;
            default:
                break;
        }
    }
}

void
on_doc_function_light(eu_tabpage *pnode, int lex, int index, int64_t rgb) 
{
    if (pnode)
    {
        on_doc_set_keyword(pnode, index);
        if (rgb)
        {
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
        }
        else
        {
            eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.keywords1.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.keywords1.fontsize));
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.keywords1.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.keywords1.bold));
        }
    }
}

void
on_doc_marcro_light(eu_tabpage *pnode, int lex, int index, int64_t rgb)
{
    if (pnode)
    {
        on_doc_set_keyword(pnode, index);
        if (!rgb)
        {
            eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.preprocessor.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.preprocessor.fontsize));
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.preprocessor.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.preprocessor.bold));
        }
        else
        {
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
        }
    }
}

void
on_doc_preprocessor_light(eu_tabpage *pnode, int lex, int index, int64_t rgb)
{
    if (pnode)
    {
        on_doc_set_keyword(pnode, index);
        if (!rgb)
        {
            eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.preprocessor.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.preprocessor.fontsize));
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.preprocessor.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.preprocessor.bold));
        }
        else
        {
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
        }
    }
}

void 
on_doc_send_light(eu_tabpage *pnode, int lex, int index, int64_t rgb)
{
    if (pnode && rgb)
    {
        on_doc_set_keyword(pnode, index);
        eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
    }    
}

void
on_doc_variable_light(eu_tabpage *pnode, int lex, int64_t rgb)
{
    if (pnode && rgb)
    {
        eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
    }
}

void
on_doc_tags_light(eu_tabpage *pnode, int lex, int64_t rgb)
{
    if (pnode)
    {
        if (!rgb)
        {
            eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.tags.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.tags.fontsize));
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.tags.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.tags.bold));
        }
        else
        {
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
        }
    }
}

void
on_doc_string_light(eu_tabpage *pnode, int lex, int64_t rgb)
{
    if (pnode)
    {
        if (!rgb)
        {   // set string light
            eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.string.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.string.fontsize));
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.string.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.string.bold));
        }
        else
        {
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
        }
    }
}

void
on_doc_operator_light(eu_tabpage *pnode, int lex, int64_t rgb)
{
    if (pnode)
    {
        if (!rgb)
        {   // set string light
            eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.operators.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.operators.fontsize));
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.operators.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.operators.bold));
        }
        else
        {
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
        }
    }
}

void
on_doc_char_light(eu_tabpage *pnode, int lex, int64_t rgb)
{
    if (pnode)
    {
        if (!rgb)
        {
            eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.character.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.character.fontsize));
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.character.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.character.bold));
        }
        else
        {
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
        }
    }
}

void
on_doc_number_light(eu_tabpage *pnode, int lex, int64_t rgb)
{
    if (pnode)
    {
        if (!rgb)
        {
            eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.number.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.number.fontsize));
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.number.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.number.bold));
        }
        else
        {
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
        }
    }
}

void
on_doc_special_light(eu_tabpage *pnode, int lex, int64_t rgb)
{
    if (pnode && rgb)
    {
        eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
    }
}

void
on_doc_comment_light(eu_tabpage *pnode, int lex, int64_t rgb)
{
    if (pnode)
    {
        if (!rgb)
        {
            eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.commentline.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.commentline.fontsize));
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.commentline.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.commentline.bold));
        }
        else
        {
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
        }
    }
}

void
on_doc_commentblock_light(eu_tabpage *pnode, int lex, int64_t rgb)
{
    if (pnode)
    {
        if (!rgb)
        {
            eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.comment.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.comment.fontsize));
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.comment.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.comment.bold));
        }
        else
        {
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
        }
    }
}

void
on_doc_commentdoc_light(eu_tabpage *pnode, int lex, int64_t rgb)
{
    if (pnode)
    {
        if (!rgb)
        {
            eu_sci_call(pnode, SCI_STYLESETFONT, lex, (sptr_t)(eu_get_theme()->item.commentdoc.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, lex, (sptr_t)(eu_get_theme()->item.commentdoc.fontsize));
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, (sptr_t)(eu_get_theme()->item.commentdoc.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, lex, (sptr_t)(eu_get_theme()->item.commentdoc.bold));
        }
        else
        {
            eu_sci_call(pnode, SCI_STYLESETFORE, lex, rgb);
        }
    }
}

int
on_doc_init_after_cpp(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_CPP, 0);
    // Disable track preprocessor to avoid incorrect detection.
    // In the most of cases, the symbols are defined outside of file.
    eu_sci_call(pnode, SCI_SETPROPERTY, (WPARAM)("lexer.cpp.track.preprocessor"), (LPARAM)"0");
    on_doc_keyword_light(pnode, SCE_C_WORD, 0, 0);
    on_doc_function_light(pnode, SCE_C_WORD2, 1, 0);
    on_doc_string_light(pnode, SCE_C_STRING, 0);
    on_doc_char_light(pnode, SCE_C_CHARACTER, 0);
    on_doc_number_light(pnode, SCE_C_NUMBER, 0);
    on_doc_operator_light(pnode, SCE_C_OPERATOR, 0);
    on_doc_comment_light(pnode, SCE_C_COMMENTLINE, 0);
    on_doc_commentblock_light(pnode, SCE_C_COMMENT, 0);
    on_doc_commentdoc_light(pnode, SCE_C_COMMENTDOC, 0);
    on_doc_preprocessor_light(pnode, SCE_C_PREPROCESSOR, -1, 0);
    on_doc_commentblock_light(pnode, SCE_C_PREPROCESSORCOMMENT, 0);
    on_doc_commentblock_light(pnode, SCE_C_PREPROCESSORCOMMENTDOC, 0);
    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_cs(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_CPP, 0);
    on_doc_keyword_light(pnode, SCE_C_WORD, 0, 0);
    on_doc_string_light(pnode, SCE_C_STRING, 0);
    on_doc_char_light(pnode, SCE_C_CHARACTER, 0);
    on_doc_number_light(pnode, SCE_C_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_C_COMMENTLINE, 0);
    on_doc_commentblock_light(pnode, SCE_C_COMMENT, 0);
    on_doc_preprocessor_light(pnode, SCE_C_PREPROCESSOR, -1, 0);
    on_doc_commentdoc_light(pnode, SCE_C_COMMENTDOC, 0);
    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_java(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_CPP, 0);
    on_doc_keyword_light(pnode, SCE_C_WORD, 0, 0);
    on_doc_string_light(pnode, SCE_C_STRING, 0);
    on_doc_char_light(pnode, SCE_C_CHARACTER, 0);
    on_doc_number_light(pnode, SCE_C_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_C_COMMENTLINE, 0);
    on_doc_commentblock_light(pnode, SCE_C_COMMENT, 0);
    on_doc_preprocessor_light(pnode, SCE_C_PREPROCESSOR, -1, 0);
    on_doc_commentdoc_light(pnode, SCE_C_COMMENTDOC, 0);
    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_go(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_CPP, 0);
    on_doc_keyword_light(pnode, SCE_C_WORD, 0, 0);
    on_doc_string_light(pnode, SCE_C_STRING, 0);
    on_doc_char_light(pnode, SCE_C_CHARACTER, 0);
    on_doc_number_light(pnode, SCE_C_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_C_COMMENTLINE, 0);
    on_doc_commentblock_light(pnode, SCE_C_COMMENT, 0);
    on_doc_preprocessor_light(pnode, SCE_C_PREPROCESSOR, -1, 0);
    on_doc_commentdoc_light(pnode, SCE_C_COMMENTDOC, 0);
    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_swift(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_CPP, 0);
    on_doc_keyword_light(pnode, SCE_C_WORD, 0, 0);
    on_doc_string_light(pnode, SCE_C_STRING, 0);
    on_doc_char_light(pnode, SCE_C_CHARACTER, 0);
    on_doc_number_light(pnode, SCE_C_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_C_COMMENTLINE, 0);
    on_doc_commentblock_light(pnode, SCE_C_COMMENT, 0);
    on_doc_preprocessor_light(pnode, SCE_C_PREPROCESSOR, -1, 0);
    on_doc_commentdoc_light(pnode, SCE_C_COMMENTDOC, 0);
    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_sql(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_SQL, 0);
    on_doc_keyword_light(pnode, SCE_SQL_WORD, 0, 0);
    on_doc_string_light(pnode, SCE_C_STRING, 0);
    on_doc_char_light(pnode, SCE_C_CHARACTER, 0);
    on_doc_number_light(pnode, SCE_C_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_C_COMMENTLINE, 0);
    on_doc_commentblock_light(pnode, SCE_C_COMMENT, 0);
    on_doc_commentdoc_light(pnode, SCE_C_COMMENTDOC, 0);
    init_sc_fold(pnode);
    init_systree_theme(pnode);
    return 0;
}

int
on_doc_init_after_redis(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_CPP, 0);
    on_doc_keyword_light(pnode, SCE_C_WORD, 0, 0);
    init_sc_fold(pnode);
    init_systree_theme(pnode);
    return 0;
}

int
on_doc_init_after_python(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_PYTHON, 0);
    on_doc_keyword_light(pnode, SCE_P_WORD, 0, 0);
    on_doc_function_light(pnode, SCE_P_WORD2, 1, 0);
    on_doc_string_light(pnode, SCE_P_STRING, 0);
    on_doc_char_light(pnode, SCE_P_CHARACTER, 0);
    on_doc_number_light(pnode, SCE_P_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_P_COMMENTLINE, 0);
    on_doc_commentblock_light(pnode, SCE_P_COMMENTBLOCK, 0);
    on_doc_commentdoc_light(pnode, SCE_C_COMMENTDOC, 0);  
    on_doc_operator_light(pnode, SCE_P_OPERATOR, 0);
    
    on_doc_string_light(pnode, SCE_P_TRIPLE, 0);
    on_doc_string_light(pnode, SCE_P_TRIPLEDOUBLE, 0);
    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_lua(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_LUA, 0);
    on_doc_keyword_light(pnode, SCE_LUA_WORD, 0, 0);
    on_doc_function_light(pnode, SCE_LUA_WORD2, 1, 0);
    on_doc_string_light(pnode, SCE_LUA_STRING, 0);
    on_doc_char_light(pnode, SCE_LUA_CHARACTER, 0);
    on_doc_number_light(pnode, SCE_LUA_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_LUA_COMMENTLINE, 0);
    on_doc_commentdoc_light(pnode, SCE_LUA_COMMENTDOC, 0);  
    on_doc_operator_light(pnode, SCE_LUA_OPERATOR, 0);
    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_perl(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_PERL, 0);
    on_doc_keyword_light(pnode, SCE_PL_WORD, 0, 0);
    on_doc_string_light(pnode, SCE_PL_STRING, 0);
    on_doc_char_light(pnode, SCE_PL_CHARACTER, 0);
    on_doc_number_light(pnode, SCE_PL_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_PL_COMMENTLINE, 0);
    on_doc_operator_light(pnode, SCE_PL_OPERATOR, 0);
    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_shell(eu_tabpage *pnode)
{
    TCHAR *sp = on_doc_get_ext(pnode);
    if (sp && _tcsstr(_T(";*.ps1;*.psc1;*.psd1;*.psm1;"), sp))
    {
        eu_sci_call(pnode, SCI_SETLEXER, SCLEX_POWERSHELL, 0);
        if (pnode->doc_ptr->keywords3)
        {
            eu_sci_call(pnode, SCI_SETKEYWORDS, 0, (sptr_t)(pnode->doc_ptr->keywords3));
            eu_sci_call(pnode, SCI_STYLESETFONT, SCE_POWERSHELL_KEYWORD, (sptr_t)(eu_get_theme()->item.keywords0.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_POWERSHELL_KEYWORD, eu_get_theme()->item.keywords0.fontsize);
            eu_sci_call(pnode, SCI_STYLESETFORE, SCE_POWERSHELL_KEYWORD, (sptr_t)(eu_get_theme()->item.keywords0.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_POWERSHELL_KEYWORD, (sptr_t)(eu_get_theme()->item.keywords0.bold));
        }
        if (pnode->doc_ptr->keywords4)
        {
            eu_sci_call(pnode, SCI_SETKEYWORDS, 1, (sptr_t)(pnode->doc_ptr->keywords4));
            eu_sci_call(pnode, SCI_STYLESETFONT, SCE_POWERSHELL_CMDLET, (sptr_t)(eu_get_theme()->item.tagends.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_POWERSHELL_CMDLET, eu_get_theme()->item.tagends.fontsize);
            eu_sci_call(pnode, SCI_STYLESETFORE, SCE_POWERSHELL_CMDLET, (sptr_t)(eu_get_theme()->item.tagends.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_POWERSHELL_CMDLET, (sptr_t)(eu_get_theme()->item.tagends.bold));
        }
        on_doc_operator_light(pnode, SCE_POWERSHELL_OPERATOR, 0);
        on_doc_number_light(pnode, SCE_POWERSHELL_NUMBER, 0);
        on_doc_comment_light(pnode, SCE_POWERSHELL_COMMENT, 0);
        on_doc_commentblock_light(pnode, SCE_POWERSHELL_COMMENTSTREAM, 0);
        on_doc_preprocessor_light(pnode, SCE_POWERSHELL_VARIABLE, -1, 0);
    }
    else
    {
        eu_sci_call(pnode, SCI_SETLEXER, SCLEX_BASH, 0);
        on_doc_keyword_light(pnode, SCE_SH_WORD, 0, 0);
        on_doc_string_light(pnode, SCE_SH_STRING, 0);
        on_doc_char_light(pnode, SCE_SH_CHARACTER, 0);
        on_doc_number_light(pnode, SCE_SH_NUMBER, 0);
        on_doc_comment_light(pnode, SCE_SH_COMMENTLINE, 0);
        on_doc_operator_light(pnode, SCE_SH_OPERATOR, 0);
    }
    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

TCHAR *
on_doc_get_ext(eu_tabpage *pnode)
{
    if (pnode && STR_NOT_NUL(pnode->pathfile))
    {
        TCHAR *p = NULL;
        if (!(p = _tcsrchr(pnode->pathfile, _T('\\'))))
        {
            p = _tcsrchr(pnode->pathfile, _T('/'));
        }
        if (p)
        {
            return _tcsrchr(p, _T('.'));
        }
    }
    return NULL;
}

int
on_doc_init_after_shell_sh(eu_tabpage *pnode)
{
    if (!pnode)
    {
        return 1;
    }
    TCHAR *sp = on_doc_get_ext(pnode);
    if (sp && _tcsstr(_T(";*.bat;*.cmd;*.nt;"), sp))
    {
        eu_sci_call(pnode, SCI_SETLEXER, SCLEX_BATCH, 0);
        on_doc_default_light(pnode, SCE_BAT_DEFAULT, 0);
        if (pnode->doc_ptr->keywords1)
        {
            eu_sci_call(pnode, SCI_SETKEYWORDS, 0, (sptr_t)(pnode->doc_ptr->keywords1));
            eu_sci_call(pnode, SCI_STYLESETFONT, SCE_BAT_WORD, (sptr_t)(eu_get_theme()->item.keywords0.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_BAT_WORD, eu_get_theme()->item.keywords0.fontsize);
            eu_sci_call(pnode, SCI_STYLESETFORE, SCE_BAT_WORD, (sptr_t)(eu_get_theme()->item.keywords0.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_BAT_WORD, (sptr_t)(eu_get_theme()->item.keywords0.bold));
        }
        on_doc_char_light(pnode, SCE_BAT_LABEL, 0);
        on_doc_comment_light(pnode, SCE_BAT_COMMENT, 0);

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_BAT_HIDE, (sptr_t)(eu_get_theme()->item.unknowtags.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_BAT_HIDE, eu_get_theme()->item.unknowtags.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_BAT_HIDE, (sptr_t)(eu_get_theme()->item.unknowtags.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_BAT_HIDE, (sptr_t)(eu_get_theme()->item.unknowtags.bold));
                
        if (pnode->doc_ptr->keywords2)
        {
            eu_sci_call(pnode, SCI_SETKEYWORDS, 1, (sptr_t)(pnode->doc_ptr->keywords2));
            eu_sci_call(pnode, SCI_STYLESETFONT, SCE_BAT_COMMAND, (sptr_t)(eu_get_theme()->item.keywords1.font));
            eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_BAT_COMMAND, eu_get_theme()->item.keywords1.fontsize);
            eu_sci_call(pnode, SCI_STYLESETFORE, SCE_BAT_COMMAND, (sptr_t)(eu_get_theme()->item.keywords1.color));
            eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_BAT_COMMAND, (sptr_t)(eu_get_theme()->item.keywords1.bold));
        }
        on_doc_string_light(pnode, SCE_BAT_IDENTIFIER, 0);
        on_doc_operator_light(pnode, SCE_BAT_OPERATOR, 0);
        return 0;
    }
    return on_doc_init_after_shell(pnode);
}

int
on_doc_init_after_rust(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_RUST, 0);
    on_doc_keyword_light(pnode, SCE_RUST_WORD, 0, 0);
    on_doc_function_light(pnode, SCE_RUST_WORD2, 1, 0);
    on_doc_string_light(pnode, SCE_RUST_STRING, 0);
    on_doc_char_light(pnode, SCE_RUST_CHARACTER, 0);
    on_doc_number_light(pnode, SCE_RUST_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_RUST_COMMENTLINE, 0);
    on_doc_commentblock_light(pnode, SCE_RUST_COMMENTBLOCK, 0);
    on_doc_operator_light(pnode, SCE_RUST_OPERATOR, 0);
    on_doc_comment_light(pnode, SCE_RUST_COMMENTLINEDOC, 0);
    on_doc_commentdoc_light(pnode, SCE_RUST_COMMENTBLOCKDOC, 0);
    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_ruby(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_RUBY, 0);
    on_doc_keyword_light(pnode, SCE_RB_WORD, 0, 0);
    on_doc_function_light(pnode, SCE_RB_WORD_DEMOTED, 1, 0);
    on_doc_string_light(pnode, SCE_RB_STRING, 0);
    on_doc_char_light(pnode, SCE_RB_CHARACTER, 0);
    on_doc_number_light(pnode, SCE_RB_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_RB_COMMENTLINE, 0);
    on_doc_operator_light(pnode, SCE_RB_OPERATOR, 0);
    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_lisp(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_LISP, 0);
    on_doc_keyword_light(pnode, SCE_LISP_KEYWORD, 0, 0);
    on_doc_string_light(pnode, SCE_LISP_STRING, 0);
    on_doc_number_light(pnode, SCE_LISP_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_LISP_COMMENT, 0);
    on_doc_commentdoc_light(pnode, SCE_LISP_MULTI_COMMENT, 0);
    on_doc_operator_light(pnode, SCE_LISP_OPERATOR, 0);
    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_asm(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_ASM, 0);

    if (pnode->doc_ptr->keywords0)
    {
        on_doc_keyword_light(pnode, SCE_ASM_CPUINSTRUCTION, 0, 0);

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_ASM_MATHINSTRUCTION, (sptr_t)(eu_get_theme()->item.keywords0.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_ASM_MATHINSTRUCTION, eu_get_theme()->item.keywords0.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_ASM_MATHINSTRUCTION, (sptr_t)(eu_get_theme()->item.keywords0.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_ASM_MATHINSTRUCTION, (sptr_t)(eu_get_theme()->item.keywords0.bold));

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_ASM_REGISTER, (sptr_t)(eu_get_theme()->item.keywords0.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_ASM_REGISTER, eu_get_theme()->item.keywords0.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_ASM_REGISTER, (sptr_t)(eu_get_theme()->item.keywords0.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_ASM_REGISTER, (sptr_t)(eu_get_theme()->item.keywords0.bold));

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_ASM_DIRECTIVE, (sptr_t)(eu_get_theme()->item.keywords0.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_ASM_DIRECTIVE, eu_get_theme()->item.keywords0.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_ASM_DIRECTIVE, (sptr_t)(eu_get_theme()->item.keywords0.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_ASM_DIRECTIVE, (sptr_t)(eu_get_theme()->item.keywords0.bold));

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_ASM_DIRECTIVEOPERAND, (sptr_t)(eu_get_theme()->item.keywords0.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_ASM_DIRECTIVEOPERAND, eu_get_theme()->item.keywords0.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_ASM_DIRECTIVEOPERAND, (sptr_t)(eu_get_theme()->item.keywords0.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_ASM_DIRECTIVEOPERAND, (sptr_t)(eu_get_theme()->item.keywords0.bold));

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_ASM_EXTINSTRUCTION, (sptr_t)(eu_get_theme()->item.keywords0.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_ASM_EXTINSTRUCTION, eu_get_theme()->item.keywords0.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_ASM_EXTINSTRUCTION, (sptr_t)(eu_get_theme()->item.keywords0.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_ASM_EXTINSTRUCTION, (sptr_t)(eu_get_theme()->item.keywords0.bold));
    }
    on_doc_string_light(pnode, SCE_ASM_STRING, 0);
    on_doc_number_light(pnode, SCE_ASM_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_ASM_COMMENT, 0);
    on_doc_comment_light(pnode, SCE_ASM_COMMENTDIRECTIVE, 0);
    on_doc_operator_light(pnode, SCE_ASM_OPERATOR, 0);
    
    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_nim(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_NIM, 0);
    
    on_doc_default_light(pnode, SCE_NIM_DEFAULT, 0);
    on_doc_keyword_light(pnode, SCE_NIM_WORD, 0, 0);
    on_doc_string_light(pnode, SCE_NIM_STRING, 0);
    on_doc_char_light(pnode, SCE_NIM_STRING, 0);
    on_doc_number_light(pnode, SCE_NIM_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_NIM_COMMENTLINE, 0);
    on_doc_commentblock_light(pnode, SCE_NIM_COMMENT, 0);
    on_doc_commentdoc_light(pnode, SCE_NIM_COMMENTDOC, 0);
    on_doc_operator_light(pnode, SCE_NIM_OPERATOR, 0);

    on_doc_commentdoc_light(pnode, SCE_NIM_COMMENTLINEDOC, 0);
    on_doc_string_light(pnode, SCE_NIM_TRIPLE, 0);
    on_doc_string_light(pnode, SCE_NIM_TRIPLEDOUBLE, 0);
    on_doc_string_light(pnode, SCE_NIM_STRINGEOL, 0);
    on_doc_tags_light(pnode, SCE_NIM_FUNCNAME, 0);
    
    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_NIM_NUMERROR, (sptr_t)(eu_get_theme()->item.unknowtags.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_NIM_NUMERROR, eu_get_theme()->item.unknowtags.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_NIM_NUMERROR, (sptr_t)(eu_get_theme()->item.unknowtags.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_NIM_NUMERROR, (sptr_t)(eu_get_theme()->item.unknowtags.bold));

    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_cobol(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_COBOL, 0);
    on_doc_keyword_light(pnode, SCE_C_WORD, 0, 0);

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_C_UUID, (sptr_t)(eu_get_theme()->item.keywords0.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_C_UUID, eu_get_theme()->item.keywords0.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_C_UUID, (sptr_t)(eu_get_theme()->item.keywords0.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_C_UUID, (sptr_t)(eu_get_theme()->item.keywords0.bold));
    
    on_doc_function_light(pnode, SCE_C_WORD2, 1, 0);
    on_doc_string_light(pnode, SCE_C_STRING, 0);
    on_doc_char_light(pnode, SCE_C_STRING, 0);
    on_doc_number_light(pnode, SCE_C_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_C_COMMENTLINE, 0);
    on_doc_commentdoc_light(pnode, SCE_C_COMMENTDOC, 0);
    on_doc_operator_light(pnode, SCE_C_OPERATOR, 0);
    on_doc_preprocessor_light(pnode, SCE_C_PREPROCESSOR, -1, 0);

    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_html(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_HTML, 0);

    if (pnode->doc_ptr->keywords0)
    {
        eu_sci_call(pnode, SCI_SETKEYWORDS, 0, (sptr_t)(pnode->doc_ptr->keywords0));
       
        on_doc_tags_light(pnode, SCE_H_TAG, 0);
        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_TAGUNKNOWN, (sptr_t)(eu_get_theme()->item.unknowtags.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_TAGUNKNOWN, eu_get_theme()->item.unknowtags.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_TAGUNKNOWN, (sptr_t)(eu_get_theme()->item.unknowtags.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_TAGUNKNOWN, (sptr_t)(eu_get_theme()->item.unknowtags.bold));

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_ATTRIBUTE, (sptr_t)(eu_get_theme()->item.attributes.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_ATTRIBUTE, eu_get_theme()->item.attributes.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_ATTRIBUTE, (sptr_t)(eu_get_theme()->item.attributes.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_ATTRIBUTE, (sptr_t)(eu_get_theme()->item.attributes.bold));
        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_ATTRIBUTEUNKNOWN, (sptr_t)(eu_get_theme()->item.unknowattributes.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_ATTRIBUTEUNKNOWN, eu_get_theme()->item.unknowattributes.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_ATTRIBUTEUNKNOWN, (sptr_t)(eu_get_theme()->item.unknowattributes.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_ATTRIBUTEUNKNOWN, (sptr_t)(eu_get_theme()->item.unknowattributes.bold));
        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_ENTITY, (sptr_t)(eu_get_theme()->item.entities.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_ENTITY, eu_get_theme()->item.entities.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_ENTITY, (sptr_t)(eu_get_theme()->item.entities.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_ENTITY, (sptr_t)(eu_get_theme()->item.entities.bold));

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_TAGEND, (sptr_t)(eu_get_theme()->item.tagends.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_TAGEND, eu_get_theme()->item.tagends.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_TAGEND, (sptr_t)(eu_get_theme()->item.tagends.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_TAGEND, (sptr_t)(eu_get_theme()->item.tagends.bold));

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_CDATA, (sptr_t)(eu_get_theme()->item.cdata.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_CDATA, eu_get_theme()->item.cdata.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_CDATA, (sptr_t)(eu_get_theme()->item.cdata.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_CDATA, (sptr_t)(eu_get_theme()->item.cdata.bold));

        on_doc_string_light(pnode, SCE_H_DOUBLESTRING, 0);
        on_doc_number_light(pnode, SCE_H_NUMBER, 0);
        on_doc_char_light(pnode, SCE_H_SINGLESTRING, 0);
        on_doc_commentblock_light(pnode, SCE_H_COMMENT, 0);

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_QUESTION, (sptr_t)(eu_get_theme()->item.phpsection.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_QUESTION, eu_get_theme()->item.phpsection.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_QUESTION, (sptr_t)(eu_get_theme()->item.phpsection.color));
        eu_sci_call(pnode, SCI_STYLESETBACK, SCE_H_QUESTION, (sptr_t)(eu_get_theme()->item.aspsection.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_QUESTION, (sptr_t)(eu_get_theme()->item.phpsection.bold));

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_SCRIPT, (sptr_t)(eu_get_theme()->item.phpsection.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_SCRIPT, eu_get_theme()->item.phpsection.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_SCRIPT, (sptr_t)(eu_get_theme()->item.phpsection.color));
        eu_sci_call(pnode, SCI_STYLESETBACK, SCE_H_SCRIPT, (sptr_t)(eu_get_theme()->item.aspsection.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_SCRIPT, (sptr_t)(eu_get_theme()->item.phpsection.bold));

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_ASP, (sptr_t)(eu_get_theme()->item.phpsection.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_ASP, eu_get_theme()->item.phpsection.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_ASP, (sptr_t)(eu_get_theme()->item.phpsection.color));
        eu_sci_call(pnode, SCI_STYLESETBACK, SCE_H_ASP, (sptr_t)(eu_get_theme()->item.aspsection.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_ASP, (sptr_t)(eu_get_theme()->item.phpsection.bold));

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_ASPAT, (sptr_t)(eu_get_theme()->item.phpsection.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_ASPAT, eu_get_theme()->item.phpsection.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_ASPAT, (sptr_t)(eu_get_theme()->item.phpsection.color));
        eu_sci_call(pnode, SCI_STYLESETBACK, SCE_H_ASPAT, (sptr_t)(eu_get_theme()->item.aspsection.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_ASPAT, (sptr_t)(eu_get_theme()->item.phpsection.bold));
    }

    /* JavaScript */
    if (pnode->doc_ptr->keywords2)
    {
        eu_sci_call(pnode, SCI_SETKEYWORDS, 1, (sptr_t)(pnode->doc_ptr->keywords2));
        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HJ_START, (sptr_t)(eu_get_theme()->item.phpsection.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HJ_START, eu_get_theme()->item.phpsection.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HJ_START, (sptr_t)(eu_get_theme()->item.phpsection.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HJ_START, (sptr_t)(eu_get_theme()->item.phpsection.bold));

        on_doc_string_light(pnode, SCE_HJ_DOUBLESTRING, 0);
        on_doc_char_light(pnode, SCE_HJ_SINGLESTRING, 0);
        on_doc_number_light(pnode, SCE_HJ_NUMBER, 0);
        on_doc_comment_light(pnode, SCE_HJ_COMMENTLINE, 0);
        on_doc_commentblock_light(pnode, SCE_HJ_COMMENT, 0);
        on_doc_commentdoc_light(pnode, SCE_HJ_COMMENTDOC, 0);

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HJ_WORD, (sptr_t)(eu_get_theme()->item.keywords0.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HJ_WORD, eu_get_theme()->item.keywords0.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HJ_WORD, (sptr_t)(eu_get_theme()->item.keywords0.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HJ_WORD, (sptr_t)(eu_get_theme()->item.keywords0.bold));

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HJ_KEYWORD, (sptr_t)(eu_get_theme()->item.keywords0.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HJ_KEYWORD, eu_get_theme()->item.keywords0.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HJ_KEYWORD, (sptr_t)(eu_get_theme()->item.keywords0.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HJ_KEYWORD, (sptr_t)(eu_get_theme()->item.keywords0.bold));

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HJA_START, (sptr_t)(eu_get_theme()->item.phpsection.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HJA_START, eu_get_theme()->item.phpsection.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HJA_START, (sptr_t)(eu_get_theme()->item.phpsection.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HJA_START, (sptr_t)(eu_get_theme()->item.phpsection.bold));

        on_doc_string_light(pnode, SCE_HJA_DOUBLESTRING, 0);
        on_doc_char_light(pnode, SCE_HJA_SINGLESTRING, 0);
        on_doc_number_light(pnode, SCE_HJA_NUMBER, 0);
        on_doc_comment_light(pnode, SCE_HJA_COMMENTLINE, 0);
        on_doc_commentblock_light(pnode, SCE_HJA_COMMENT, 0);
        on_doc_commentdoc_light(pnode, SCE_HJA_COMMENTDOC, 0);

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HJA_KEYWORD, (sptr_t)(eu_get_theme()->item.keywords0.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HJA_KEYWORD, eu_get_theme()->item.keywords0.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HJA_KEYWORD, (sptr_t)(eu_get_theme()->item.keywords0.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HJA_KEYWORD, (sptr_t)(eu_get_theme()->item.keywords0.bold));
    }

    /* VBScript */
    if (pnode->doc_ptr->keywords3)
    {
        eu_sci_call(pnode, SCI_SETKEYWORDS, 2, (sptr_t)(pnode->doc_ptr->keywords3));
        for (int style = SCE_HB_START; style <= SCE_HB_STRINGEOL; style++)
        {
            eu_sci_call(pnode, SCI_STYLESETBACK, style, (sptr_t)(eu_get_theme()->item.aspsection.color));
        }
        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HB_START, (sptr_t)(eu_get_theme()->item.phpsection.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HB_START, eu_get_theme()->item.phpsection.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HB_START, (sptr_t)(eu_get_theme()->item.phpsection.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HB_START, (sptr_t)(eu_get_theme()->item.phpsection.bold));
        on_doc_string_light(pnode, SCE_HB_STRING, 0);
        on_doc_number_light(pnode, SCE_HB_NUMBER, 0);
        on_doc_comment_light(pnode, SCE_HB_COMMENTLINE, 0);

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HB_WORD, (sptr_t)(eu_get_theme()->item.keywords0.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HB_WORD, eu_get_theme()->item.keywords0.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HB_WORD, (sptr_t)(eu_get_theme()->item.keywords0.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HB_WORD, (sptr_t)(eu_get_theme()->item.keywords0.bold));
        for (int style = SCE_HBA_START; style <= SCE_HBA_STRINGEOL; style++)
        {
            eu_sci_call(pnode, SCI_STYLESETBACK, style, (sptr_t)(eu_get_theme()->item.aspsection.color));
        }
        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HBA_START, (sptr_t)(eu_get_theme()->item.phpsection.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HBA_START, eu_get_theme()->item.phpsection.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HBA_START, (sptr_t)(eu_get_theme()->item.phpsection.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HBA_START, (sptr_t)(eu_get_theme()->item.phpsection.bold));
        on_doc_string_light(pnode, SCE_HBA_STRING, 0);
        on_doc_number_light(pnode, SCE_HBA_NUMBER, 0);
        on_doc_comment_light(pnode, SCE_HBA_COMMENTLINE, 0);

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HBA_WORD, (sptr_t)(eu_get_theme()->item.keywords0.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HBA_WORD, eu_get_theme()->item.keywords0.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HBA_WORD, (sptr_t)(eu_get_theme()->item.keywords0.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HBA_WORD, (sptr_t)(eu_get_theme()->item.keywords0.bold));
    }

    /* Python */
    if (pnode->doc_ptr->keywords4)
    {
        eu_sci_call(pnode, SCI_SETKEYWORDS, 3, (sptr_t)(pnode->doc_ptr->keywords4));
        for (int style = SCE_HP_START; style <= SCE_HP_IDENTIFIER; style++)
        {
            eu_sci_call(pnode, SCI_STYLESETBACK, style, (sptr_t)(eu_get_theme()->item.aspsection.color));
        }
        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HP_START, (sptr_t)(eu_get_theme()->item.phpsection.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HP_START, eu_get_theme()->item.phpsection.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HP_START, (sptr_t)(eu_get_theme()->item.phpsection.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HP_START, (sptr_t)(eu_get_theme()->item.phpsection.bold));
        on_doc_string_light(pnode, SCE_HP_STRING, 0);
        on_doc_char_light(pnode, SCE_HP_CHARACTER, 0);
        on_doc_number_light(pnode, SCE_HP_NUMBER, 0);
        on_doc_operator_light(pnode, SCE_HP_OPERATOR, 0);
        on_doc_comment_light(pnode, SCE_HP_COMMENTLINE, 0);
        on_doc_string_light(pnode, SCE_HP_TRIPLE, 0);
        on_doc_string_light(pnode, SCE_HP_TRIPLEDOUBLE, 0);

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HP_WORD, (sptr_t)(eu_get_theme()->item.keywords0.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HP_WORD, eu_get_theme()->item.keywords0.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HP_WORD, (sptr_t)(eu_get_theme()->item.keywords0.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HP_WORD, (sptr_t)(eu_get_theme()->item.keywords0.bold));
        for (int style = SCE_HPHP_COMPLEX_VARIABLE; style <= SCE_HPA_IDENTIFIER; style++)
        {
            eu_sci_call(pnode, SCI_STYLESETBACK, style, (sptr_t)(eu_get_theme()->item.aspsection.color));
        }
        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HPA_START, (sptr_t)(eu_get_theme()->item.phpsection.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HPA_START, eu_get_theme()->item.phpsection.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HPA_START, (sptr_t)(eu_get_theme()->item.phpsection.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HPA_START, (sptr_t)(eu_get_theme()->item.phpsection.bold));
        on_doc_string_light(pnode, SCE_HPA_STRING, 0);
        on_doc_char_light(pnode, SCE_HPA_CHARACTER, 0);
        on_doc_number_light(pnode, SCE_HPA_NUMBER, 0);
        on_doc_operator_light(pnode, SCE_HPA_OPERATOR, 0);
        on_doc_comment_light(pnode, SCE_HPA_COMMENTLINE, 0);
        on_doc_string_light(pnode, SCE_HPA_TRIPLE, 0);
        on_doc_string_light(pnode, SCE_HPA_TRIPLEDOUBLE, 0);

        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HPA_WORD, (sptr_t)(eu_get_theme()->item.keywords0.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HPA_WORD, eu_get_theme()->item.keywords0.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HPA_WORD, (sptr_t)(eu_get_theme()->item.keywords0.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HPA_WORD, (sptr_t)(eu_get_theme()->item.keywords0.bold));
    }

    /* PHP */
    if (pnode->doc_ptr->keywords5)
    {
        eu_sci_call(pnode, SCI_SETKEYWORDS, 4, (sptr_t)(pnode->doc_ptr->keywords5));
        on_doc_string_light(pnode, SCE_HPHP_HSTRING, 0);
        on_doc_number_light(pnode, SCE_HPHP_NUMBER, 0);
        on_doc_operator_light(pnode, SCE_HPHP_OPERATOR, 0);
        on_doc_comment_light(pnode, SCE_HPHP_COMMENTLINE, 0);
        on_doc_commentblock_light(pnode, SCE_HPHP_COMMENT, 0);
        on_doc_string_light(pnode, SCE_HPHP_SIMPLESTRING, 0);
        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_HPHP_WORD, (sptr_t)(eu_get_theme()->item.keywords0.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_HPHP_WORD, eu_get_theme()->item.keywords0.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_HPHP_WORD, (sptr_t)(eu_get_theme()->item.keywords0.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_HPHP_WORD, (sptr_t)(eu_get_theme()->item.keywords0.bold));
    }
    init_sc_fold(pnode);
    return 0;
}

int
on_doc_init_after_css(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_CSS, 0);
    if (pnode->doc_ptr->keywords0)
    {
        eu_sci_call(pnode, SCI_SETKEYWORDS, 0, (sptr_t)(pnode->doc_ptr->keywords0));
    }
    if (pnode->doc_ptr->keywords1)
    {
        eu_sci_call(pnode, SCI_SETKEYWORDS, 1, (sptr_t)(pnode->doc_ptr->keywords1));
    }
    
    on_doc_string_light(pnode, SCE_CSS_DOUBLESTRING, 0);
    on_doc_char_light(pnode, SCE_CSS_SINGLESTRING, 0);
    on_doc_operator_light(pnode, SCE_CSS_OPERATOR, 0);
    on_doc_commentblock_light(pnode, SCE_CSS_COMMENT, 0);
    on_doc_tags_light(pnode, SCE_CSS_TAG, 0);

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_CSS_CLASS, (sptr_t)(eu_get_theme()->item.unknowtags.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_CSS_CLASS, eu_get_theme()->item.unknowtags.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_CSS_CLASS, (sptr_t)(eu_get_theme()->item.unknowtags.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_CSS_CLASS, (sptr_t)(eu_get_theme()->item.unknowtags.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_CSS_PSEUDOCLASS, (sptr_t)(eu_get_theme()->item.attributes.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_CSS_PSEUDOCLASS, eu_get_theme()->item.attributes.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_CSS_PSEUDOCLASS, (sptr_t)(eu_get_theme()->item.attributes.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_CSS_PSEUDOCLASS, (sptr_t)(eu_get_theme()->item.attributes.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_CSS_IDENTIFIER, (sptr_t)(eu_get_theme()->item.attributes.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_CSS_IDENTIFIER, eu_get_theme()->item.attributes.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_CSS_IDENTIFIER, (sptr_t)(eu_get_theme()->item.attributes.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_CSS_IDENTIFIER, (sptr_t)(eu_get_theme()->item.attributes.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_CSS_UNKNOWN_IDENTIFIER, (sptr_t)(eu_get_theme()->item.unknowattributes.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_CSS_UNKNOWN_IDENTIFIER, eu_get_theme()->item.unknowattributes.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_CSS_UNKNOWN_IDENTIFIER, (sptr_t)(eu_get_theme()->item.unknowattributes.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_CSS_UNKNOWN_IDENTIFIER, (sptr_t)(eu_get_theme()->item.unknowattributes.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_CSS_VALUE, (sptr_t)(eu_get_theme()->item.attributes.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_CSS_VALUE, eu_get_theme()->item.attributes.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_CSS_VALUE, (sptr_t)(eu_get_theme()->item.attributes.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_CSS_VALUE, (sptr_t)(eu_get_theme()->item.attributes.bold));

    init_sc_fold(pnode);
    return 0;
}

int
on_doc_init_after_js(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_CPP, 0);
    if (pnode->doc_ptr->keywords0)
    {
        on_doc_keyword_light(pnode, SCE_C_WORD, 0, 0);
    }
    
    on_doc_string_light(pnode, SCE_C_STRING, 0);
    on_doc_char_light(pnode, SCE_C_CHARACTER, 0);
    on_doc_number_light(pnode, SCE_C_NUMBER, 0);
    on_doc_comment_light(pnode, SCE_C_COMMENTLINE, 0);
    on_doc_commentblock_light(pnode, SCE_C_COMMENT, 0);
    on_doc_commentdoc_light(pnode, SCE_C_COMMENTDOC, 0);
    on_doc_preprocessor_light(pnode, SCE_C_PREPROCESSOR, -1, 0);

    init_sc_fold(pnode);
    if (pnode->doc_ptr->fn_reload_symlist)
    {
        return pnode->doc_ptr->fn_reload_symlist(pnode);
    }
    return 0;
}

int
on_doc_init_after_xml(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_XML, 0);
    on_doc_tags_light(pnode, SCE_H_TAG, 0);

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_TAGUNKNOWN, (sptr_t)(eu_get_theme()->item.unknowtags.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_TAGUNKNOWN, eu_get_theme()->item.unknowtags.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_TAGUNKNOWN, (sptr_t)(eu_get_theme()->item.unknowtags.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_TAGUNKNOWN, (sptr_t)(eu_get_theme()->item.unknowtags.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_ATTRIBUTE, (sptr_t)(eu_get_theme()->item.attributes.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_ATTRIBUTE, eu_get_theme()->item.attributes.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_ATTRIBUTE, (sptr_t)(eu_get_theme()->item.attributes.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_ATTRIBUTE, (sptr_t)(eu_get_theme()->item.attributes.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_ATTRIBUTEUNKNOWN, (sptr_t)(eu_get_theme()->item.unknowattributes.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_ATTRIBUTEUNKNOWN, eu_get_theme()->item.unknowattributes.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_ATTRIBUTEUNKNOWN, (sptr_t)(eu_get_theme()->item.unknowattributes.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_ATTRIBUTEUNKNOWN, (sptr_t)(eu_get_theme()->item.unknowattributes.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_ENTITY, (sptr_t)(eu_get_theme()->item.entities.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_ENTITY, eu_get_theme()->item.entities.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_ENTITY, (sptr_t)(eu_get_theme()->item.entities.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_ENTITY, (sptr_t)(eu_get_theme()->item.entities.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_TAGEND, (sptr_t)(eu_get_theme()->item.tagends.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_TAGEND, eu_get_theme()->item.tagends.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_TAGEND, (sptr_t)(eu_get_theme()->item.tagends.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_TAGEND, (sptr_t)(eu_get_theme()->item.tagends.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_CDATA, (sptr_t)(eu_get_theme()->item.cdata.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_CDATA, eu_get_theme()->item.cdata.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_CDATA, (sptr_t)(eu_get_theme()->item.cdata.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_CDATA, (sptr_t)(eu_get_theme()->item.cdata.bold));

    on_doc_string_light(pnode, SCE_H_DOUBLESTRING, 0);
    on_doc_char_light(pnode, SCE_H_SINGLESTRING, 0);
    on_doc_number_light(pnode, SCE_H_NUMBER, 0);
    on_doc_commentblock_light(pnode, SCE_H_COMMENT, 0);

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_H_QUESTION, (sptr_t)(eu_get_theme()->item.phpsection.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_H_QUESTION, eu_get_theme()->item.phpsection.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_H_QUESTION, (sptr_t)(eu_get_theme()->item.phpsection.color));
    eu_sci_call(pnode, SCI_STYLESETBACK, SCE_H_QUESTION, (sptr_t)(eu_get_theme()->item.aspsection.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_H_QUESTION, (sptr_t)(eu_get_theme()->item.phpsection.bold));

    init_sc_fold(pnode);
    return 0;
}

int
on_doc_init_after_json(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_JSON, 0);
    on_doc_keyword_light(pnode, SCE_JSON_KEYWORD, 0, 0);

    on_doc_string_light(pnode, SCE_JSON_STRING, 0);
    on_doc_number_light(pnode, SCE_JSON_NUMBER, 0);
    on_doc_operator_light(pnode, SCE_JSON_OPERATOR, 0);
    on_doc_comment_light(pnode, SCE_JSON_LINECOMMENT, 0);
    on_doc_commentdoc_light(pnode, SCE_JSON_BLOCKCOMMENT, 0);

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_JSON_PROPERTYNAME, (sptr_t)(eu_get_theme()->item.attributes.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_JSON_PROPERTYNAME, eu_get_theme()->item.attributes.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_JSON_PROPERTYNAME, (sptr_t)(eu_get_theme()->item.attributes.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_JSON_PROPERTYNAME, (sptr_t)(eu_get_theme()->item.attributes.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_JSON_LDKEYWORD, (sptr_t)(eu_get_theme()->item.keywords0.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_JSON_LDKEYWORD, eu_get_theme()->item.keywords0.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_JSON_LDKEYWORD, (sptr_t)(eu_get_theme()->item.keywords0.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_JSON_LDKEYWORD, (sptr_t)(eu_get_theme()->item.keywords0.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_YAML_ERROR, (sptr_t)(eu_get_theme()->item.phpsection.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_YAML_ERROR, eu_get_theme()->item.phpsection.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_YAML_ERROR, (sptr_t)(eu_get_theme()->item.phpsection.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_YAML_ERROR, (sptr_t)(eu_get_theme()->item.phpsection.bold));

    init_sc_fold(pnode);
    init_systree_theme(pnode);
    if (pnode->doc_ptr->fn_reload_symtree)
    {
        return pnode->doc_ptr->fn_reload_symtree(pnode);
    }
    return 0;
}

int
on_doc_init_after_yaml(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_YAML, 0);
    on_doc_keyword_light(pnode, SCE_YAML_KEYWORD, 0, 0);
    on_doc_string_light(pnode, SCE_YAML_TEXT, 0);
    on_doc_number_light(pnode, SCE_YAML_NUMBER, 0);
    on_doc_operator_light(pnode, SCE_YAML_OPERATOR, 0);   
    on_doc_commentblock_light(pnode, SCE_YAML_COMMENT, 0); 

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_YAML_IDENTIFIER, (sptr_t)(eu_get_theme()->item.attributes.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_YAML_IDENTIFIER, eu_get_theme()->item.attributes.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_YAML_IDENTIFIER, (sptr_t)(eu_get_theme()->item.attributes.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_YAML_IDENTIFIER, (sptr_t)(eu_get_theme()->item.attributes.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_YAML_ERROR, (sptr_t)(eu_get_theme()->item.phpsection.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_YAML_ERROR, eu_get_theme()->item.phpsection.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_YAML_ERROR, (sptr_t)(eu_get_theme()->item.phpsection.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_YAML_ERROR, (sptr_t)(eu_get_theme()->item.phpsection.bold));

    init_sc_fold(pnode);
    return 0;
}

int
on_doc_init_after_makefile(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_MAKEFILE, 0);
    on_doc_comment_light(pnode, SCE_MAKE_COMMENT, 0);
    on_doc_operator_light(pnode, SCE_MAKE_OPERATOR, 0);
    on_doc_preprocessor_light(pnode, SCE_MAKE_PREPROCESSOR, -1, 0);
    on_doc_tags_light(pnode, SCE_MAKE_TARGET, 0);
    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_MAKE_IDENTIFIER, (sptr_t)(eu_get_theme()->item.attributes.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_MAKE_IDENTIFIER, eu_get_theme()->item.attributes.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_MAKE_IDENTIFIER, (sptr_t)(eu_get_theme()->item.attributes.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_MAKE_IDENTIFIER, (sptr_t)(eu_get_theme()->item.attributes.bold));

    // 折叠
    init_sc_fold(pnode);
    return 0;
}

int
on_doc_init_after_cmake(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_CMAKE, 0);
    on_doc_keyword_light(pnode, SCE_CMAKE_COMMANDS, 0, 0);
    on_doc_function_light(pnode, SCE_CMAKE_PARAMETERS, 1, 0);
    
    on_doc_string_light(pnode, SCE_CMAKE_STRINGDQ, 0);
    on_doc_string_light(pnode, SCE_CMAKE_STRINGLQ, 0);
    on_doc_string_light(pnode, SCE_CMAKE_STRINGRQ, 0);
    on_doc_number_light(pnode, SCE_CMAKE_NUMBER, 0);
    on_doc_commentblock_light(pnode, SCE_CMAKE_COMMENT, 0);

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_CMAKE_VARIABLE, (sptr_t)(eu_get_theme()->item.attributes.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_CMAKE_VARIABLE, eu_get_theme()->item.attributes.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_CMAKE_VARIABLE, (sptr_t)(eu_get_theme()->item.attributes.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_CMAKE_VARIABLE, (sptr_t)(eu_get_theme()->item.attributes.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_CMAKE_IFDEFINEDEF, (sptr_t)(eu_get_theme()->item.tagends.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_CMAKE_IFDEFINEDEF, eu_get_theme()->item.tagends.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_CMAKE_IFDEFINEDEF, (sptr_t)(eu_get_theme()->item.tagends.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_CMAKE_IFDEFINEDEF, (sptr_t)(eu_get_theme()->item.tagends.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_CMAKE_MACRODEF, (sptr_t)(eu_get_theme()->item.tagends.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_CMAKE_MACRODEF, eu_get_theme()->item.tagends.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_CMAKE_MACRODEF, (sptr_t)(eu_get_theme()->item.tagends.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_CMAKE_MACRODEF, (sptr_t)(eu_get_theme()->item.tagends.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_CMAKE_FOREACHDEF, (sptr_t)(eu_get_theme()->item.tagends.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_CMAKE_FOREACHDEF, eu_get_theme()->item.tagends.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_CMAKE_FOREACHDEF, (sptr_t)(eu_get_theme()->item.tagends.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_CMAKE_FOREACHDEF, (sptr_t)(eu_get_theme()->item.tagends.bold));

    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_CMAKE_STRINGVAR, (sptr_t)(eu_get_theme()->item.unknowattributes.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_CMAKE_STRINGVAR, eu_get_theme()->item.unknowattributes.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_CMAKE_STRINGVAR, (sptr_t)(eu_get_theme()->item.unknowattributes.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_CMAKE_STRINGVAR, (sptr_t)(eu_get_theme()->item.unknowattributes.bold));

    init_sc_fold(pnode);
    return 0;
}

int
on_doc_init_after_markdown(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_MARKDOWN, 0);
    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_MARKDOWN_STRONG1, (sptr_t)(eu_get_theme()->item.tags.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_MARKDOWN_STRONG1, eu_get_theme()->item.tags.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_MARKDOWN_STRONG1, (sptr_t)(eu_get_theme()->item.tags.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_MARKDOWN_STRONG1, true);
    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_MARKDOWN_STRONG2, (sptr_t)(eu_get_theme()->item.tags.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_MARKDOWN_STRONG2, eu_get_theme()->item.tags.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_MARKDOWN_STRONG2, (sptr_t)(eu_get_theme()->item.tags.color));
    
    for (int i = SCE_MARKDOWN_EM1; i < SCE_MARKDOWN_CODE; ++i)
    {
        on_doc_tags_light(pnode, i, 0);       
    }
    eu_sci_call(pnode, SCI_STYLESETITALIC, SCE_MARKDOWN_EM1, true);
    eu_sci_call(pnode, SCI_STYLESETITALIC, SCE_MARKDOWN_EM2, true);
    eu_sci_call(pnode, SCI_STYLESETWEIGHT, SCE_MARKDOWN_STRIKEOUT, 1);
    eu_sci_call(pnode, SCI_STYLESETWEIGHT, SCE_MARKDOWN_HRULE, 999);
    eu_sci_call(pnode, SCI_STYLESETWEIGHT, SCE_MARKDOWN_LINK, 99);
    
    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_MARKDOWN_CODE, (sptr_t)(eu_get_theme()->item.string.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_MARKDOWN_CODE, eu_get_theme()->item.string.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_MARKDOWN_CODE, (sptr_t)(eu_get_theme()->item.string.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_MARKDOWN_CODE, (sptr_t)(eu_get_theme()->item.string.bold));
    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_MARKDOWN_CODE2, (sptr_t)(eu_get_theme()->item.cdata.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_MARKDOWN_CODE2, eu_get_theme()->item.cdata.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_MARKDOWN_CODE2, (sptr_t)(eu_get_theme()->item.cdata.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_MARKDOWN_CODE2, (sptr_t)(eu_get_theme()->item.cdata.bold));
    eu_sci_call(pnode, SCI_STYLESETFONT, SCE_MARKDOWN_CODEBK, (sptr_t)(eu_get_theme()->item.cdata.font));
    eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_MARKDOWN_CODEBK, eu_get_theme()->item.cdata.fontsize);
    eu_sci_call(pnode, SCI_STYLESETFORE, SCE_MARKDOWN_CODEBK, (sptr_t)(eu_get_theme()->item.cdata.color));
    eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_MARKDOWN_CODEBK, (sptr_t)(eu_get_theme()->item.cdata.bold));
    
    init_sc_fold(pnode);
    return 0;
}

int
on_doc_init_after_log(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_STTXT, 0);
    if (pnode->doc_ptr->keywords0)
    {
        on_doc_keyword_light(pnode, SCE_STTXT_KEYWORD, 0, 0);
    }
    if (pnode->doc_ptr->keywords1)
    {
        eu_sci_call(pnode, SCI_SETKEYWORDS, 1, (sptr_t)(pnode->doc_ptr->keywords1));
        eu_sci_call(pnode, SCI_STYLESETFONT, SCE_STTXT_TYPE, (sptr_t)(eu_get_theme()->item.unknowtags.font));
        eu_sci_call(pnode, SCI_STYLESETSIZE, SCE_STTXT_TYPE, eu_get_theme()->item.unknowtags.fontsize);
        eu_sci_call(pnode, SCI_STYLESETFORE, SCE_STTXT_TYPE, (sptr_t)(eu_get_theme()->item.unknowtags.color));
        eu_sci_call(pnode, SCI_STYLESETBOLD, SCE_STTXT_TYPE, (sptr_t)(eu_get_theme()->item.unknowtags.bold));
    }   
    on_doc_number_light(pnode, SCE_STTXT_DATETIME, 0);
    on_doc_operator_light(pnode, SCE_STTXT_OPERATOR, 0xff);
    on_doc_comment_light(pnode, SCE_STTXT_COMMENT, 0);
    on_doc_commentblock_light(pnode, SCE_STTXT_COMMENTLINE, 0);
    return 0;
}

int
on_doc_init_after_properties(eu_tabpage *pnode)
{
    eu_sci_call(pnode, SCI_SETLEXER, SCLEX_PROPERTIES, 0);
    on_doc_default_light(pnode, SCE_PROPS_DEFAULT, 0);
    on_doc_keyword_light(pnode, SCE_PROPS_KEY, 0, 0);
    on_doc_commentblock_light(pnode, SCE_PROPS_COMMENT, 0);
    on_doc_string_light(pnode, SCE_PROPS_SECTION, 0);
    on_doc_operator_light(pnode, SCE_PROPS_ASSIGNMENT, 0);
    on_doc_preprocessor_light(pnode, SCE_PROPS_DEFVAL, -1, 0);
    return 0;
}

static bool
is_inavailed_char(const uint8_t ch)
{
    if (ch > 0x7f)
    {
        return false;
    }
    return isspace(ch);
}

static int
add_autoclose_char(eu_tabpage *pnode, SCNotification *lpnotify)
{
    sptr_t current_pos = eu_sci_call(pnode, SCI_GETCURRENTPOS, 0, 0);
    int current_char = (int) eu_sci_call(pnode, SCI_GETCHARAT, current_pos, 0);
    if (current_char > 0 && eu_get_config()->auto_close_chars && is_inavailed_char(current_char))
    {   /* 自动补全关闭符号 */
        switch (lpnotify->ch)
        {
            case '(':
                eu_sci_call(pnode, SCI_REPLACESEL, 0, (sptr_t) ")");
                eu_sci_call(pnode, SCI_GOTOPOS, current_pos, 0);
                break;
            case '[':
                eu_sci_call(pnode, SCI_REPLACESEL, 0, (sptr_t) "]");
                eu_sci_call(pnode, SCI_GOTOPOS, current_pos, 0);
                break;
            case '{':
                eu_sci_call(pnode, SCI_REPLACESEL, 0, (sptr_t) "}");
                eu_sci_call(pnode, SCI_GOTOPOS, current_pos, 0);
                break;
            case '\'':
            {
                int pre_pre_character = 0;
                if (current_pos >= 2)
                {
                    pre_pre_character = (int) eu_sci_call(pnode, SCI_GETCHARAT, current_pos - 2, 0);
                }
                if (!(pre_pre_character == '\'' || current_char == '\''))
                {
                    eu_sci_call(pnode, SCI_REPLACESEL, 0, (sptr_t) "'");
                    eu_sci_call(pnode, SCI_GOTOPOS, current_pos, 0);
                }
                break;
            }
            case '"':
            {
                int pre_pre_character = 0;
                if (current_pos >= 2)
                {
                    pre_pre_character = (int) eu_sci_call(pnode, SCI_GETCHARAT, current_pos - 2, 0);
                }
                if (!(pre_pre_character == '"' || current_char == '"'))
                {
                    eu_sci_call(pnode, SCI_REPLACESEL, 0, (sptr_t) "\"");
                    eu_sci_call(pnode, SCI_GOTOPOS, current_pos, 0);
                }
                break;
            }
            default:
                break;
        }
    }
    return 0;
}

static int
add_autoclose_bracket(eu_tabpage *pnode, SCNotification *lpnotify)
{
    /* web脚本自动补全符号 */
    if (lpnotify->ch == '<' && eu_get_config()->auto_close_chars)
    {
        sptr_t current_pos = eu_sci_call(pnode, SCI_GETCURRENTPOS, 0, 0);
        int current_char = (int) eu_sci_call(pnode, SCI_GETCHARAT, current_pos, 0);
        if (current_char > 0 && is_inavailed_char(current_char))
        {
            eu_sci_call(pnode, SCI_REPLACESEL, 0, (sptr_t) ">");
            eu_sci_call(pnode, SCI_GOTOPOS, current_pos, 0);
        }
    }
    return 0;
}

int
on_doc_identation(eu_tabpage *pnode, SCNotification *lpnotify)
{
    /* 自动缩进 */
    if (lpnotify->ch == '\n' && eu_get_config()->m_ident)
    {
        sptr_t current_pos = eu_sci_call(pnode, SCI_GETCURRENTPOS, 0, 0);
        sptr_t current_line = eu_sci_call(pnode, SCI_LINEFROMPOSITION, current_pos, 0);
        sptr_t pre_lpos = eu_sci_call(pnode, SCI_POSITIONFROMLINE, current_line - 1, 0);
        char pre_indent[64] = {0};
        int indent_len = 0;
        sptr_t n_pos = pre_lpos;
        bool collect_indent = true;
        int line_tail_char = 0;
        int ch = (int) eu_sci_call(pnode, SCI_GETCHARAT, n_pos, 0);
        for (; ch > 0 && ch != '\n'; n_pos++, ch = (int) eu_sci_call(pnode, SCI_GETCHARAT, n_pos, 0))
        {
            if (collect_indent)
            {
                if (strchr(" \t", ch) && indent_len <= sizeof(pre_indent) - 1)
                {
                    pre_indent[indent_len] = ch;
                    indent_len++;
                    continue;
                }
                else
                {
                    collect_indent = false;
                }
            }
            if (!strchr(" \t\r\n", ch))
            {
                line_tail_char = ch;
            }
        }
        eu_sci_call(pnode, SCI_REPLACESEL, 0, (sptr_t) pre_indent);
        if (line_tail_char && strchr("([{<", line_tail_char))
        {
            eu_sci_call(pnode, SCI_REPLACESEL, 0, (sptr_t) "\t");
            current_pos = eu_sci_call(pnode, SCI_GETCURRENTPOS, 0, 0);
            if (strchr(")]}>", (int) eu_sci_call(pnode, SCI_GETCHARAT, current_pos, 0)))
            {
                eu_sci_call(pnode, SCI_REPLACESEL, 0, (sptr_t) on_encoding_get_eol(pnode));
                eu_sci_call(pnode, SCI_REPLACESEL, 0, (sptr_t) pre_indent);
                eu_sci_call(pnode, SCI_GOTOPOS, current_pos, 0);
            }
        }
    }
    return 0;
}

static int
on_doc_match_parenthesis(eu_tabpage *pnode, SCNotification *lpnotify)
{
    char word_buffer[ACNAME_LEN+1] = {0};
    if (!eu_get_config()->m_ctshow)
    {
        return 0;
    }
    if (lpnotify->ch == '(')
    {   /* 类c风格的函数原型提示 */
        if (pnode->doc_ptr && !RB_EMPTY_ROOT(&pnode->doc_ptr->ctshow_tree))
        {
            int start_pos = 0;
            int end_pos = 0;
            int current_pos = (int) eu_sci_call(pnode, SCI_GETCURRENTPOS, 0, 0);
            int ch = (int) eu_sci_call(pnode, SCI_GETCHARAT, current_pos - 2 , 0);
            if (ch == 0x20)
            {   // 函数名称后面跟了一个空格
                start_pos = (int) eu_sci_call(pnode, SCI_WORDSTARTPOSITION, current_pos - 2, true);
                end_pos = (int) eu_sci_call(pnode, SCI_WORDENDPOSITION, current_pos - 2, true);
            }
            else
            {
                start_pos = (int) eu_sci_call(pnode, SCI_WORDSTARTPOSITION, current_pos - 1, true);
                end_pos = (int) eu_sci_call(pnode, SCI_WORDENDPOSITION, current_pos - 1, true);
            }
            if (end_pos - start_pos >= ACNAME_LEN)
            {
                end_pos = start_pos + ACNAME_LEN;
            }
            Sci_TextRange tr = {{start_pos, end_pos}, word_buffer};
            eu_sci_call(pnode, SCI_GETTEXTRANGE, 0, (sptr_t) &tr);
            if (*word_buffer)
            {
                const char *psz_location = eu_query_calltip_tree(&pnode->doc_ptr->ctshow_tree, word_buffer);
                if (psz_location)
                {
                    eu_sci_call(pnode, SCI_CALLTIPSHOW, current_pos, (sptr_t) psz_location);
                }
                else if (eu_sci_call(pnode, SCI_AUTOCACTIVE, 0, 0))
                {
                    eu_sci_call(pnode, SCI_CALLTIPCANCEL, 0, 0);
                }
            }
        }
    }
    else if (lpnotify->ch == ',')
    {   /* 函数原型提示 */
        if (pnode->doc_ptr && !RB_EMPTY_ROOT(&pnode->doc_ptr->ctshow_tree))
        {
            int ch;
            int current_pos = (int) eu_sci_call(pnode, SCI_GETCURRENTPOS, 0, 0) - 1;
            int n_pos = current_pos;
            for (ch = (int) eu_sci_call(pnode, SCI_GETCHARAT, n_pos, 0); 
                 n_pos >= 0 && ch != '('; 
                 n_pos--, ch = (int) eu_sci_call(pnode, SCI_GETCHARAT, n_pos, 0))
            {
                ;
            }
            if (ch == '(' && n_pos >= 0)
            {
                int start_pos = (int) eu_sci_call(pnode, SCI_WORDSTARTPOSITION, n_pos - 1, true);
                int end_pos = (int) eu_sci_call(pnode, SCI_WORDENDPOSITION, n_pos - 1, true);
                if (end_pos - start_pos >= ACNAME_LEN) 
                {
                    end_pos = start_pos + ACNAME_LEN;
                }
                Sci_TextRange tr = {{start_pos, end_pos}, word_buffer};
                eu_sci_call(pnode, SCI_GETTEXTRANGE, 0, (sptr_t) &tr);
                if (*word_buffer)
                {
                    const char *psz_location = eu_query_calltip_tree(&pnode->doc_ptr->ctshow_tree, word_buffer);
                    if (psz_location)
                    {
                        eu_sci_call(pnode, SCI_CALLTIPSHOW, current_pos, (sptr_t) psz_location);
                    }
                    else if (eu_sci_call(pnode, SCI_AUTOCACTIVE, 0, 0))
                    {
                        eu_sci_call(pnode, SCI_CALLTIPCANCEL, 0, 0);
                    }
                }
            }
            else if (eu_sci_call(pnode, SCI_AUTOCACTIVE, 0, 0))
            {
                eu_sci_call(pnode, SCI_CALLTIPCANCEL, 0, 0);
            }
        }
    }
    return 0;
}

static int
add_ctinput_char(eu_tabpage *pnode, SCNotification *lpnotify, char ch_from, bool upper_case)
{
    char word_buffer[ACNAME_LEN+1];
    if (lpnotify->ch == ' ' && eu_get_config()->m_ctshow)
    {   /* 函数原型提示 */
        if (pnode->doc_ptr && !RB_EMPTY_ROOT(&pnode->doc_ptr->ctshow_tree))
        {
            int current_pos = (int) eu_sci_call(pnode, SCI_GETCURRENTPOS, 0, 0);
            int start_pos = current_pos;
            int end_pos;
            int ch;
            while (1)
            {
                start_pos--;
                if (start_pos < 0)
                {
                    start_pos = 0;
                    break;
                }
                ch = (int) eu_sci_call(pnode, SCI_GETCHARAT, start_pos, 0);
                if (ch == ch_from)
                {
                    start_pos++;
                    break;
                }
            }
            while (1)
            {
                ch = (int) eu_sci_call(pnode, SCI_GETCHARAT, start_pos, 0);
                if (!strchr(" \t\r\n\f", ch))
                {
                    break;
                }
                start_pos++;
                if (start_pos >= current_pos)
                {
                    break;
                }
            }
            end_pos = start_pos + 1;
            while (1)
            {
                ch = (int) eu_sci_call(pnode, SCI_GETCHARAT, end_pos, 0);
                if (strchr(" \t\r\n\f", ch))
                {
                    break;
                }
                end_pos++;
                if (end_pos >= current_pos)
                {
                    break;
                }
            }
            if (end_pos - start_pos >= ACNAME_LEN)
            {
                end_pos = start_pos + ACNAME_LEN;
            }            
            Sci_TextRange tr = {{start_pos, end_pos}, word_buffer};
            eu_sci_call(pnode, SCI_GETTEXTRANGE, 0, (sptr_t) &tr);
            if (upper_case)
            {
                util_upper_string(word_buffer);
            }
            const char *psz_location = eu_query_calltip_tree(&pnode->doc_ptr->ctshow_tree, word_buffer);
            if (psz_location)
            {
                eu_sci_call(pnode, SCI_CALLTIPSHOW, current_pos, (sptr_t) psz_location);
            }
            else
            {
                if (eu_sci_call(pnode, SCI_AUTOCACTIVE, 0, 0)) eu_sci_call(pnode, SCI_CALLTIPCANCEL, 0, 0);
            }
        }
    }
    else if (lpnotify->ch == '\n' && eu_get_config()->m_ctshow)
    {
        if (eu_sci_call(pnode, SCI_AUTOCACTIVE, 0, 0))
        {
            eu_sci_call(pnode, SCI_CALLTIPCANCEL, 0, 0);
        }
    }
    return 0;
}

static int
add_acshow_char(eu_tabpage *pnode, SCNotification *lpnotify)
{
    char word_buffer[ACNAME_LEN+1] = {0};
    if (-1 <= lpnotify->ch && lpnotify->ch <= 255 && isprint(lpnotify->ch) && lpnotify->ch != ' ' && lpnotify->ch != '\t' && eu_get_config()->m_acshow)
    {
        /* 自动完成提示 */
        if (pnode->doc_ptr && !RB_EMPTY_ROOT(&(pnode->doc_ptr->acshow_tree)))
        {
            int current_pos = (int) eu_sci_call(pnode, SCI_GETCURRENTPOS, 0, 0);
            // SCI_GETCHARAT
            int start_pos = (int) eu_sci_call(pnode, SCI_WORDSTARTPOSITION, current_pos - 1, true);
            int ndo = (int) eu_sci_call(pnode, SCI_GETCHARAT, current_pos - 2, 0);
            int end_pos = (int) eu_sci_call(pnode, SCI_WORDENDPOSITION, current_pos - 1, true);
            if (end_pos - start_pos >= ACNAME_LEN)
            {
                end_pos = start_pos + ACNAME_LEN;
            }
            if (ndo == '#')
            {
                --start_pos;
            }
            else if (ndo == '.')
            {
                return 0;
            }
            if (end_pos - start_pos >= eu_get_config()->acshow_chars)
            {
                Sci_TextRange tr = {{start_pos, end_pos}, word_buffer};
                eu_sci_call(pnode, SCI_GETTEXTRANGE, 0, (sptr_t) &tr);
                if (word_buffer[0])
                {
                    const char *key = eu_find_completed_tree(&pnode->doc_ptr->acshow_tree, word_buffer);
                    if (key)
                    {
                        eu_sci_call(pnode, SCI_AUTOCSHOW, current_pos - start_pos, (sptr_t) key);
                        free((void *) key);
                    }
                }
            }
        }
    }
    return 0;
}

static int
add_acshow_html(eu_tabpage *pnode, SCNotification *lpnotify)
{
    int current_pos = 0;
    int nret = 0;
    int n_pos = 0;
    int ch_pre = 0;
    int ch = 0;
    char word_buffer[ACNAME_LEN+1];
    if (!(pnode && pnode->doc_ptr && lpnotify))
    {
        return 1;
    }
    if (-1 <= lpnotify->ch && lpnotify->ch <= 255 && isprint(lpnotify->ch) && lpnotify->ch != '\t' && eu_get_config()->m_acshow)
    {
        /* 自动完成提示 */
        if (lpnotify->ch == '<')
        {
            const char *key = eu_find_completed_tree(&pnode->doc_ptr->acshow_tree, ANY_WORD);
            if (!key)
            {
                return 1;
            }
            eu_sci_call(pnode, SCI_AUTOCSHOW, 0, (sptr_t) key);
            free((void *) key);
        }
        else if (lpnotify->ch == ' ')
        {
            current_pos = (int) eu_sci_call(pnode, SCI_GETCURRENTPOS, 0, 0);
            for (n_pos = current_pos - 1; n_pos >= 0; n_pos--)
            {
                ch_pre = ch;
                ch = (int) eu_sci_call(pnode, SCI_GETCHARAT, n_pos, 0);
                if (ch == '<' || ch == '>') break;
            }
            if (n_pos >= 0 && ch == '<' && ch_pre != '?' && ch_pre != '%')
            {
                const char *key = eu_find_completed_tree(&pnode->doc_ptr->acshow_tree, ANY_WORD);
                if (!key)
                {
                    return 1;
                }
                eu_sci_call(pnode, SCI_AUTOCSHOW, 0, (sptr_t) key);
                free((void *) key);
            }
        }
        else if (isalpha(lpnotify->ch))
        {
            current_pos = (int) eu_sci_call(pnode, SCI_GETCURRENTPOS, 0, 0);
            int start_pos = (int) eu_sci_call(pnode, SCI_WORDSTARTPOSITION, current_pos - 1, true);
            int end_pos = (int) eu_sci_call(pnode, SCI_WORDENDPOSITION, current_pos - 1, true);
            if (end_pos - start_pos >= ACNAME_LEN)
            {
                end_pos = start_pos + ACNAME_LEN;
            }
            if (start_pos > 0 && end_pos - start_pos >= eu_get_config()->acshow_chars)
            {
                bool is_attr = false;
                for (ch = 0, n_pos = current_pos; n_pos >= 0; n_pos--)
                {
                    ch_pre = ch;
                    ch = (int) eu_sci_call(pnode, SCI_GETCHARAT, n_pos, 0);
                    if (ch == ' ')
                    {
                        is_attr = true;
                    }
                    else if (ch == '<' || ch == '>')
                    {
                        break;
                    }
                }
                if (ch > 0 && (is_attr || (ch == '<' && ch_pre != '?' && ch_pre != '%')))
                {
                    memset(word_buffer, 0, sizeof(word_buffer));
                    Sci_TextRange tr = {{start_pos, end_pos}, word_buffer};
                    eu_sci_call(pnode, SCI_GETTEXTRANGE, 0, (sptr_t) &tr);
                    if (word_buffer[0])
                    {
                        const char *key = eu_find_completed_tree(&pnode->doc_ptr->acshow_tree, word_buffer);
                        if (key)
                        {
                            eu_sci_call(pnode, SCI_AUTOCSHOW, current_pos - start_pos, (sptr_t) key);
                            free((void *) key);
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int
on_doc_cpp_like(eu_tabpage *pnode, SCNotification *lpnotify)
{
    if (pnode)
    {
        on_doc_identation(pnode, lpnotify);
        add_autoclose_char(pnode, lpnotify);
        add_acshow_char(pnode, lpnotify);
        on_doc_match_parenthesis(pnode, lpnotify);
    }
    return 0;
}

int
on_doc_sql_like(eu_tabpage *pnode, SCNotification *lpnotify)
{
    if (pnode)
    {
        on_doc_identation(pnode, lpnotify);
        add_autoclose_char(pnode, lpnotify);
        add_acshow_char(pnode, lpnotify);
        add_ctinput_char(pnode, lpnotify, ';', true);
    }
    return 0;
}

int
on_doc_redis_like(eu_tabpage *pnode, SCNotification *lpnotify)
{
    if (pnode)
    {
        on_doc_identation(pnode, lpnotify);
        add_autoclose_char(pnode, lpnotify);
        add_acshow_char(pnode, lpnotify);
        add_ctinput_char(pnode, lpnotify, '\n', true);
    }
    return 0;
}

int
on_doc_html_like(eu_tabpage *pnode, SCNotification *lpnotify)
{
    if (pnode)
    {
        on_doc_identation(pnode, lpnotify);
        add_autoclose_char(pnode, lpnotify);
        add_autoclose_bracket(pnode, lpnotify);
        add_acshow_html(pnode, lpnotify);
    }
    return 0;
}

int
on_doc_xml_like(eu_tabpage *pnode, SCNotification *lpnotify)
{
    if (pnode)
    {
        on_doc_identation(pnode, lpnotify);
        add_autoclose_char(pnode, lpnotify);
        add_autoclose_bracket(pnode, lpnotify);
    }
    return 0;
}

int
on_doc_css_like(eu_tabpage *pnode, SCNotification *lpnotify)
{
    if (pnode)
    {
        on_doc_identation(pnode, lpnotify);
        add_autoclose_char(pnode, lpnotify);
        add_acshow_char(pnode, lpnotify);
    }
    return 0;
}

int
on_doc_json_like(eu_tabpage *pnode, SCNotification *lpnotify)
{
    if (pnode)
    {
        on_doc_identation(pnode, lpnotify);
        add_autoclose_char(pnode, lpnotify);
    }
    return 0;
}

int
on_doc_yaml_like(eu_tabpage *pnode, SCNotification *lpnotify)
{
    if (pnode)
    {
        on_doc_identation(pnode, lpnotify);
    }
    return 0;
}

int
on_doc_makefile_like(eu_tabpage *pnode, SCNotification *lpnotify)
{
    if (pnode)
    {
        on_doc_identation(pnode, lpnotify);
        add_autoclose_char(pnode, lpnotify);
    }
    return 0;
}

int
on_doc_cmake_like(eu_tabpage *pnode, SCNotification *lpnotify)
{
    if (pnode)
    {
        on_doc_identation(pnode, lpnotify);
        add_autoclose_char(pnode, lpnotify);
        add_acshow_char(pnode, lpnotify);
    }
    return 0;
}

int
on_doc_markdown_like(eu_tabpage *pnode, SCNotification *lpnotify)
{
    if (pnode)
    {
        on_doc_identation(pnode, lpnotify);
        add_autoclose_char(pnode, lpnotify);
    }
    return 0;
}

int
on_doc_keydown_light(eu_tabpage *pnode)
{
    if (!pnode)
    {
        return 0;
    }
    sptr_t current_pos = eu_sci_call(pnode, SCI_GETCURRENTPOS, 0, 0);
    int current_char = (int) eu_sci_call(pnode, SCI_GETCHARAT, current_pos, 0);
    int ch = (int) eu_sci_call(pnode, SCI_GETCHARAT, current_pos-1, 0);
    bool current_match = current_char > 0 && strchr("()[]{}<>", current_char);
    bool pre_match = ch > 0 && strchr("()[]{}<>", ch);
    if (current_match || pre_match)
    {
        sptr_t match_pos = -1;
        if (current_match)
        {
            match_pos = eu_sci_call(pnode, SCI_BRACEMATCH, current_pos, 0);
        }
        else if (pre_match)
        {
            match_pos = eu_sci_call(pnode, SCI_BRACEMATCH, current_pos-1, 0);
            --current_pos;
        }
        if (match_pos != -1)
        {
            sptr_t m_style = eu_sci_call(pnode, SCI_GETSTYLEAT, current_pos, 0);
            sptr_t m_forecolor = eu_sci_call(pnode, SCI_STYLEGETFORE, m_style, 0);
            sptr_t m_backcolor = eu_sci_call(pnode, SCI_STYLEGETBACK, m_style, 0);
            eu_sci_call(pnode, SCI_STYLESETFORE, STYLE_BRACELIGHT, m_backcolor);
            eu_sci_call(pnode, SCI_STYLESETBACK, STYLE_BRACELIGHT, m_forecolor);
            eu_sci_call(pnode, SCI_STYLESETBOLD, STYLE_BRACELIGHT, true);
            eu_sci_call(pnode, SCI_BRACEHIGHLIGHT, current_pos, match_pos);
        }
        else
        {
            sptr_t m_style = eu_sci_call(pnode, SCI_GETSTYLEAT, current_pos, 0);
            sptr_t m_forecolor = eu_sci_call(pnode, SCI_STYLEGETFORE, m_style, 0);
            sptr_t m_backcolor = eu_sci_call(pnode, SCI_STYLEGETBACK, m_style, 0);
            eu_sci_call(pnode, SCI_STYLESETFORE, STYLE_BRACEBAD, m_forecolor);
            eu_sci_call(pnode, SCI_STYLESETBACK, STYLE_BRACEBAD, m_backcolor);
            eu_sci_call(pnode, SCI_STYLESETITALIC, STYLE_BRACEBAD, true);
            eu_sci_call(pnode, SCI_STYLESETUNDERLINE, STYLE_BRACEBAD, true);
            eu_sci_call(pnode, SCI_BRACEBADLIGHT, current_pos, 0);
        }
    }
    else
    {
        eu_sci_call(pnode, SCI_BRACEBADLIGHT, INVALID_POSITION, INVALID_POSITION);
    }
    return 0;
}

int
on_doc_keydown_jmp(eu_tabpage *pnode, WPARAM wParam, LPARAM lParam)
{
    if (wParam == VK_F12)
    {
        return on_symlist_jump_word(pnode);
    }
    return 0;
}

static void
select_sql_stat(eu_tabpage *pnode)
{
    sptr_t current_pos;
    sptr_t max_pos;
    sptr_t start_pos;
    sptr_t end_pos;
    if (!pnode)
    {
        return;
    }
    current_pos = eu_sci_call(pnode, SCI_GETCURRENTPOS, 0, 0);
    max_pos = eu_sci_call(pnode, SCI_GETTEXTLENGTH, 0, 0);
    for (start_pos = current_pos; start_pos >= 0; start_pos--)
    {
        if (!strchr("\n", (int) eu_sci_call(pnode, SCI_GETCHARAT, start_pos, 0)))
        {
            break;
        }
    }
    if ((int) eu_sci_call(pnode, SCI_GETCHARAT, start_pos, 0) == ';')
    {
        start_pos--;
    }
    end_pos = start_pos;
    for (; start_pos >= 0; start_pos--)
    {
        if ((int) eu_sci_call(pnode, SCI_GETCHARAT, start_pos, 0) == ';')
        {
            break;
        }
    }
    if (start_pos < 0)
    {
        start_pos = 0;
    }
    else
    {
        start_pos++;
    }
    for (; end_pos < max_pos; end_pos++)
    {
        if ((int) eu_sci_call(pnode, SCI_GETCHARAT, start_pos, 0) == ';')
        {
            break;
        }
    }
    if (end_pos >= max_pos)
    {
        end_pos = max_pos - 1;
    }
    eu_sci_call(pnode, SCI_SETSEL, start_pos, end_pos);
}

int
on_doc_keydown_sql(eu_tabpage *pnode, WPARAM wParam, LPARAM lParam)
{
    if (wParam == VK_F5)
    {
        if (lParam == VK_CONTROL)
        {
            select_sql_stat(pnode);
        }
        return on_table_sql_query(pnode, NULL);
    }
    return 0;
}

int
on_doc_keydown_redis(eu_tabpage *pnode, WPARAM wParam, LPARAM lParam)
{
    if (wParam == VK_F5)
    {
        if (lParam == VK_CONTROL)
        {
            select_sql_stat(pnode);
        }
        return on_symtree_query_redis(pnode);
    }
    return 0;
}

int
on_doc_keyup_general(eu_tabpage *pnode, WPARAM wParam, LPARAM lParam)
{
    return on_doc_keydown_light(pnode);
}

int
on_doc_keyup_general_sh(eu_tabpage *pnode, WPARAM wParam, LPARAM lParam)
{
    TCHAR *sp = on_doc_get_ext(pnode);
    if (sp && _tcsstr(_T(";*.bat;*.cmd;*.nt;"), sp))
    {
        return 0;
    }
    return on_doc_keydown_light(pnode);
}

int
on_doc_reload_list_reqular(eu_tabpage *pnode)
{
    return on_symlist_reqular(pnode);
}

int
on_doc_reload_list_sh(eu_tabpage *pnode)
{
    TCHAR *sp = on_doc_get_ext(pnode);
    if (sp && _tcsstr(_T(";*.bat;*.cmd;*.nt;"), sp))
    {
        return 0;
    }
    return on_symlist_reqular(pnode);
}

int
on_doc_click_list_jmp(eu_tabpage *pnode)
{
    return on_symlist_jump_item(pnode);
}

int
on_doc_click_list_jump_sh(eu_tabpage *pnode)
{
    TCHAR *sp = on_doc_get_ext(pnode);
    if (sp && _tcsstr(_T(";*.bat;*.cmd;*.nt;"), sp))
    {
        return 0;
    }
    return on_symlist_jump_item(pnode);
}

int
on_doc_reload_tree_sql(eu_tabpage *pnode)
{
    if (!pnode)
    {
        return 1;
    }
    if (pnode->db_is_connect)
    {
        on_table_disconnect_database(pnode);
    }
    return on_symtree_do_sql(pnode, true);
}

int
on_doc_reload_tree_redis(eu_tabpage *pnode)
{
    int err = 0;
    if (!pnode)
    {
        return 1;
    }    
    if (pnode->redis_is_connect)
    {
        // 关闭redis数据库链接
        on_symtree_disconnect_redis(pnode);
        err = on_symtree_connect_redis(pnode);
    }
    return err;
}

int
on_doc_reload_tree_json(eu_tabpage *pnode)
{
    return on_symtree_json(pnode);
}

int
on_doc_click_tree_sql(eu_tabpage *pnode)
{
    return on_symtree_add_text(pnode);
}

int
on_doc_click_tree_json(eu_tabpage *pnode)
{
    return on_symtree_postion(pnode);
}

int
on_doc_click_tree_redis(eu_tabpage *pnode)
{
    return on_symtree_add_text(pnode);
}

int
on_doc_count(void)
{
    int count = 0;
    for (doctype_t *doc_ptr = g_doc_config; doc_ptr->doc_type != DOCTYPE_END; ++doc_ptr)
    {
        ++count;
    }
    return count;
}

doctype_t *
on_doc_get_type(const TCHAR *pfile)
{
#define EXTRA_EXT "CMakeLists"
    const char *split = NULL;
    size_t str_len = 0;
    char filename[MAX_PATH] = { 0 };
    char extname[_MAX_EXT + 1] = { 0 };
    doctype_t *doc_ptr = NULL;
    if (!(pfile && *pfile && g_doc_config))
    {
        return NULL;
    }
    if (!WideCharToMultiByte(CP_UTF8, 0, pfile, -1, filename, MAX_PATH, NULL, NULL))
    {
        return NULL;
    }
    if ((str_len = strlen(filename)) > 0 && filename[str_len - 1] == '*')
    {
        filename[str_len - 1]  = 0;
    }
    if (_strnicmp(filename, EXTRA_EXT, strlen(EXTRA_EXT)) == 0)
    {
        _snprintf(extname, _MAX_EXT, ";%s;", filename);
    }    
    else if ((split = strrchr(filename, '.')) == NULL)
    {
        _snprintf(extname, _MAX_EXT, ";%s;", filename);
    }  
    else
    {
        _snprintf(extname, _MAX_EXT, ";*%s;", split);
    }
    for (doc_ptr = g_doc_config; doc_ptr->doc_type != DOCTYPE_END; doc_ptr++)
    {
        if (eu_strcasestr(doc_ptr->extname, extname))
        {
            return doc_ptr;
        }
    }
    return NULL;
#undef EXTRA_EXT
}

doctype_t*
eu_doc_get_ptr(void)
{
    return g_doc_config;
}

void
eu_doc_set_ptr(doctype_t *ptr)
{
    g_doc_config = ptr;
}

void
eu_doc_config_release(void)
{
    for (doctype_t *mapper = g_doc_config; mapper && mapper->filetypename; ++mapper)
    {
        eu_destory_completed_tree(&mapper->acshow_tree);
        eu_destory_completed_tree(&mapper->ctshow_tree);
    }
    do_lua_parser_release();
}
