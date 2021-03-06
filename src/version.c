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

#include <stdio.h>

#include "version.h"

void
print_version(void)
{
	#if defined(__DATE__) && defined(__TIME__)
		printf(prog_name " v" prog_version " aka \"" prog_version_name "\" "
				"(built on " __DATE__ ", " __TIME__ ")\n");
	#else
		printf(prog_name " v" prog_version " aka \"" prog_version_name "\"\n");
	#endif
}

