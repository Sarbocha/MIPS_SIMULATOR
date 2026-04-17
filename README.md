What Everything is:

Assembly File → Parser → Instructions → CPU Pipeline → Output

Parser.h / Parsar.cpp is supposed to convert ADD $1, #2,$3 to something like
Instruction {
  opcode = "ADD",
  rs = 2,
  rt = 3,
  rd = 1,
}

Instruction.h defines what an instructiuon looks like

Memory.h simulates RAM, stores data and supports load(address) and store(address, value)

PipelineRegisters.h is the pipeline logic 
IF_ID
ID_EX
EX_MEM
MEM_WB

CPU.h and CPU.cpp 
the actual processor and the main loop while(not finished)

instruction fetch - instruction decode - execute - memory - write back
