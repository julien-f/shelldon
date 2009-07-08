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

#include "object.h"

typedef struct Expression Expression;
typedef struct ExpressionClass ExpressionClass;

#define EXPRESSION(pointer) ((Expression *) pointer)

#define EXPRESSION_CLASS(pointer) ((ExpressionClass *) pointer)

#define EXPRESSION_GET_CLASS(pointer) (EXPRESSION_CLASS(OBJECT(pointer)->klass))

/**
 * Represents an expression class.
 */
struct ExpressionClass {
	ObjectClass parent;
	int (*evaluate) (Expression *);
};

/**
 * Allocates and initializes a new Expression class of size "size".
 *
 * All fields are initialized except Object.name.
 *
 * This function is only useful to create a derivated class of Expression.
 */
ExpressionClass *
expression_class_allocate (size_t size);

/**
 * Returns the Expression class.
 *
 * This function is only useful to create a derivated class of Expression.
 */
const ExpressionClass *
expression_class_get ();

/**
 * Allocates and initialized the Expression class.
 *
 * This function must be called before any use of the class Expression.
 */
void
expression_class_initialize ();

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
expression_allocate (size_t size);

/**
 * Initializes a new Expression object.
 */
void
expression_initialize (Expression *expression);

/**
 * Allocates and initializes a new Expression object.
 */
Expression *
expression_new ();

/**
 * Evaluates an Expression.
 */
int
expression_evaluate (Expression *expression);

#endif

