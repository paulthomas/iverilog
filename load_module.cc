/*
 * Copyright (c) 2001 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */
#if !defined(WINNT)
#ident "$Id: load_module.cc,v 1.6 2002/05/28 00:50:39 steve Exp $"
#endif

# include  "config.h"
# include  "util.h"
# include  "parse_api.h"
# include  "compiler.h"
# include  <iostream.h>


const char dir_character = '/';
extern FILE *depend_file;

bool load_module(const char*type)
{
      char path[4096];

      for (list<const char*>::iterator cur = library_dirs.begin()
		 ; cur != library_dirs.end()
		 ; cur ++ ) {
	    for (list<const char*>::iterator suf = library_suff.begin()
		       ; suf != library_suff.end()
		       ; suf ++ ) {

		  sprintf(path, "%s%c%s%s", *cur, dir_character, type, *suf);

		  FILE*file = fopen(path, "r");
		  if (file == 0)
			continue;
		  if(depend_file) {
			  fprintf(depend_file, "%s\n", path);
		  }

		  if (ivlpp_string) {
			fclose(file);
			char*cmdline = (char*)malloc(strlen(ivlpp_string) +
						     strlen(path) + 2);
			strcpy(cmdline, ivlpp_string);
			strcat(cmdline, " ");
			strcat(cmdline, path);
			file = popen(cmdline, "r");

			if (verbose_flag)
			      cerr << "Executing: " << cmdline << endl;

			pform_parse(path, file);
			fclose(file);

		  } else {
			if (verbose_flag)
			      cerr << "Loading library file "
				   << path << "." << endl;

			pform_parse(path, file);

			fclose(file);
		  }

		  return true;
	    }
      }

      return false;
}

/*
 * $Log: load_module.cc,v $
 * Revision 1.6  2002/05/28 00:50:39  steve
 *  Add the ivl -C flag for bulk configuration
 *  from the driver, and use that to run library
 *  modules through the preprocessor.
 *
 * Revision 1.5  2002/04/04 05:26:13  steve
 *  Add dependency generation.
 *
 * Revision 1.4  2001/11/20 23:36:34  steve
 *  Close library files after parsing.
 *
 * Revision 1.3  2001/11/16 05:07:19  steve
 *  Add support for +libext+ in command files.
 *
 * Revision 1.2  2001/10/22 02:05:21  steve
 *  Handle activating tasks in another root.
 *
 * Revision 1.1  2001/10/20 23:02:40  steve
 *  Add automatic module libraries.
 *
 */

