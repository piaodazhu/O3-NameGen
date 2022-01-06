/* -*- Mode:C; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018
 * Regents of the University of Arizona & University of Michigan.
 *
 * Description:
 * Statistical name info.
 * NameGen is a free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * NameGen source code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with TrieGranularity, e.g., in COPYING.md or LICENSE file.
 * If not, see <http://www.gnu.org/licenses/>.
 * 
 * For list of authors, please see AUTHORS.md file.
 */

#ifndef NAME_INFO_H
#define NAME_INFO_H

#ifndef MAX_NUM_OF_COMPS
#define MAX_NUM_OF_COMPS 20 //-- we assume that a name has at most 20 components --//
#endif

#ifndef MAX_NUM_OF_CHARS_IN_COMP
#define MAX_NUM_OF_CHARS_IN_COMP 1000 //-- we assume that a component has at most 1000 characters --//
#endif

#ifndef MAX_LENGTH_OF_NAME
#define MAX_LENGTH_OF_NAME 1000 //-- we assume that a name has at most 1000 characters --//
#endif

#ifndef NUM_OF_POSSIBLE_CHARS
#define NUM_OF_POSSIBLE_CHARS 128 //-- we have 128 possible characters --//
#endif

// lzhx start
#ifndef NUM_OF_POSSIBLE_CHARS_O3
#define NUM_OF_POSSIBLE_CHARS_O3 64
#endif

static inline int char2idx(const char ch)
{
     if (ch < 35)
     {
          return -1;
     }
     else if (ch < 65)
     {
          return ch - 35;
     }
     else if (ch < 95)
     {
          return -1;
     }
     else if (ch < 127)
     {
          return ch - 65;
     }
     else
     {
          return -1;
     }
}

static inline char idx2char(const int idx)
{
     if (idx < 30)
     {
          return idx + 35;
     }
     else if (idx < 62)
     {
          return idx + 65;
     }
     else
     {
          return -1;
     }
}

struct newgeneral_info
{
     long num_of_names;                                        //-- number of names --//
     double avg_name_length;                                   //-- average length of all names --//
     int max_length_of_names;                                  //-- max length of all names --//
     double avg_comp_length;                                   //-- average component length of all names --//
     int max_length_of_comps;                                  //-- max length of a component --//
     double num_of_comps[MAX_NUM_OF_COMPS];                    //-- average number of components --//
     int max_num_of_comps;                                     //-- max number of components --//
     long comp_count_presence[MAX_NUM_OF_COMPS];               //-- number of names having component i [1:MAX_NUM_OF_COMPS] --//
     struct newgeneral_comp_info *component[MAX_NUM_OF_COMPS]; //-- general info of components --//
     int opt_comp_length;                                      //-- this optional parameter reads as input and determines the length of all components of a new name to be generated --//
     int opt_num_of_comps;                                     //-- this optional parameter reads as input and determines the number of components of a new name to be generated --//

     /* ------------------
     * optioinal fields:
     *     ....;
     * -----------------
     */
};

struct newgeneral_comp_info
{
     int max_comp_length;                          //-- max length of i-th components --//
     double avg_comp_length;                       //-- average length of i-th components --//
     double comp_length[MAX_NUM_OF_CHARS_IN_COMP]; //-- length of i-th components --//
};

// lzhx end

typedef int bool;
#define true 1
#define false 0

struct general_info
{
     long num_of_names;                                     //-- number of names --//
     double avg_name_length;                                //-- average length of all names --//
     int max_length_of_names;                               //-- max length of all names --//
     double avg_comp_length;                                //-- average component length of all names --//
     int max_length_of_comps;                               //-- max length of a component --//
     double num_of_comps[MAX_NUM_OF_COMPS];                 //-- average number of components --//
     int max_num_of_comps;                                  //-- max number of components --//
     long comp_count_presence[MAX_NUM_OF_COMPS];            //-- number of names having component i [1:MAX_NUM_OF_COMPS] --//
     struct general_comp_info *component[MAX_NUM_OF_COMPS]; //-- general info of components --//
     int opt_comp_length;                                   //-- this optional parameter reads as input and determines the length of all components of a new name to be generated --//
     int opt_num_of_comps;                                  //-- this optional parameter reads as input and determines the number of components of a new name to be generated --//

     /* ------------------
     * optioinal fields:
     *     ....;
     * -----------------
     */
};

struct general_comp_info
{
     int max_comp_length;
     double avg_comp_length;
     double comp_length[MAX_NUM_OF_CHARS_IN_COMP];                                                                                   
     double markov_matrix_o3[NUM_OF_POSSIBLE_CHARS_O3][NUM_OF_POSSIBLE_CHARS_O3][NUM_OF_POSSIBLE_CHARS_O3][NUM_OF_POSSIBLE_CHARS_O3];  

     double char_freq[NUM_OF_POSSIBLE_CHARS_O3];
};

struct name_info
{
     int name_length;                               //-- length of a name --//
     int num_of_comp;                               //-- number of components of a name --//
     double avg_comp_length;                        //-- average length of components of a name --//
     int max_com_length;                            //-- max length of components of a name --//
     struct comp_info *component[MAX_NUM_OF_COMPS]; //-- components of a name --//
                                                    /* ------------------
     * optioinal fields:
     *     ....;
     * -----------------
     */
};

struct comp_info
{
     char *comp_chars; // -- characters of a component -- //
     int comp_length;  // -- length of a component -- //
     /* ------------------
     * optioinal fields:
     *     ....;
     * -----------------
     */
     double char_freq[NUM_OF_POSSIBLE_CHARS_O3];
};

//-----------------------FUNCTIONS-------------------------//
char *name_parsing(char *);                                          // -- parse/pre-process a name -- //
void print_info(struct name_info *);                                 // -- print statistical info of a name -- //
struct name_info *name_proc(const char *, struct name_info *);       // -- retrieve statistical info of a name -- //
void print_general_info(struct general_info *);                      // -- print statistical info of total names -- //
void update_general_info(struct general_info *, struct name_info *); // -- retrieve statistical info of total names -- //
void gen_new_names_o3_withinfo(struct general_info *, long);               // -- generate new names based on the trained order-3 Markov -- //
void print_newgeneral_info(struct newgeneral_info *);                      // -- print statistical info of total
void update_newgeneral_info(struct newgeneral_info *, struct name_info *); // -- retrieve
#endif                                                                     /* -- NAME_INFO_H -- */
