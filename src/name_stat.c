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
 * Method: print_info()
 * Scope: Public 
 * 
 * Description:
 * print statistical info of a single name
 * --------------------------------------------------- */
void print_info(struct name_info *info)
{
    printf("Name Length: %d\n", info->name_length);
    printf("Number of components: %d\n", info->num_of_comp);
    printf("Avg. length of components: %f\n", info->avg_comp_length);
    printf("Max length of components: %d\n", info->max_com_length);
    for (int i = 0; i < MAX_NUM_OF_COMPS; i++)
        if (info->component[i]->comp_chars[0] != 0)
            printf("Componet %d-th is %s\n", i + 1, info->component[i]->comp_chars);
} /* -- end of print_info () -- */

/* ---------------------------------------------------
 * Method: print_general_info()
 * Scope: Public 
 * 
 * Description:
 * print general statistical info (all names analysis)
 * --------------------------------------------------- */
void print_general_info(struct general_info *gen)
{
    FILE *f1 = fopen("../output/num_of_comp_dist.txt", "w");
    if (f1 == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    FILE *f2 = fopen("../output/comp_length_dist.txt", "w");
    if (f2 == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    FILE *f3 = fopen("../output/inputname_basic.txt", "w");
    if (f3 == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f3, "Number of names: %ld\n", gen->num_of_names);
    fprintf(f3, "Avg. length of names: %f\n", gen->avg_name_length / gen->num_of_names);
    fprintf(f3, "Max length of names: %d\n", gen->max_length_of_names);
    fprintf(f3, "Avg. length of components: %f\n", gen->avg_comp_length / gen->num_of_names);
    fprintf(f3, "Max length of components: %d\n", gen->max_length_of_comps);
    double tmp = 0;
    for (int i = 0; i < MAX_NUM_OF_COMPS; i++)
    {
        // printf("%d-th component count %f\n", i, gen->num_of_comps[i]);
        gen->num_of_comps[i] /= gen->num_of_names;
        fprintf(f1, " %f", gen->num_of_comps[i]);
    }
    for (int i = 0; i < MAX_NUM_OF_COMPS; i++)
        tmp += (i + 1) * gen->num_of_comps[i];
    fprintf(f3, "Avg. Number of components: %f\n", tmp);
    fprintf(f3, "Max Number of components: %d\n", gen->max_num_of_comps);

    for (int index_comp = 0; index_comp < MAX_NUM_OF_COMPS; index_comp++)
    {
        fprintf(f2, "============%d-th Component=====================\n", index_comp + 1);
        for (int i = 0; i < MAX_NUM_OF_CHARS_IN_COMP; i++)
        {
            if (gen->component[index_comp]->comp_length[i] != 0)
            {
                // printf("component %d, length = %d: %f\n", index_comp, i, gen->component[index_comp]->comp_length[i]);
            }

            gen->component[index_comp]->comp_length[i] /= gen->comp_count_presence[index_comp];
            fprintf(f2, " %f", gen->component[index_comp]->comp_length[i]);
        }
        /*
        printf("=================INFO of %d-th Component=====================\n", index_comp+1);
        printf("Avg. length of %d-th component: %f\n", index_comp+1, gen->component[index_comp]->avg_comp_length / gen->comp_count_presence[index_comp]);
        printf("Max length of %d-th component: %d\n", index_comp+1, gen->component[index_comp]->max_comp_length);  
        */
        // fprintf(f3, "============%d-th Component=====================\n", index_comp + 1);
        // int tmp = 0;
        // for (int i = 0; i < NUM_OF_POSSIBLE_CHARS + 1; i++)
        // {
        //     fprintf(f3, " %d", i);
        //     tmp = 0;
        //     for (int j = 0; j < NUM_OF_POSSIBLE_CHARS + 1; j++)
        //         tmp += gen->component[index_comp]->markov_matrix_o1[i][j];
        //     for (int j = 0; j < NUM_OF_POSSIBLE_CHARS + 1; j++)
        //         if (tmp != 0)
        //             fprintf(f3, " %f", gen->component[index_comp]->markov_matrix_o1[i][j] / tmp);
        //         else
        //             fprintf(f3, " %f", 0.0);
        //     fprintf(f3, "\n");
        // }
        // printf("============%d-th Component=====================\n", index_comp + 1);
        double sumofchars = 0.0;
        for (int i = 0; i < NUM_OF_POSSIBLE_CHARS_O3; i++)
        {
            sumofchars += gen->component[index_comp]->char_freq[i];
        }
        if (sumofchars - 0.0 > 0.1)
        {
            for (int i = 0; i < NUM_OF_POSSIBLE_CHARS_O3; i++)
            {
                gen->component[index_comp]->char_freq[i] /= sumofchars;
            }
        }
        // for (int i = 0; i < NUM_OF_POSSIBLE_CHARS_O3; i++) {
        //     printf("%c: %d\t", idx2char(i), gen->component[index_comp]->char_freq[i]);

        //     // for (int j = 0; j < NUM_OF_POSSIBLE_CHARS_O3; j++)
        //     // {
        //     //     double count = gen->component[index_comp]->markov_matrix_o3[63][63][i][j];
        //     //     if (count - 0.0 > 0.1)
        //     //         printf("%c: %f\t", idx2char(j), count);
        //     // }
        //     // printf("\n");
        // }
        // printf("\n");
    }
    fclose(f1);
    fclose(f2);
    fclose(f3);
} /* -- end of print_general_info () -- */

/* ---------------------------------------------------
 * Method: update_general_info()
 * Scope: Public
 *
 * Description:
 * Extract general statistical info (all names analysis)
 * --------------------------------------------------- */
void update_general_info(struct general_info *gen, struct name_info *info)
{
    gen->num_of_names++;
    gen->avg_name_length += info->name_length;
    if (gen->max_length_of_names < info->name_length)
        gen->max_length_of_names = info->name_length;
    gen->avg_comp_length += info->avg_comp_length; //--[TODO] Here, we report per-name length.//
    if (gen->max_length_of_comps < info->max_com_length)
        gen->max_length_of_comps = info->max_com_length;
    gen->num_of_comps[(info->num_of_comp) - 1]++;
    if (gen->max_num_of_comps < info->num_of_comp)
        gen->max_num_of_comps = info->num_of_comp;

    for (int i = 0; i < info->num_of_comp; i++)
    {
        gen->component[i]->comp_length[strlen(info->component[i]->comp_chars) - 1]++;
        gen->component[i]->avg_comp_length += strlen(info->component[i]->comp_chars);
        if (gen->component[i]->max_comp_length < strlen(info->component[i]->comp_chars))
            gen->component[i]->max_comp_length = strlen(info->component[i]->comp_chars);
        gen->comp_count_presence[i]++;
        for (int j = 0; j < NUM_OF_POSSIBLE_CHARS_O3; j++)
            gen->component[i]->char_freq[j] += info->component[i]->char_freq[j];

        //===============Train MARKOV (Order-3 model): Transition Probability Calc==================
        if (strlen(info->component[i]->comp_chars) > 3)
            for (int j = 0; j < (strlen(info->component[i]->comp_chars) - 3); j++)
                gen->component[i]->markov_matrix_o3[char2idx(info->component[i]->comp_chars[j])][char2idx(info->component[i]->comp_chars[j + 1])][char2idx(info->component[i]->comp_chars[j + 2])][char2idx(info->component[i]->comp_chars[j + 3])]++;
        if (strlen(info->component[i]->comp_chars) > 2)
            gen->component[i]->markov_matrix_o3[63][char2idx(info->component[i]->comp_chars[0])][char2idx(info->component[i]->comp_chars[1])][char2idx(info->component[i]->comp_chars[2])]++; //--for transition to the second char --//
        if (strlen(info->component[i]->comp_chars) > 1)
            gen->component[i]->markov_matrix_o3[63][63][char2idx(info->component[i]->comp_chars[0])][char2idx(info->component[i]->comp_chars[1])]++; //--for transition to the second char --//
        gen->component[i]->markov_matrix_o3[63][63][63][char2idx(info->component[i]->comp_chars[0])]++;                                              //--for transition to the first char --//
    }
} /* -- end of update_general_info () -- */

/* ---------------------------------------------------
 * Method: name_proc()
 * Scope: Public
 *
 * Description:
 * Extract statistical info of a name
 * --------------------------------------------------- */
struct name_info *name_proc(const char *name, struct name_info *info)
{
    int num_of_component = 0;
    int i = 0, j = 0;
    while (i < strlen(name))
    {
        if (name[i] == '/')
        {
            num_of_component++;
            j = 0;
        }
        else
        {
            info->component[num_of_component - 1]->comp_chars[j] = name[i];

            if ((name[i] >= '0' && name[i] <= '9') || (name[i] >= 'a' && name[i] <= 'z'))
                info->component[num_of_component - 1]->char_freq[char2idx(name[i])] += 1;

            j++;
        }
        i++;
    }
    info->name_length = strlen(name);
    info->num_of_comp = num_of_component;
    for (i = 0; i < MAX_NUM_OF_COMPS; i++)
    {
        info->component[i]->comp_length = strlen(info->component[i]->comp_chars);
        info->avg_comp_length += strlen(info->component[i]->comp_chars);
        if (info->max_com_length < info->component[i]->comp_length)
            info->max_com_length = info->component[i]->comp_length;
    }
    info->avg_comp_length /= info->num_of_comp;
    // printf("name: %s\n", name);
    // printf("info: totallen = %d, totalcmp = %d, cmplen = %f\n", info->name_length, info->num_of_comp, info->avg_comp_length);
    return (info);
} /* -- end of name_proc () -- */

void print_newgeneral_info(struct newgeneral_info *gen)
{
    FILE *f1 = fopen("../output/new_num_of_comp_dist.txt", "w");
    if (f1 == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    FILE *f2 = fopen("../output/new_comp_length_dist.txt", "w");
    if (f2 == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    FILE *f3 = fopen("../output/outputname_basic.txt", "w");
    if (f3 == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f3, "Number of names: %ld\n", gen->num_of_names);
    fprintf(f3, "Avg. length of names: %f\n", gen->avg_name_length / gen->num_of_names);
    fprintf(f3, "Max length of names: %d\n", gen->max_length_of_names);
    fprintf(f3, "Avg. length of components: %f\n", gen->avg_comp_length / gen->num_of_names);
    fprintf(f3, "Max length of components: %d\n", gen->max_length_of_comps);
    double tmp = 0;
    for (int i = 0; i < MAX_NUM_OF_COMPS; i++)
    {
        // printf("%d-th component count %f\n", i, gen->num_of_comps[i]);
        gen->num_of_comps[i] /= gen->num_of_names;
        fprintf(f1, " %f", gen->num_of_comps[i]);
    }
    for (int i = 0; i < MAX_NUM_OF_COMPS; i++)
        tmp += (i + 1) * gen->num_of_comps[i];
    fprintf(f3, "Avg. Number of components: %f\n", tmp);
    fprintf(f3, "Max Number of components: %d\n", gen->max_num_of_comps);

    for (int index_comp = 0; index_comp < MAX_NUM_OF_COMPS; index_comp++)
    {
        fprintf(f2, "============%d-th Component=====================\n", index_comp + 1);
        for (int i = 0; i < MAX_NUM_OF_CHARS_IN_COMP; i++)
        {
            if (gen->component[index_comp]->comp_length[i] != 0)
            {
                // printf("component %d, length = %d: %f\n", index_comp, i, gen->component[index_comp]->comp_length[i]);
            }

            gen->component[index_comp]->comp_length[i] /= gen->comp_count_presence[index_comp];
            fprintf(f2, " %f", gen->component[index_comp]->comp_length[i]);
        }
    }
    fclose(f1);
    fclose(f2);
    fclose(f3);
} /* -- end of print_general_info () -- */

void update_newgeneral_info(struct newgeneral_info *gen, struct name_info *info)
{
    gen->num_of_names++;
    gen->avg_name_length += info->name_length;
    if (gen->max_length_of_names < info->name_length)
        gen->max_length_of_names = info->name_length;
    gen->avg_comp_length += info->avg_comp_length; //--[TODO] Here, we report per-name length.//
    if (gen->max_length_of_comps < info->max_com_length)
        gen->max_length_of_comps = info->max_com_length;
    gen->num_of_comps[(info->num_of_comp) - 1]++;
    if (gen->max_num_of_comps < info->num_of_comp)
        gen->max_num_of_comps = info->num_of_comp;

    for (int i = 0; i < info->num_of_comp; i++)
    {
        gen->component[i]->comp_length[strlen(info->component[i]->comp_chars) - 1]++;
        gen->component[i]->avg_comp_length += strlen(info->component[i]->comp_chars);
        if (gen->component[i]->max_comp_length < strlen(info->component[i]->comp_chars))
            gen->component[i]->max_comp_length = strlen(info->component[i]->comp_chars);
        gen->comp_count_presence[i]++;
    }
} /* -- end of update_general_info () -- */

void gen_new_names_o3_withinfo(struct general_info *gen, long num_of_new_names)
{
    // --------
    struct newgeneral_info *newgen = (struct newgeneral_info *)malloc(sizeof(struct general_info));
    newgen->num_of_names = 0;
    newgen->avg_name_length = 0;
    newgen->max_length_of_names = 0;
    newgen->avg_comp_length = 0;
    newgen->max_length_of_comps = 0;
    // printf("DBG1\n");
    for (int i = 0; i < MAX_NUM_OF_COMPS; i++)
        newgen->num_of_comps[i] = 0;
    newgen->max_num_of_comps = 0;
    for (int i = 0; i < MAX_NUM_OF_COMPS; i++)
    {
        newgen->component[i] = (struct newgeneral_comp_info *)malloc(sizeof(struct newgeneral_comp_info));
        if (newgen->component[i] == NULL)
        {
            // printf("DBG5, i = %d\n", i);
        }
        newgen->component[i]->max_comp_length = 0;
        newgen->component[i]->avg_comp_length = 0;
        for (int j = 0; j < MAX_NUM_OF_CHARS_IN_COMP; j++)
            newgen->component[i]->comp_length[j] = 0;
    }
    for (int i = 0; i < MAX_NUM_OF_COMPS; i++)
        newgen->comp_count_presence[i] = 0;
    newgen->opt_comp_length = 0;
    newgen->opt_num_of_comps = 0;
    // printf("DBG2\n");
    struct name_info *newinfo = (struct name_info *)malloc(sizeof(struct name_info));
    for (int i = 0; i < MAX_NUM_OF_COMPS; i++)
    {
        newinfo->component[i] = (struct comp_info *)malloc(sizeof(struct comp_info));
        newinfo->component[i]->comp_chars = (char *)malloc(MAX_NUM_OF_CHARS_IN_COMP * sizeof(char));
    }

    // print_newgeneral_info(newgen);
    // exit(0);
    //-------
    FILE *f5 = fopen("../output/o3_generated_names.txt", "w");
    if (f5 == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    char *new_name = malloc(MAX_LENGTH_OF_NAME * sizeof(char));
    for (int i = 0; i < num_of_new_names; i++)
    {
        for (int j = 0; j < MAX_LENGTH_OF_NAME; j++)
            new_name[j] = 0;
        int index_in_new_name = 0;
        int new_num_of_comp = 0;
        int new_comp_length = 0;
        int j = 0;
        double sum_prob = 0;
        // printf("DBG3\n");
        if (i % 1000 == 0)
            printf("generate names (%d / %d)\n", i, num_of_new_names);
        //============== BEGIN: calculation of number of components=============
        if (gen->opt_num_of_comps == 0)
        {
            // j = 0;
            // while (i == 0 && j < MAX_NUM_OF_COMPS) {
            //     printf("component length %d: %f, ", j, gen->num_of_comps[j]);
            //     j++;
            // }
            // if (i==0)
            //     printf("\n");
            // j = 0;

            double random = (rand() * 1.0) / RAND_MAX;
            while (j < MAX_NUM_OF_COMPS)
            {
                // double random = (rand() * 1.0) / RAND_MAX; //-- [TODO] make it more accourate if necessary --//
                sum_prob += gen->num_of_comps[j];
                if (sum_prob >= random)
                    break;
                j++;
            }
            // if (random > 0.995)
            //     printf("random = %f, i = %d, j = %d\n", random, i, j);
            new_num_of_comp = j;
            new_num_of_comp++; //-- j is in [0:MAX_NUM_OF_COMPS-1], but number of components is in [1:MAX_NUM_OF_COMPS] --//
            // printf("DBG3: %d\n", new_num_of_comp);
        }
        else
            new_num_of_comp = gen->opt_num_of_comps;

        //============== END: calculation of number of components=============
        int index_comp = 0;
        for (index_comp = 0; index_comp < new_num_of_comp; index_comp++)
        {
            int k = 0;
            double sum_prob = 0;
            //============== BEGIN: calculation of length of a component=============
            if (gen->opt_comp_length == 0)
            {
                double random = 0.0;
            again:
                random = (rand() * 1.0) / RAND_MAX;
                while (k < MAX_NUM_OF_CHARS_IN_COMP)
                {
                    // double random = (rand() % 1000000000) / 1000000000.0; //-- [TODO] make it more accourate if necessary --//
                    sum_prob += gen->component[index_comp]->comp_length[k];
                    if (sum_prob >= random)
                        break;
                    k++;
                }
                if (k == MAX_NUM_OF_CHARS_IN_COMP)
                {
                    // printf("DBG4: %f\n", random);
                    k = 0;
                    goto again;
                }
                new_comp_length = k;
                new_comp_length++; //-- k is in [0:MAX_NUM_OF_CHARS_IN_COMP-1], but length is in [1:MAX_NUM_OF_CHARS_IN_COMP] --//
                // printf("i = %d, compidx = %d, lenofcomp = %d\n", i, index_comp, new_comp_length);
            }
            else
                new_comp_length = gen->opt_comp_length;
            //============== END: calculation of length of a component=============
            int state1 = 63;
            int state2 = 63;
            int state3 = 63;
            index_in_new_name = strlen(new_name);
            new_name[index_in_new_name] = '/';
            index_in_new_name++;
            // printf("DBG5: component length %d, component idx: %d\n", new_comp_length, index_comp);
            // printf("DBG5.5: char range:%d - %d\n", index_in_new_name, index_in_new_name + new_comp_length);
            //============== BEGIN: generating characters for a component=============
            for (int l = index_in_new_name; l < (index_in_new_name + new_comp_length); l++)
            {
                // printf("DBG6: l = %d, s1=%d, s2=%d, s3=%d\n", l, state1, state2, state3);
                double random = (rand() % 1000000000) / 1000000000.0; //-- [TODO] make it more accourate if necessary --//
                double random2 = (rand() % 1000000000) / 1000000000.0;
                double sum_markov_o3 = 0;
                double offset = 0.05;
                int s = 0;
                for (int m = 0; m < NUM_OF_POSSIBLE_CHARS_O3; m++)
                    sum_markov_o3 += gen->component[index_comp]->markov_matrix_o3[state1][state2][state3][m];
                // zero condition
                if (sum_markov_o3 - 0.0 < 0.1)
                {
                    // TODO: choose next char by the char_freq
                    double random3 = (rand() % 1000000000) / 1000000000.0;
                    double sum_prob = 0;
                    s = 0;
                    while (s < 62)
                    {
                        sum_prob += gen->component[index_comp]->char_freq[s];
                        if (sum_prob >= random3)
                            break;
                        s++;
                    }
                }
                else
                {
                    if (random2 < offset)
                    {
                        // TODO: choose next char by the char_freq
                        double random3 = (rand() % 1000000000) / 1000000000.0;
                        double sum_prob = 0;
                        s = 0;
                        while (s < 62)
                        {
                            sum_prob += gen->component[index_comp]->char_freq[s];
                            if (sum_prob >= random3)
                                break;
                            s++;
                        }
                    }
                    else
                    {
                        // markov
                        double sum_prob = 0;
                        s = 0;
                        while (s < 62)
                        {
                            sum_prob += (gen->component[index_comp]->markov_matrix_o3[state1][state2][state3][s] / sum_markov_o3);
                            if (sum_prob >= random)
                                break;
                            s++;
                        }
                    }
                }
                // printf("sum markov: %f, random = %f\n", sum_markov_o3, random);

                //--[TODO] here if there is no pattern to continue, we simply return to the first state (s=128) and repeat character generation
                // printf("s = %d, ", s);
                if (s < 62)
                {
                    new_name[l] = idx2char(s);
                    state1 = state2;
                    state2 = state3;
                    state3 = s;
                }
                else
                {
                    l--;
                }
            }
            //============== END: generating characters for a component=============
        }
        //printf("%s\n", new_name);
        fprintf(f5, "%s\n", new_name);
        newinfo->name_length = 0;
        newinfo->num_of_comp = 0;
        newinfo->avg_comp_length = 0;
        newinfo->max_com_length = 0;
        for (int i = 0; i < MAX_NUM_OF_COMPS; i++)
        {
            for (int j = 0; j < MAX_NUM_OF_CHARS_IN_COMP; j++)
                newinfo->component[i]->comp_chars[j] = 0; //--[TODO] how to initialize component characters--//
            newinfo->component[i]->comp_length = 0;
        }
        if (!name_proc(new_name, newinfo))
        {
            fprintf(stderr, "ERROR in Name Statistics Analysis");
        }
        else
        {
            //print_info (info);
            update_newgeneral_info(newgen, newinfo);
        }
    }
    print_newgeneral_info(newgen);
    fclose(f5);
} /* -- end of gen_new_names_o2 () -- */