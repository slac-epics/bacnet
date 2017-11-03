/*
 * indtext.h
 *
 *  Created on: Nov 30, 2011
 *      Author: 8w4
 */

#ifndef INDTEXT_H
#define INDTEXT_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* index and text pairs */
typedef struct {
    unsigned index;     /* index number that matches the text */
    const char *pString;        /* text pair - use NULL to end the list */
} INDTEXT_DATA;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*  Searches for a matching string and returns the index to the string
    in the parameter found_index.
    If the string is not found, false is returned
    If the string is found, true is returned and the found_index contains
    the first index where the string was found. */
    bool indtext_by_string(
        INDTEXT_DATA * data_list,
        const char *search_name,
        unsigned *found_index);
/* case insensitive version */
    bool indtext_by_istring(
        INDTEXT_DATA * data_list,
        const char *search_name,
        unsigned *found_index);
/*  Searches for a matching string and returns the index to the string
    or the default_index if the string is not found. */
    unsigned indtext_by_string_default(
        INDTEXT_DATA * data_list,
        const char *search_name,
        unsigned default_index);
/* case insensitive version */
    unsigned indtext_by_istring_default(
        INDTEXT_DATA * data_list,
        const char *search_name,
        unsigned default_index);
/* for a given index, return the matching string,
   or NULL if not found */
    const char *indtext_by_index(
        INDTEXT_DATA * data_list,
        unsigned index);
/* for a given index, return the matching string,
   or default_name if not found */
    const char *indtext_by_index_default(
        INDTEXT_DATA * data_list,
        unsigned index,
        const char *default_name);
/* for a given index, return the matching string,
   or default_name if not found.
   if the index is before the split,
   the before_split_default_name is used */
    const char *indtext_by_index_split_default(
        INDTEXT_DATA * data_list,
        unsigned index,
        unsigned split_index,
        const char *before_split_default_name,
        const char *default_name);

/* returns the number of elements in the list */
    unsigned indtext_count(
        INDTEXT_DATA * data_list);


    int stricmp(const char *s1, const char *s2);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
