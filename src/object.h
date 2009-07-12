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

#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>
#include <stdlib.h>

#include "assert.h"

/**
 * This macro casts the pointer to "Object *".
 */
#define OBJECT(pointer) ((Object *) pointer)

/**
 * This macro casts the pointer to "ObjectClass *".
 */
#define OBJECT_CLASS(pointer) ((ObjectClass *) pointer)

typedef struct Object Object;
typedef struct ObjectClass ObjectClass;

/**
 * Represents the Object class or an Object-based class.
 */
struct ObjectClass {
	/**
	 * An owned reference to the parent class (must not be NULL, except for the
	 * Object class).
	 */
	void *parent;

	/**
	 * The name of this class (must not be NULL).
	 */
	char *name;

	/**
	 * This counter indicates the number of owned references to this class.
	 * When it reaches zero, the class is automatically deallocated.
	 */
	unsigned int ref_count;

	/**
	 * This virtual method is called when a object of this class is about to be
	 * destroyed (must not be NULL).
	 *
	 * If you implement it, you should (must) at the end of it call the parent
	 * finalize method : OBJECT_CLASS_GET_PARENT (klass)->finalize (self);
	 */
	void (*finalize) (void *);

	/**
	 * This method is called when the class is about to be deallocated (may be
	 * NULL).
	 *
	 * Contrary to the previous one, you should not call the parent method.
	 */
	void (*finalize_class) (void *);
};

/**
 * Allocates and initializes a new Object-based class of size "size" with name
 * "name".
 *
 * This function is only useful to create a Object-based class.
 *
 * @param size   The size of the structure of the class to allocate (must be
 *               greater or equal to "sizeof (ArrayClass)".
 * @param parent An owned reference to the parent class.
 * @param name   The name of the class (must not be NULL).
 *
 * @return The new allocated memory with all fields filled.
 */
ObjectClass *
object_class_allocate (size_t size, void *parent, char *name);

/**
 * Returns an owned reference the Object class.
 *
 * When no longer needed, the reference should be unreferenced by calling
 * "object_class_unref (void *)".
 *
 * This function is only useful to create a Object-based class.
 *
 * @return The reference.
 */
ObjectClass *
object_class_get ();

/**
 * Returns the name of the class.
 *
 * @param klass The class (must not be NULL).
 *
 * @return The class name.
 */
static inline const char *
object_class_get_name (const void *klass);

/**
 * Returns an unowned reference to the parent class of the class.
 *
 * @param klass The class (must not be NULL).
 *
 * @return The parent name.
 */
static inline void *
object_class_get_parent (const void *klass);

/**
 * Returns true if "klass" is a reference to the class called "name" or a
 * subclass of it.
 *
 * @param klass The klass to check (must not be NULL);
 * @param name  The name of the class.
 *
 * @return True if yes, else false.
 */
bool
object_class_is_a (const void *klass, const void *name);

/**
 * Registers a new reference to this class.
 *
 * @param klass The class (must not be NULL).
 *
 * @return The same class.
 */
static inline void *
object_class_ref (void *klass);

/**
 * Unregisters a reference of this class. If there is no references left, the
 * class is freed.
 *
 * @param klass The class (must not be NULL).
 */
void
object_class_unref (void *klass);

/**
 * Represents an instance of the Object type.
 */
struct Object {
	/**
	 * An owned reference to the object's class.
	 */
	ObjectClass *klass;

	/**
	 * This counter indicates the number of owned references to this object.
	 * When it reaches zero, the object is automatically deallocated.
	 */
	unsigned int ref_count;
};

/**
 * Allocates a memory space of size "size" and initializes the Object instance.
 *
 * @param size  The memory space to allocate (greater or equal to
 *              "sizeof (Object)").
 * @param klass An owned reference to the class of this object (must not be
 *              NULL).
 *
 * @return An owned reference to the newly allocated Object.
 */
Object *
object_construct (size_t size, void *klass);

/**
 * Creates a new object.
 */
static inline Object *
object_new ();

/**
 * Returns an unowned reference to the class of the object.
 *
 * @param self The object (must not be NULL).
 *
 * @return The class.
 */
static inline void *
object_get_class (const void *self);

/**
 * Returns the class name of the object.
 *
 * @param self The object (must not be NULL).
 *
 * @return The class name.
 */
static inline const char *
object_get_class_name (const void *self);

/**
 * Returns true if "object" is a reference to an instance of the class called
 * "name" or a of a subclass of it.
 *
 * @param object The object to check (must not be NULL);
 * @param name   The name of the class.
 *
 * @return True if yes, else false.
 */
static inline bool
object_is_a (const void *object, const void *name);

/**
 * Registers a new reference to this object.
 *
 * @param self The object (must not be NULL).
 *
 * @return The same object.
 */
static inline void *
object_ref (void *self);

/**
 * Unregisters a reference of this object. If there is no references left, the
 * object is freed.
 *
 * @param self The object (must not be NULL).
 */
void
object_unref (void *self);


// Inline functions.

static inline const char *
object_class_get_name (const void *klass)
{
	assert (klass);

	return OBJECT_CLASS (klass)->name;
}

static inline void *
object_class_get_parent (const void *klass)
{
	assert (klass);

	return OBJECT_CLASS (klass)->parent;
}

static inline void *
object_class_ref (void *klass)
{
	assert (klass);
	assert (OBJECT_CLASS (klass)->ref_count);

	++(OBJECT_CLASS (klass)->ref_count);

	return klass;
}

static inline void *
object_get_class (const void *self)
{
	assert (self);

	return OBJECT (self)->klass;
}

static inline const char *
object_get_class_name (const void *self)
{
	assert (self);

	return object_class_get_name (object_get_class (self));
}

static inline Object *
object_new ()
{
	return object_construct (sizeof (Object), (void *) object_class_get ());
}

static inline bool
object_is_a (const void *object, const void *name)
{
	assert (object);

	return object_class_is_a (object_get_class (object), name);
}

static inline void *
object_ref (void *self)
{
	assert (self);
	assert (OBJECT (self)->ref_count);

	++(OBJECT (self)->ref_count);

	return self;
}

#endif

