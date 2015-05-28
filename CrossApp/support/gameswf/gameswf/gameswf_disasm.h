// gameswf_disasm.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "../base/container.h"
#include "gameswf_abc.h"

namespace gameswf
{

	// Disassemble one instruction to the log.
	void	log_disasm(const unsigned char* instruction_data);

	// Disassemble one instruction to the log, AVM2
	void	log_disasm_avm2(const membuf& code, const abc_def* def);
	int read_vu30(int& result, const Uint8* args);
}
