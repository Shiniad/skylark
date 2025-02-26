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

#include "framework.h"

/* rc_string part */
#define RSTRING_INCSTEP 5
#define RSTRING_DEFAULT 8

typedef enum _rstring_code
{
    RS_MEMORY,
    RS_OK = 1,
    RS_UNKNOWN
}rstring_code;

typedef bool (*ptr_format)(const char *filename, const char *data, size_t size, char **pout);

static rcstring *
rcs_create(size_t length)
{
    rcstring *rcs;
    rcs = (rcstring *) malloc(sizeof(rcstring)); /* allocates memory for a struct rcstring */
    if (rcs == NULL)
    {
        return NULL;
    }
    rcs->max = length;
    rcs->length = 0;
    rcs->text = (char *) malloc((rcs->max + 1) * sizeof(char));
    if (rcs->text == NULL)
    {
        free(rcs);
        return NULL;
    }
    rcs->text[0] = '\0';
    return rcs;
}

static void
rcs_free(rcstring **rcs)
{
    EU_VERIFY(rcs != NULL);
    if (*rcs != NULL)
    {
        if ((*rcs)->text != NULL)
        {
            free((*rcs)->text);
            (*rcs)->text = NULL;
        }
        free(*rcs);
        *rcs = NULL;
    }
}

static rstring_code
rcs_resize(rcstring *rcs, size_t length)
{
    if (!rcs)
    {
        return RS_UNKNOWN;
    }
    char *temp = (char *) realloc(rcs->text, sizeof(char) * (length + 1)); /* length plus '\0' */
    if (temp == NULL)
    {
        rcs_free(&rcs);
        return RS_MEMORY;
    }
    rcs->text = temp;
    rcs->max = length;
    rcs->text[rcs->max] = '\0';
    return RS_OK;
}

static rstring_code
rcs_catcs(rcstring *pre, const char *pos, const size_t length)
{
    EU_VERIFY(pos != NULL);
    if (!pre)
    {
        return RS_UNKNOWN;
    }
    if (pre->max < pre->length + length)
    {
        if (rcs_resize(pre, pre->length + length + RSTRING_INCSTEP) != RS_OK)
        {
            return RS_MEMORY;
        }
    }
    strncpy(pre->text + pre->length, pos, length);
    pre->text[pre->length + length] = '\0';
    pre->length += length;
    return RS_OK;
}

static rstring_code
rcs_catc(rcstring *pre, const char c)
{
    if (!pre)
    {
        return RS_UNKNOWN;
    }
    if (pre->max <= pre->length)
    {
        if (rcs_resize(pre, pre->max + RSTRING_INCSTEP) != RS_OK) 
        {
            return RS_MEMORY;
        }
    }
    pre->text[pre->length] = c;
    pre->length++;
    pre->text[pre->length] = '\0';
    return RS_OK;
}

#define DO_RCS_CATCS(p, q, l) if (rcs_catcs(p, q, l) != RS_OK) return NULL
#define DO_RCS_CATC(p, c) if (rcs_catc(p, c) != RS_OK) return NULL

static char *
rcs_unwrap(rcstring *rcs)
{
    char *out;
    if (!rcs)
    {
        return NULL;
    }
    if (!rcs->text)
    {
        out = NULL;
    }
    else if ((out = (char *) realloc(rcs->text, sizeof(char) * (strlen(rcs->text) + 1))) == NULL)
    {
        rcs_free(&rcs);
        return NULL;
    }
    free(rcs);
    return out;
}

static size_t
rcs_length(rcstring *rcs)
{
    /*TODO account for UTF8 */
    EU_VERIFY(rcs != NULL);
    return rcs->length;
}

char *
format_undo_json_string(const char *text)
{
    rcstring *output;
    size_t i;
    size_t length;
    /* check if pre-conditions are met */
    EU_VERIFY(text != NULL);
    /* defining the temporary variables */
    length = strlen(text);
    if ((output = rcs_create(length)) == NULL)
    {
        return NULL;
    }
    for (i = 0; i < length; i++)
    {
        if (isspace(text[i]))
        {
            continue;
        }
        else if (text[i] == '/' && text[i+1] == '/')
        {
            // 删除单行注释
            char *p = strchr(&text[i+1], '\n');
            if (p)
            {
                i += (p - &text[i+1]);
                continue;
            }
        }
        else
        {
            DO_RCS_CATC(output, text[i]);
        }
    }
    return rcs_unwrap(output);
}

char *
format_do_json_string(const char *text)
{
    size_t pos = 0;
    size_t text_length;
    unsigned int indentation = 0; /* the current indentation level */
    unsigned int i;               /* loop iterator variable */
    char loop;

    rcstring *output;
    text_length = strlen(text);
    if ((output = rcs_create(text_length)) == NULL)
    {
        return NULL;
    }
    while (pos < text_length)
    {
        switch (text[pos])
        {
            case '\x20':
            case '\x09':
            case '\x0A':
            case '\x0D': /* JSON insignificant white spaces */
                pos++;
                break;

            case '{':
                indentation++;
                DO_RCS_CATCS(output, "{\n", 2);
                for (i = 0; i < indentation; i++)
                {
                    DO_RCS_CATC(output, '\t');
                }
                pos++;
                break;

            case '}':
                indentation--;
                DO_RCS_CATC(output, '\n');
                for (i = 0; i < indentation; i++)
                {
                    DO_RCS_CATC(output, '\t');
                }
                DO_RCS_CATC(output, '}');
                pos++;
                break;

            case ':':
                DO_RCS_CATCS(output, ": ", 2);
                pos++;
                break;

            case ',':
                DO_RCS_CATCS(output, ",\n", 2);
                for (i = 0; i < indentation; i++)
                {
                    DO_RCS_CATC(output, '\t');
                }
                pos++;
                break;

            case '\"': /* open string */
                DO_RCS_CATC(output, text[pos]);
                pos++;
                loop = 1; /* inner string loop trigger is enabled */
                while (loop)
                {
                    if (text[pos] == '\\')
                    { /* escaped sequence */
                        DO_RCS_CATC(output, '\\');
                        pos++;
                        if (text[pos] == '\"')
                        { /* don't consider a \" escaped sequence as an end of string */
                            DO_RCS_CATC(output, '\"');
                            pos++;
                        }
                    }
                    else if (text[pos] == '\"')
                    { /* reached end of string */
                        loop = 0;
                    }
                    DO_RCS_CATC(output, text[pos]);
                    pos++;
                    if (pos >= text_length)
                    {
                        loop = 0;
                    }
                }
                break;
            case '/':
                if (text[pos+1] == '/')
                {   // 保留单行注释
                    while (text[pos] != '\n')
                    {
                        DO_RCS_CATC(output, text[pos++]);
                    }
                    DO_RCS_CATC(output, '\n');
                    for (i = 0; i < indentation; i++)
                    {
                        DO_RCS_CATC(output, '\t');
                    }
                }
                else if (text[pos+1] == '*')
                {   // 保留块注释
                    char *p = strstr(&text[i+1], "*/");
                    if (p)
                    {
                        DO_RCS_CATCS(output, &text[pos], (p - &text[pos]) + 2);
                        DO_RCS_CATC(output, '\n');
                        pos += (p - &text[pos]);
                    }
                    for (i = 0; i < indentation; i++)
                    {
                        DO_RCS_CATC(output, '\t');
                    }
                }
                pos++;
                break;
            default:
                DO_RCS_CATC(output, text[pos]);
                pos++;
                break;
        }
    }
    return rcs_unwrap(output);
}

void
format_do_json_file(eu_tabpage *pnode, format_back fn)
{
    char *text = NULL;
    char *fjson = NULL;
    if (!pnode)
    {
        return;
    }
    if (!pnode->hwnd_symtree)
    {
        MSG_BOX(IDC_MSG_JSON_ERR1, IDC_MSG_ERROR, MB_ICONERROR | MB_OK);
        return;
    }
    if (!(text = util_strdup_content(pnode, NULL)))
    {
        return;
    }
    eu_sci_call(pnode, SCI_BEGINUNDOACTION, 0, 0);
    if ((fjson = fn(text)))
    {
        eu_sci_call(pnode, SCI_CLEARALL, 0, 0);
        eu_sci_call(pnode, SCI_ADDTEXT, strlen(fjson), (LPARAM) fjson);
        free(fjson);
    }
    eu_sci_call(pnode, SCI_ENDUNDOACTION, 0, 0);
    free(text);
}

static void
init_stderr_redirect(FILE **pconsole)
{
    *pconsole = NULL;
    if ((_fileno(stderr) != 2) && AllocConsole())
    {
        *pconsole = freopen("CONOUT$", "w", stderr);
        ShowWindow (FindWindow(_T("ConsoleWindowClass"), NULL), SW_HIDE);
    }
}

static bool
do_lib_format(const char *filename, const char *data, size_t size, char **pout)
{
    bool ret = false;
    TCHAR format_path[MAX_PATH+1] = {0};
    HMODULE m_dll = NULL;
    _sntprintf(format_path, MAX_PATH, _T("%s\\clang-format.dll"), eu_module_path);
    if ((m_dll = LoadLibrary(format_path)))
    {
        ptr_format fn_lib_format = (ptr_format) GetProcAddress(m_dll, "lib_format");
        if (fn_lib_format)
        {
            FILE *console;
            init_stderr_redirect(&console);
            ret = fn_lib_format(filename, data, size, pout);
            safe_close_console(console);
        }
        FreeLibrary(m_dll);
    }
    return ret;
}

void
format_file_with_clang(eu_tabpage *pnode)
{
    char *out = NULL;
    char *text = NULL;
    sptr_t text_len = 0;
    char *filename = NULL;
    if (!pnode)
    {
        return;
    } 
    filename = eu_utf16_utf8(pnode->filename, NULL);
    if (!filename)
    {
        return;
    }
    if (!(text = util_strdup_content(pnode, (size_t *)&text_len)))
    {
        return;
    }  
    eu_sci_call(pnode, SCI_BEGINUNDOACTION, 0, 0);  
    if (filename[strlen(filename) - 1] == '*')
    {
        filename[strlen(filename) - 1] = 0;
    }
    if (do_lib_format(filename, text, text_len+1, &out))
    {
        eu_sci_call(pnode, SCI_CLEARALL, 0, 0);
        eu_sci_call(pnode, SCI_ADDTEXT, strlen(out), (LPARAM) out);
    }
    eu_sci_call(pnode, SCI_ENDUNDOACTION, 0, 0);
    free(text);
    free(filename);
    if (out)
    {
        free(out);
    }
}

void
format_str_with_clang(eu_tabpage *pnode)
{
    char *out = NULL;
    char *text = NULL;
    char *filename = NULL;
    size_t len = 0;
    if (!pnode)
    {
        return;
    }
    filename = eu_utf16_utf8(pnode->filename, NULL);
    if (!filename)
    {
        return;
    }
    eu_sci_call(pnode, SCI_BEGINUNDOACTION, 0, 0);
    if (filename[strlen(filename) - 1] == '*')
    {
        filename[strlen(filename) - 1] = 0;
    }
    text = util_strdup_select(pnode, &len, 0);
    if (text && do_lib_format(filename, text, len, &out))
    {
        eu_sci_call(pnode, SCI_REPLACESEL, 0, (sptr_t) out);
    }
    eu_sci_call(pnode, SCI_ENDUNDOACTION, 0, 0);
    free(filename);
    if (text)
    {
        free(text);
    }
    if (out)
    {
        free(out);
    } 
}
