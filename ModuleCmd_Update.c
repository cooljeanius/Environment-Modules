/*****
 ** ** Module Header ******************************************************* **
 ** 									     **
 **   Modules Revision 3.0						     **
 **   Providing a flexible user environment				     **
 ** 									     **
 **   File:		ModuleCmd_Update.c				     **
 **   First Edition:	1991/10/23					     **
 ** 									     **
 **   Authors:	John Furlan, jlf@behere.com				     **
 **		Jens Hamisch, jens@Strawberry.COM			     **
 ** 									     **
 **   Description: 	Uses the beginning environment stored the first time **
 **			each login session to reload all of the currently    **
 ** 									     **
 **   Notes:								     **
 ** 									     **
 ** ************************************************************************ **
 ****/

/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 1991-1994 by John L. Furlan.                      	     **
 ** see LICENSE.GPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

static char Id[] = "@(#)$Id: 1286756fe8af52b423fea50510cbfd76ead170e6 $";
static void *UseId[] = { &UseId, Id };

/** ************************************************************************ **/
/** 				      HEADERS				     **/
/** ************************************************************************ **/

#include "modules_def.h"

/** ************************************************************************ **/
/** 				  LOCAL DATATYPES			     **/
/** ************************************************************************ **/

/** not applicable **/

/** ************************************************************************ **/
/** 				     CONSTANTS				     **/
/** ************************************************************************ **/

#define	UPD_BUFSIZE	2048

/** ************************************************************************ **/
/**				      MACROS				     **/
/** ************************************************************************ **/

/** not applicable **/

/** ************************************************************************ **/
/** 				    LOCAL DATA				     **/
/** ************************************************************************ **/

static	char	module_name[] = "ModuleCmd_Update.c";	/** File name of this module **/
#if WITH_DEBUGGING_MODULECMD
static	char	_proc_ModuleCmd_Update[] = "ModuleCmd_Update";
#endif

/** ************************************************************************ **/
/**				    PROTOTYPES				     **/
/** ************************************************************************ **/

/** not applicable **/


/*++++
 ** ** Function-Header ***************************************************** **
 ** 									     **
 **   Function:		ModuleCmd_Update				     **
 ** 									     **
 **   Description:	Execution of the module-command 'update'	     **
 ** 									     **
 **   First Edition:	1991/10/23					     **
 ** 									     **
 **   Parameters:	Tcl_Interp	*interp		Attached Tcl Interp. **
 **			int		 argc		Number of arguments  **
 **			char 		*argv[]		Argument list	     **
 ** 									     **
 **   Result:		int	TCL_ERROR	Failure			     **
 **				TCL_OK		Successful operation	     **
 ** 									     **
 **   Attached Globals:	flags		Controls the callback functions      **
 ** 									     **
 ** ************************************************************************ **
 ++++*/

int	ModuleCmd_Update(Tcl_Interp	*interp, int count, char *module_list[])
{
#ifdef  BEGINENV
    char *buf,			/** Read buffer			     **/
		 *var_ptr,		/** Pointer to the name	of a variable     **/
		 *val_ptr,		/** Pointer to the value of a variable    **/
		**load_list,		/** List of loaded modules	     **/
		 *tmpload,		/** LOADEDMODULES contents	     **/
		 *loaded,		/** Buffer for tokenization	     **/
		 *filename;		/** The name of the file, where the beginning
						 ** environment resides    **/
    FILE	 *file;			/** Handle to read in a file	     **/
    int	  list_count = 0,
		  maxlist = 16,		/** Max. number of list entries	     **/
		  buffer_size = UPD_BUFSIZE;
    					/** Current size of the input buffer **/
    char	 *ptr;	/** Read pointer */
	char	 c;		/** char buffer (i.e. the last character seen) (declare it
					 ** separately to make it clearer that it is NOT also a
					 ** pointer) **/
	/* some additional loop counters: */
	unsigned long inner_loop_counter, outer_loop_counter;
	inner_loop_counter = 0;
	outer_loop_counter = 0;

#if WITH_DEBUGGING_MODULECMD
    ErrorLogger(NO_ERR_START, LOC, _proc_ModuleCmd_Update, NULL);
#endif /* WITH_DEBUGGING_MODULECMD */

#if BEGINENV == 99
	if (!Tcl_GetVar2(interp, "env", "MODULESBEGINENV", TCL_GLOBAL_ONLY)) {
		ErrorLogger(ERR_BEGINENVX, LOC, NULL);
		return (TCL_ERROR);	/** -------- EXIT (FAILURE) -------> **/
	}
#endif /* BEGINENV == 99 */
    /**
     **  Nothing loaded so far - we should be ready!
     **/

    if (!(tmpload = (char *)getenv("LOADEDMODULES"))) {
		if (OK != ErrorLogger(ERR_MODULE_PATH, LOC, NULL)) {
			goto unwind0;
		} else {
			goto success0;
		}
    }

    /**
     **  First I shall update the environment with what is/was stored to
	 ** _MODULESBEGINENV_:
     **/
    filename = EMGetEnv(interp,"_MODULESBEGINENV_");
    if (filename && *filename) {
		/**
		 **  Read the begining environment:
		 **/
		if (NULL != (file = fopen(filename, "r"))) {
			if ((char *)NULL == (buf = stringer(NULL, buffer_size, NULL))) {
				if (OK != ErrorLogger(ERR_STRING, LOC, NULL)) {
					goto unwind0;
				}
			}

			while (!feof(file)) {
				outer_loop_counter++;
				/**
				 **  Trigger to entries of the type
				 **    <variable> = <value>
				 **/
				ptr = buf;
				while (!feof(file)) {
					inner_loop_counter++;
					if ((ptr - buf) >= (buffer_size - 10)) { /* 10 bytes safety */
						null_free((void *)&buf);
						if ((char *)NULL == (buf = stringer(NULL,
															(buffer_size += UPD_BUFSIZE),
															NULL))) {
							if (OK != ErrorLogger(ERR_STRING, LOC, NULL)) {
								goto unwind0;
							}
						}
					}

					/**
					 **  Read a character and put it into the read buffer. Check
					 **  for the lines (CR) or a terminator character ...
					 **/
					if ('\n' == (*ptr++ = c = (char)fgetc(file))) {
						*ptr++ = c = '\0';
						break;
					}

					if (!c) {
						break;
					}
				} /** end inner while-loop **/

				/* deal with case where EOF was encountered immediately,
				 * resulting in loop running 0 times: */
				if (inner_loop_counter == 0) {
					/* this would never have gotten initialized in this case
					 * if we had not done it here: */
					c = '\0';
				}

				/**
				 **  If there has NOT been a null terminator so far (it should
				 **  have been stored in 'c' if there had been one), then force
				 **  a null terminator to be put at the end of the line pointed
				 **  to by 'ptr'. Therefore we should have left a safety space
				 **  at the end of the buffer ;-)
				 **/
				if (c && (c != '\0')) {
					*ptr++ = '\0';
					/* dummy condition to use value stored to 'ptr': */
					if (ptr != NULL) {
						;
					}
				}

				/**
				 **  Now let us evaluate the read line:
				 **/
				/* 'var_ptr' does not appear to have been initialized yet, so
				 * assume that the '=' is correct: */
				if ((var_ptr = strchr(buf, '='))) {
					*var_ptr = '\0';
					val_ptr = (var_ptr + 1);
					var_ptr = buf;

					/**
					 **  Reset the environment to the values derivered from the
					 **  _MODULESBEGINENV_.
					 **  Do not change the LOADEDMODULES variable ;-)
					 **  Do not change the TCL_LIBRARY and TK_LIBRARY also.
					 **/
					if (strncmp(var_ptr, "LOADEDMODULES", 12) &&
						strncmp(var_ptr, "TCL_LIBRARY", 10 ) &&
						strncmp(var_ptr, "TK_LIBRARY", 9 )) {
						if (!strncmp(var_ptr, "MODULEPATH", 10)) {
							moduleSetenv(interp, var_ptr, val_ptr, 1);
						} else {
							EMSetEnv(interp, var_ptr, val_ptr);
						}
					} /* end "if (var_ptr)" (the one with strncmp() in it) **/
				} /** end "if (var_ptr)" (the one with strchr() in it) **/
			} /** end outer while-loop **/

			/**
			 **  Close the _MODULESBEGINENV_ file anf free up the read buffer.
			 **/
			null_free((void *)&buf);

			if (EOF == fclose(file)) {
				if (OK != ErrorLogger(ERR_CLOSE, LOC, filename, NULL)) {
					goto unwind0;
				}
			}
		} else { /** if( fopen) **/
			if (OK != ErrorLogger(ERR_OPEN, LOC, filename, "reading", NULL)) {
				goto unwind0;
			}
		} /** end "if (fopen)" **/
    } /** end "if (filename)" **/

    /**
     **  Allocate memory for a buffer to tokenize the list of loaded modules
     **  and a list buffer:
     **/
    if (NULL == (load_list = (char**)module_malloc((size_t)((unsigned long)maxlist * sizeof(char**))))) {
		if (OK != ErrorLogger(ERR_ALLOC, LOC, NULL)) {
			goto unwind0;
		}
	}


    if (NULL == (loaded = stringer(NULL, 0, tmpload, NULL))) {
		if (OK != ErrorLogger(ERR_STRING, LOC, NULL)) {
			goto unwind1;
		}
	}

    /**
     **  Tokenize and build the list
     **/
    if (*loaded) {
		for ((load_list[list_count++] = xstrtok(loaded, ":"));
			 load_list[(list_count - 1)];
			 (load_list[list_count++] = xstrtok(NULL, ":"))) {
			/**
			 **  Conditionally we have to double the space that we have
			 **  allocated for the list:
			 **/
            if (list_count >= maxlist) {
                maxlist = (maxlist << 1);

                if (!(load_list = (char**)module_realloc((char *)load_list,
														 (size_t)((unsigned long)maxlist * sizeof(char**))))) {
					if (OK != ErrorLogger(ERR_ALLOC, LOC, NULL)) {
						goto unwind1;
					}
				}
            } /** end "if (maxlist)" **/
		} /** end for-loop **/

		/**
		 **  Load all the modules in the list:
		 **/
        ModuleCmd_Load(interp, 1, list_count, load_list);
    }

    /**
     **  Free up what has been allocated and return on success
     **/
    null_free((void *)&loaded);
    null_free((void *)&load_list);

success0:
#if WITH_DEBUGGING_MODULECMD
    ErrorLogger(NO_ERR_END, LOC, _proc_ModuleCmd_Update, NULL);
#endif /* WITH_DEBUGGING_MODULECMD */

    return (TCL_OK);			/** -------- EXIT (SUCCESS) -------> **/

unwind1:
    null_free((void *)&load_list);
unwind0:
    null_free((void *)&filename);
#else	/* BEGINENV not defined: */
	ErrorLogger(ERR_BEGINENV, LOC, NULL);
#endif	/* BEGINENV */
    return (TCL_ERROR);			/** -------- EXIT (FAILURE) -------> **/

} /** End of 'ModuleCmd_Update' **/

/* EOF */
