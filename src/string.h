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

#ifndef STRING_H
#define STRING_H

#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "object.h"

typedef struct String String;
typedef struct StringClass StringClass;

#define STRING(pointer) ((String *) pointer)

#define STRING_CLASS(pointer) ((StringClass *) pointer)

/**
 * Represents the String class or a String-based class.
 */
struct StringClass {
	ObjectClass parent;
};

/**
 * Allocates and initializes a new String-based class of size "size" with name
 * "name".
 *
 * This function is only useful to create a String-based class.
 *
 * @param size   The size of the structure of the class to allocate (must be
 *               greater or equal to "sizeof (StringClass)".
 * @param parent An owned reference to the parent class.
 * @param name   The name of the class (must not be NULL).
 *
 * @return The new allocated memory with all fields filled.
 */
StringClass *
string_class_allocate (size_t size, void *parent, char *name);

/**
 * Returns an owned reference the String class.
 *
 * When no longer needed, the reference should be unreferenced by calling
 * "object_class_unref (void *)".
 *
 * This function is only useful to create a String-based class.
 *
 * @return The reference.
 */
StringClass *
string_class_get ();

/**
 * Represents an instance of the String type.
 */
struct String {
	Object parent;

	/**
	 * The number of characters (trailing '\0' included) the String can contain.
	 */
	size_t capacity;

	/**
	 * The number of characters (trailing '\0' not included) the String
	 * contains.
	 */
	size_t length;

	/**
	 * The content.
	 */
	char *string;
};

/**
 * Allocates a memory space of size "size" and initializes the String object.
 *
 * @param size  The memory space to allocate (greater or equal to
 *              "sizeof (String)").
 * @param klass An owned reference to the class of this object (must not be
 *              NULL).
 * @param chars A '\0' terminated characters array which will be used to
 *              initialize the String.
 *
 * @return An owned reference to the newly allocated String.
 */
String *
string_construct (size_t size, void *klass, const char *chars);

/**
 * Allocates and initializes a new String object.
 *
 * @return An owned reference to the newly allocated String or NULL if there was
 *         an error.
 */
static inline String *
string_new ();

/**
 * Allocates and initializes a new String object from an array of characters.
 *
 * @param chars A '\0' terminated characters array which will be used to
 *              initialize the String.
 *
 * @return An owned reference to the newly allocated String or NULL if there was
 *         an error.
 */
static inline String *
string_new_with_chars (const char *chars);

/**
 * Appends "chars" to the String.
 *
 * @param self  The String.
 * @param chars The string to append (must not be NULL).
 */
static inline void
string_append (void *self, const char *chars);

/**
 * Appends the "n" first character of "chars" to the String.
 *
 * Just make sure there is no null characters in the first "n" or you will
 * encounter some problems.
 *
 * @param self  The String.
 * @param chars The string to append (must not be NULL).
 * @param n     The number of characters to append.
 */
void
string_append_n (void *self, const char *chars, size_t n);

/**
 * Appends "c" to the String.
 *
 * @param self The String.
 * @param c    The character to append (must not be '\0').
 */
void
string_append_char (void *self, char c);

/**
 * TODO: write doc
 */
static inline void
string_append_integer (void *self, int n, unsigned char base);

/**
 * Appends the content of "string" to the String.
 *
 * @param self   The String.
 * @param string The String to append (must not be null).
 */
static inline void
string_append_string (void *self, void *string);

/**
 * TODO: write doc
 */
static inline void
string_append_uinteger (void *self, unsigned int n, unsigned char base);

/**
 * Clears the String (i.e. sets its length to 0).
 *
 * @param self The String.
 */
static inline void
string_clear (void *self);

/**
 * Static method which concatenates a variable number of strings into @dest.
 *
 * If necessary, @dest is resized. If @dest is NULL, a new string will be
 * allocated via malloc.
 *
 * @param dest The destination string or NULL.
 * @param ...  The strings to concatenate to @dest, the last argument must be
 *             NULL.
 *
 * @return NULL if the concatenation failed, else a pointer to the destination
 *         string (it might have changed).
 **/
char *
string_concat (char *dest, ...);

/**
 * Increases the String's capacity if necessary to ensure that it can hold
 * "capacity" characters (including the trailing '\0').
 *
 * @param self     The String.
 * @param capacity The number of characters the String should be able to
 *                 contain.
 */
void
string_ensure_capacity (void *self, size_t capacity);

/**
 * TODO: write doc
 */
String *
string_from_integer (int n, unsigned char base);

/**
 * Returns the capacity of the String.
 *
 * @param self The String.
 *
 * @return The number of characters (including the trailing '\0') the String can
 *         contain.
 */
static inline size_t
string_get_capacity (const void *self);

/**
 * Gets the character at the given index.
 *
 * @param self  The String.
 * @param index Index of the character to retrieve (must be lesser than the
 *              String's length).
 *
 * @return The character.
 */
static inline char
string_get_char (const void *self, size_t index);

/**
 * Returns the content of the String.
 *
 *
 * @param self The String.
 *
 * @return The content.
 */
static inline const char *
string_get_chars (const void *self);

/**
 * Returns the size of the String.
 *
 * @param self The String.
 *
 * @return The number of characters (the trailing '\0' is not included) the
 *         String contains.
 */
static inline size_t
string_get_length (const void *self);

/**
 * Reverses the String.
 *
 * /!\: Does not work with multibyte encoded characters.
 *
 * @param self The String.
 */
void
string_reverse (void *self);

/**
 * Sets the character at the given index.
 *
 * @param self  The String.
 * @param index Index of the character to set (must be lesser than the String's
 *              length).
 * @param c     The character to set (must not be '\0').
 */
static inline void
string_set_char (void *self, size_t index, char c);

/**
 * Steals the content of the String.
 *
 * The String will be empty.
 *
 * @param self The String.
 *
 * @return The previous String's content or NULL if there wasn't.
 */
char *
string_steal (void *self);

/**
 * Reduces, if possible, the memory space used by the String.
 *
 * @param self The String.
 */
void
string_trim_size (void *self);

/**
 * Creates a new String which will contain the string representation
 * of the unsigned integer @n.
 *
 * @param n    The unsigned integer.
 * @param base The base with wich the unsigned integer should be
 *             written.
 *
 * @return The new String.
 */
String *
string_from_uinteger (unsigned int n, unsigned char base);

// Inline functions:

static inline String *
string_new ()
{
	return string_construct (sizeof (String), string_class_get (), NULL);
}

static inline String *
string_new_with_chars (const char *chars)
{
	return string_construct (sizeof (String), string_class_get (), chars);
}

static inline void
string_append (void *self, const char *chars)
{
	string_append_n (self, chars, strlen (chars));
}

static inline void
string_append_integer (void *self, int n, unsigned char base)
{
	assert (self);

	String *s = string_from_integer (n, base);
	string_append_string (self, s);

	object_unref (s);
}

static inline void
string_append_uinteger (void *self, unsigned int n, unsigned char base)
{
	assert (self);

	String *s = string_from_uinteger (n, base);
	string_append_string (self, s);

	object_unref (s);
}

static inline void
string_append_string (void *self, void *string)
{
	assert (string);

	string_append_n (self, string_get_chars (string), string_get_length (string));
}

static inline void
string_clear (void *self)
{
	assert (self);

	if (STRING (self)->length) // The string is allocated and not empty.
	{
		STRING (self)->length = 0;
		STRING (self)->string[0] = 0;
	}
}

static inline size_t
string_get_capacity (const void *self)
{
	assert (self);

	return STRING (self)->capacity;
}

static inline char
string_get_char (const void *self, size_t index)
{
	assert_cmpuint (index, <, STRING (self)->length);

	return STRING (self)->string[index];
}

static inline const char *
string_get_chars (const void *self)
{
	assert (self);

	if (STRING (self)->string)
	{
		return STRING (self)->string;
	}

	return "";
}

static inline size_t
string_get_length (const void *self)
{
	assert (self);

	return STRING (self)->length;
}

static inline void
string_set_char (void *self, size_t index, char c)
{
	assert (c);
	assert_cmpuint (index, <, STRING (self)->length);

	STRING (self)->string[index] = c;
}

#endif

