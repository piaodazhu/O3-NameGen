/* -*- Mode:C; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018
 * Regents of the University of Arizona & University of Michigan.
 *
 * TrieGranularity source code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with TrieGranularity, e.g., in COPYING.md or LICENSE file.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * For list of authors, please see AUTHORS.md file.
 *
 * Description:
 * NameGen tries to analyze the URLs statistics and generate NDN names accordingly.
 * NameGen is a free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <getopt.h>

#include "name_stat.h"

/* ---------------------------------------------------
 * Method: name_parsing ()
 * Scope: Public
 *
 * Description:
 * url parsing
 * --------------------------------------------------- */
char *name_parsing(char *str)
{
    /* -----------------remove protocol (http://www., https://www., www., ftp://, etc.)------------------*/
    int index = 0, i = 0;

    if (strncmp(str, "http://", strlen("http://")) == 0)
        index = strlen("http://");
    else if (strncmp(str, "https://", strlen("https://")) == 0)
        index = strlen("https://");
    /* ------------------
     * [TODO] ....... : more patterns to be added
     *     ....;
     * -----------------
     */
    if (index == 0)
        return NULL;

    char buf[1024];
    char *rptr = buf + 1024;
    int pre = index, clen = 0, plen = 0;
    for (i = index; i < strlen(str); i++)
    {
        if (str[i] == '.' || str[i] == '/')
        {
            clen = i - pre;
            rptr -= clen;
            memcpy(rptr, str + pre, clen);
            --rptr;
            *rptr = '/';
            pre = i + 1;
            plen += clen;
            ++plen;
        }
        if (str[i] == '/')
            break;
    }
    if (i == strlen(str))
    {
        clen = i - pre;
        rptr -= clen;
        memcpy(rptr, str + pre, clen);
        --rptr;
        *rptr = '/';
        pre = i + 1;
        plen += clen;
        ++plen;
    }
    memcpy(str, rptr, plen);
    if (i != strlen(str))
    {
        memmove(str + plen, str + i, strlen(str) - i);
        str[plen + strlen(str) - i] = '\0';
    }
    else
    {
        str[plen] = '\0';
    }
    return (str);
} /* -- end of name_parsing () -- */
