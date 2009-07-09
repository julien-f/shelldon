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

#include <stdlib.h>

#define OBJECT(pointer) ((Object *) pointer)

#define OBJECT_CLASS(pointer) ((ObjectClass *) pointer)

#define OBJECT_CLASS_GET_PARENT(pointer) (OBJECT_CLASS (OBJECT_CLASS (pointer)->parent_class))

#define OBJECT_GET_CLASS(pointer) (OBJECT(pointer)->klass)

//#define OBJECT_GET_PARENT_CLASS(pointer) (OBJECT_CLASS_GET_PARENT (OBJECT_GET_CLASS (pointer)))

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
	void *parent_class;

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
 * @param size The size of the structure of the class to allocate (must be
 *             greater or equal to "sizeof (ArrayClass)".
 * @param parent_class An owned reference to the parent class.
 * @param name The name of the class (must not be NULL).
 * @return The new allocated memory with all fields filled.
 */
ObjectClass *
object_class_allocate (size_t size, void *parent_class, char *name);

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
 * Registers a new reference to this class.
 *
 * @param klass The class (must not be null).
 * @return The same class.
 */
void *
object_class_ref (void *klass);

/**
 * Unregisters a reference of this class. If there is no references left, the
 * class is freed.
 *
 * @param klass The class (must not be null).
 */
void
object_class_unref (void *klass);

/**
 * Represents an instance of the Object type.
 */
struct Object {
	ObjectClass *klass;
	unsigned int ref_count;
};

/**
 * Allocates a memory space of size "size" and initializes the object.
 *
 * "size" must be greater or equal to "sizeof (Object)".
 */
Object *
object_construct (size_t size, void *klass);

/**
 * Creates a new object.
 *
 * Equivalent to "object_construct (sizeof (Object), object_class_get ())".
 */
static inline Object *
object_new ()
{
	return object_construct (sizeof (Object), (void *) object_class_get ());
}

/**
 * Returns the class name of the object.
 *
 * @param self The object (must not be null).
 * @return The class name.
 */
const char *
object_get_class_name (const void *self);

/**
 * Registers a new reference to this object.
 *
 * @param self The object (must not be null).
 * @return The same object.
 */
void *
object_ref (void *self);

/**
 * Unregisters a reference of this object. If there is no references left, the
 * object is freed.
 *
 * @param self The object (must not be null).
 */
void
object_unref (void *self);

#endif

