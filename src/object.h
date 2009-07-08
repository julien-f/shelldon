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

#define OBJECT_GET_CLASS(pointer) (OBJECT(pointer)->klass)

typedef struct Object Object;
typedef struct ObjectClass ObjectClass;

struct ObjectClass {
	char *name;
	void (*finalize) (Object *);
};

ObjectClass *
object_class_allocate (size_t size);

const ObjectClass *
object_class_get ();

void
object_class_initialize ();


struct Object {
	ObjectClass *klass;
	unsigned char ref_count;
};

Object *
object_allocate (size_t size);

void
object_initialize (Object *self);

Object *
object_new ();

const char *
object_get_class_name (const Object *self);

Object *
object_ref (Object *self);

void
object_unref (Object *self);

#endif

