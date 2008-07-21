/**
** @file flow.c
** @ingroup libstderesi
** Original author : sk for the modflow of elfsh
**
** @brief Merged in the ELFsh VM by the ELFsh crew.
**
**
** $Id: flow.c,v 1.2 2008-02-16 12:32:27 thor Exp $
**
*/
#include "libstderesi.h"


/*****************************************************************************
 * High level block analysis.
 *
 * this function builds a linked list of function
 * which each contains a list linked of blocks
 *
 * for each block, 
 *
 * until a new block starting a function, current blocks
 * are supposed to be part of current function
 *
 ************************************************************************/


void	elfsh_help() 
{
  printf(" flow                                     \n"
	 "  Launch control flow analysis.           \n"
	 "                                          \n"
	 " graph       <file> <min>  {+}<max>       \n"
	 "   Dump graph in file from starting       \n"
	 "    address min to ending address <max>   \n"
	 "   ie: graph debfunc.gvz func +200        \n"
	 "   ie: graph 0x805de40 0x805ff00          \n"
	 "                                          \n"
	 " inspect  <symbol/address>                \n"
	 "                                          \n"
	 " flowjack <oldsymbol> <newsymbol>         \n"
	 "     All relative call to oldsymbol       \n"
	 "     are hijacked to newsymbol            \n"
	 "                                          \n"
	 " addgoto <vaddr> <func_addr>              \n"
	 "     'redirect' for functions called      \n"
	 "     trough a pointer                     \n"
	 "                                          \n");
}


/** 
 * Manually add function pointer information 
 */
int	cmd_addgoto(void)
{
  if (!world.curjob->curcmd->param[0] || !world.curjob->curcmd->param[1])
    return (-1);
  
  printf(" [*] at %s goto %s\n",
	 world.curjob->curcmd->param[0],
	 world.curjob->curcmd->param[1]);
  
  hash_del(&goto_hash, world.curjob->curcmd->param[0]);
  hash_add(&goto_hash, world.curjob->curcmd->param[0],
	   world.curjob->curcmd->param[1]);
  return (0);
}

/**
 * Print the control information 
 */
int		cmd_control()
{
  elfshsect_t	*sect;
  elfshobj_t	*current;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);  
  current = world.curjob->curfile;
  sect = elfsh_get_section_by_name(current, 
				   ELFSH_SECTION_NAME_EDFMT_BLOCKS,
				   0, 0, 0);
  if (!sect || !hash_size(&world.mjr_session.cur->blkhash))
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__,
                      "No control flow section found", -1);

  mjr_blocks_display(world.mjr_session.cur, 1);
  revm_output("\n [*] Control flow information dumped \n\n");
  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);  
}


/* Change the graph verbose level */
int	cmd_setgvl(void) 
{
  world.state.revm_gvl = atoi(world.curjob->curcmd->param[0]);
  return (0);
}