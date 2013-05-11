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

#ifndef SHELLDON_VERSION_H
#define SHELLDON_VERSION_H

#define prog_name "Shelldon"

#define prog_version "0.11a"
#define prog_version_name "Ocean's"

/**
 * TODO: write help.
 **/
#define get_prog_name() prog_name

/**
 * TODO: write help.
 **/
#define get_prog_version() prog_version

/**
 * TODO: write help.
 **/
#define get_prog_version_name() prog_version_name

/**
 * Prints version.
 **/
void
print_version(void);

#endif

