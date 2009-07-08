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

#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

#include "object.h"

typedef struct Array Array;
typedef struct ArrayClass ArrayClass;

#define ARRAY(pointer) ((Array *) pointer)

#define ARRAY_CLASS(pointer) ((ArrayClass *) pointer)

#define ARRAY_GET_CLASS(pointer) (ARRAY_CLASS(OBJECT(pointer)->klass))

/**
 * Represents an expression class.
 */
struct ArrayClass {
	ObjectClass parent;
};

/**
 * Allocates and initializes a new Expression class of size "size".
 *
 * All fields are initialized except Object.name.
 *
 * This function is only useful to create a derivated class of Expression.
 */
ArrayClass *
array_class_allocate (size_t size);

/**
 * Returns the Expression class.
 *
 * This function is only useful to create a derivated class of Expression.
 */
const ArrayClass *
array_class_get ();

/**
 * Allocates and initialized the Array class.
 *
 * This function must be called before any use of the class Array.
 */
void
array_class_initialize ();

/**
 * A function of this type is called when an item of the array is about to be
 * removed from the array.
 */
typedef void (*destroy_func_t) (void *item);

/**
 * Represents an instance of the Array type.
 */
struct Array {
	Object parent;

	size_t capacity;
	size_t length;
	void **array;
	destroy_func_t destroy_func;
};

/**
 * Allocates a new Array object.
 */
Array *
array_allocate (size_t size);

/**
 * Initializes a new Array object.
 */
void
array_initialize (Array *self, destroy_func_t destroy_func);

/**
 * Allocates and initializes a new Array object.
 */
Array *
array_new (destroy_func_t destroy_func);

/**
 * Returns the size of the array.
 */
size_t
array_get_length (Array *self);

/**
 * Gets the item at the given index.
 */
void *
array_get (Array *self, size_t index);

/**
 * Adds a new item at the end of the array.
 */
void
array_add (Array *self, void *item);

/**
 * Sets the item at the given index.
 */
void
array_set (Array *self, size_t index, void *item);

/**
 * Clear the array (i.e. remove all the containing items.
 */
void
array_clear (Array *self);

#endif

