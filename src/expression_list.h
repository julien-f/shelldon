/**
 * This file is a part of Shelldon.
 *
 * Shelldon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Shelldon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Shelldon.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef EXPRESSION_LIST_H
#define EXPRESSION_LIST_H

#include <stdlib.h>

#include "assert.h"
#include "expression.h"

typedef struct ExpressionList ExpressionList;
typedef struct ExpressionListClass ExpressionListClass;

#define EXPRESSION_LIST(pointer) ((ExpressionList *) pointer)

#define EXPRESSION_LIST_CLASS(pointer) ((ExpressionListClass *) pointer)

#define EXPRESSION_LIST_GET_CLASS(pointer) (EXPRESSION_LIST_CLASS(OBJECT(pointer)->klass))

/**
 * Represents an ExpressionList class.
 */
struct ExpressionListClass {
	ExpressionClass parent;
	int (*evaluate) (ExpressionList *);
};

/**
 * Allocates and initializes a new ExpressionList class of size "size".
 *
 * All fields are initialized except Expression.name.
 *
 * This function is only useful to create a derivated class of ExpressionList.
 */
ExpressionListClass *
expression_list_class_allocate (size_t size, void *parent_class, char *name);

/**
 * Returns the ExpressionList class.
 *
 * This function is only useful to create a derivated class of ExpressionList.
 */
const ExpressionListClass *
expression_list_class_get ();

/**
 * Represents an instance of the ExpressionList type.
 */
struct ExpressionList {
	Expression parent;
};

/**
 * Allocates a new ExpressionList object.
 */
ExpressionList *
expression_list_construct (size_t size, void *klass);

/**
 * Allocates and initializes a new ExpressionList object.
 */
static inline ExpressionList *
expression_list_new ()
{
	return expression_list_construct (sizeof (ExpressionList), (void *) expression_list_class_get ());
}

#endif

