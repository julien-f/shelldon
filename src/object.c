#include "object.h"

#include <stdlib.h>

#include "assert.h"

static void
object_real_finalize (Object *object);

static ObjectClass *klass = NULL;

ObjectClass *
object_class_allocate (size_t size)
{
	assert (size >= sizeof (ObjectClass));

	ObjectClass *object_class = (ObjectClass *) malloc (size);
	if (!object_class) // Allocation failed
	{
		return NULL;
	}

	object_class->name = NULL;
	object_class->finalize = object_real_finalize;

	return object_class;
}

const ObjectClass *
object_class_get ()
{
	if (!klass) // The Object class is not yet initalized.
	{
		object_class_initialize ();
	}
	return klass;
}

void
object_class_initialize ()
{
	if (klass) // Already initialized
	{
		return;
	}

	klass = object_class_allocate (sizeof (ObjectClass));
	if (klass) // Allocation succeed.
	{
		klass->name = "Object";
	}
}

Object *
object_allocate (size_t size)
{
	assert (size >= sizeof (Object));

	Object *object = (Object *) malloc (size);
	return object;
}

void
object_initialize (Object *self)
{
	assert (self);

	self->ref_count = 1;
}

Object *
object_new () {
	Object *object = object_allocate (sizeof (Object));
	if (object) // Allocation failed.
	{
		return NULL;
	}

	object_initialize (object);
	object->klass = klass;
	return object;
}

const char *
object_get_class_name (const Object *self)
{
	assert (self);

	return OBJECT_GET_CLASS (self)->name;
}

Object *
object_ref (Object *self)
{
	assert (self);
	assert (self->ref_count > 0);

	++(self->ref_count);

	return self;
}

void
object_unref (Object *self)
{
	assert (self);
	assert (self->ref_count > 0);

	--(self->ref_count);

	if (self->ref_count == 0)
	{
		OBJECT_GET_CLASS (self)->finalize (self);

		free (self);
	}
}

static void
object_real_finalize (Object *self)
{}

