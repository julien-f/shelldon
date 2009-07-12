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

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <stdlib.h>

#include "assert.h"
#include "object.h"

typedef struct Expression Expression;
typedef struct ExpressionClass ExpressionClass;

#define EXPRESSION(pointer) ((Expression *) pointer)

#define EXPRESSION_CLASS(pointer) ((ExpressionClass *) pointer)

/**
 * Represents an Expression class.
 */
struct ExpressionClass {
	ObjectClass parent;
	int (*evaluate) (void *);
};

/**
 * Allocates and initializes a new Expression class of size "size".
 *
 * All fields are initialized except Object.name.
 *
 * This function is only useful to create a derivated class of Expression.
 */
ExpressionClass *
expression_class_allocate (size_t size, void *parent_class, char *name);

/**
 * Returns the Expression class.
 *
 * This function is only useful to create a derivated class of Expression.
 */
ExpressionClass *
expression_class_get ();

/**
 * Represents an instance of the Expression type.
 */
struct Expression {
	Object parent;
};

/**
 * Allocates a new Expression object.
 */
Expression *
expression_construct (size_t size, void *klass);

/**
 * Allocates and initializes a new Expression object.
 */
static inline Expression *
expression_new ()
{
	return expression_construct (sizeof (Expression), (void *) expression_class_get ());
}

/**
 * Evaluates an Expression.
 */
static inline int
expression_evaluate (void *self)
{
	assert (self);

	return EXPRESSION_CLASS (object_get_class (self))->evaluate (self);
}

#endif

