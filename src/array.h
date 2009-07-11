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

#include <stdbool.h>
#include <stdlib.h>

#include "assert.h"
#include "object.h"

typedef struct Array Array;
typedef struct ArrayClass ArrayClass;

#define ARRAY(pointer) ((Array *) pointer)

#define ARRAY_CLASS(pointer) ((ArrayClass *) pointer)

#define ARRAY_GET_CLASS(pointer) (ARRAY_CLASS(OBJECT(pointer)->klass))

/**
 * Represents the Array class or an Array-based class.
 */
struct ArrayClass {
	ObjectClass parent;
};

/**
 * Allocates and initializes a new Array-based class of size "size" with name
 * "name".
 *
 * This function is only useful to create a Array-based class.
 *
 * @param size The size of the structure of the class to allocate (must be
 *             greater or equal to "sizeof (ArrayClass)".
 * @param parent_class An owned reference to the parent class.
 * @param name The name of the class (must not be NULL).
 * @return The new allocated memory with all fields filled.
 */
ArrayClass *
array_class_allocate (size_t size, void *parent_class, char *name);

/**
 * Returns an owned reference the Array class.
 *
 * When no longer needed, the reference should be unreferenced by calling
 * "object_class_unref (void *)".
 *
 * This function is only useful to create a Array-based class.
 *
 * @return The reference.
 */
ArrayClass *
array_class_get ();

/**
 * A function of this type is called when a non-NULL item is about to be
 * removed from the Array.
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
 * Allocates a memory space of size "size" and initializes the Array object.
 *
 * "size" must be greater or equal to "sizeof (Array)".
 */
Array *
array_construct (size_t size, void *klass, destroy_func_t destroy_func);

/**
 * Allocates and initializes a new Array object.
 *
 * Equivalent to "array_construct (sizeof (Array), array_class_get (), destroy_func)".
 *
 * @param destroy_func The function which will be called before removing any
 *                     non-NULL item from the Array, or NULL.
 * @return The new Array or NULL if there was an error.
 */
static inline Array *
array_new (destroy_func_t destroy_func);

/**
 * Appends a new item to the Array.
 *
 * @param self The Array.
 * @param item The item to add.
 */
void
array_append (void *self, void *item);

/**
 * Clears the Array (i.e. removes all the containing items).
 *
 * @param self The Array.
 */
void
array_clear (void *self);

/**
 * Increases the Array's capacity if necessary to ensure that it can hold
 * "capacity" items.
 *
 * @param self The Array.
 * @param capacity
 */
void
array_ensure_capacity (void *self, size_t capacity);

/**
 * Gets the item at the given index.
 *
 * @param self The Array.
 * @param index Index of the item to retrieve (must be lesser than the Array's
 *              length).
 * @return The item.
 */
static inline void *
array_get (const void *self, size_t index);

/**
 * Returns a plain C-array copy of the Array.
 *
 * If the dynamic allocation fails, or if there is no items in the Array and
 * "null_terminated" is false, NULL will be returned.
 *
 * The array should be freed when no longer needed.
 *
 * @param self The Array.
 * @param null_terminated If true, the array will have one more item: NULL.
 * @return The C-array.
 */
void **
array_get_array (const void *self, bool null_terminated);

/**
 * Returns the capacity of the Array.
 *
 * @param self The Array.
 * @return The number of items the Array can contain.
 */
static inline size_t
array_get_capacity (const void *self);

/**
 * Returns the size of the Array.
 *
 * @param self The Array.
 * @return The number of items the Array contains.
 */
static inline size_t
array_get_length (const void *self);

/**
 * Class method which returns true if "array" is NULL or if it references an
 * empty Array.
 *
 * @param array The reference.
 * @return True if "array" is NULL or references an empty Array.
 */
static inline size_t
array_is_empty (const void *array);

/**
 * Removes the item at index "index".
 *
 * @param self The Array.
 * @param index Index of the item to remove (must be lesser than the Array's
 *              length).
 */
void
array_remove_at (void *self, size_t index);

/**
 * Sets the item at the given index.
 *
 * @param self The Array.
 * @param index Index where to set the item (must be lesser than the Array's
 *              length).
 * @param item The item to set.
 */
void
array_set (void *self, size_t index, void *item);


// Inline functions:

static inline Array *
array_new (destroy_func_t destroy_func)
{
	return array_construct (sizeof (Array), (void *) array_class_get (), destroy_func);
}

static inline void *
array_get (const void *self, size_t index)
{
	assert (index < ARRAY (self)->length);

	return ARRAY (self)->array[index];
}

static inline size_t
array_get_length (const void *self)
{
	assert (self);

	return ARRAY (self)->length;
}

static inline size_t
array_get_capacity (const void *self)
{
	assert (self);

	return ARRAY (self)->capacity;
}

static inline size_t
array_is_empty (const void *array)
{
	return ( (array == NULL) || !(array_get_length (array)) );
}

#endif

