#include "include/opcodeHandler.h"
#include "include/memory.h"
#include <cstdlib>
#include <ctime>

OpcodeHandler::OpcodeHandler(std::FILE *debugFile)
{
    // Debugging logs
    myDebugFile = debugFile;
    
    // Initializing the PC to position 512, where the instructions start.
    pc = 0x0200;
    // Setting pseudo-random generator seed to current time
    srand (time(nullptr));
    // Setting sprite location address array
    for (unsigned short i = 0; i < 16; i++)
    {
        spriteAddr[i] = 0x0050 + 5*i; 
    }
}

OpcodeHandler::~OpcodeHandler()
{
    //Empty
}

bool OpcodeHandler::readNxtInstr(Memory *mem)
{
    // Getting the nxt Instruction into opcode
    unsigned char upperOpcode = mem->getMemData(pc);
    unsigned char lowerOpcode = mem->getMemData(pc+1);
    opcode = (upperOpcode << 8) | (lowerOpcode << 0);
    pc = pc + 2;

    // Printing details to debugFile
    std::fprintf(myDebugFile,"[I] [PC: %hx]<opcodeHandler.cpp>::Next Instruction is: %hx\n",
    pc - 2, opcode);
    return true;
}

bool OpcodeHandler::emulateInstr(Memory *mem)
{
    /* \todo List of instructions left to complete
    0NNN    Call
    00E0    Display
    DXYN    Disp
    EX9E    KeyOp
    EXA1    KeyOp
    FX07    Timer
    FX0A    KeyOp
    FX15    Timer
    FX18    Sound
    */
    // Data for instructions
    unsigned char regX = 0x00;
    unsigned char regY = 0x00;
    unsigned char NNData = 0x00; //Stored 0xDDNN Last to hex data if needed
    unsigned short addr = 0x0000; //Used for storing memory address
    switch (opcode & 0xF000)
    {
    case 0x0000 : // 3 Instructions: 0xNNN; 0x0E0 and 0x00EE
        switch (opcode)
        {
        case 0x00EE : // 0x00EE Return from subroutine
            sptr = (sptr - 1) % 16;
            pc = stack[sptr];
            std::fprintf(myDebugFile,
            "[I] [PC: %hx]<opcodeHandler.cpp>::Return from subroutine. PC updated to [%hx] &"
            " stack pointer updated to [%d]\n",
             pc,
             pc, sptr);  
            break;

        default:
            std::fprintf(myDebugFile,
            "[E] [PC: %hx]<opcodeHandler.cpp>::Unknown Opcode: %hx\n",
            pc,
            opcode);
            return false;
            break;
        }
        break;

    case 0x1000 : // 0x1NNN: pc = NNN
        pc = (0x0FFF & opcode);
        std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::PC updated to [%hx]\n",
            pc);
        break;

    case 0x2000 : //0x2NNN: call subroutine at NNN
        stack[sptr] = pc;
        sptr = (sptr + 1) % 16;
        pc = (0x0FFF & opcode);
        std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Call subroutine. PC updated to [%hx] &"
            " stack pointer updated to [%d]\n",
             pc, sptr);
        break;
    
    case 0x3000 : // 0x3XNN: if(Vx == NN) skip next instruction
        regX = ((opcode & 0x0F00) >> 8);
        NNData = (opcode & 0x00FF);
        if (NNData == reg[regX])
        {
            pc = pc + 2;
            std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], NN Val: [%hhx]\n",
             pc, regX, reg[regX], NNData);
        }
        else
        {
            std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Not Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], NN Val: [%hhx]\n",
             pc, regX ,reg[regX], NNData);
        }
        break;

    case 0x4000 : // 0x4XNN: Skip nxt instr if (Vx!=NN)
        regX = ((opcode & 0x0F00) >> 8);
        NNData = (opcode & 0x00FF);
        if (NNData != reg[regX])
        {
            pc = pc + 2;
            std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], NN Val: [%hhx]\n",
             pc, regX, reg[regX], NNData);
        }
        else
        {
            std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Not Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], NN Val: [%hhx]\n",
             pc, regX, reg[regX], NNData);
        }
        break;

    case 0x5000 : // 0x5XY0: Skip nxt instr if (Vx==Vy)
        regX = ((opcode & 0x0F00) >> 8);
        regY = ((opcode & 0x00F0) >> 4);
        if (reg[regY] == reg[regX])
        {
            pc = pc + 2;
            std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], regY Val: [%hhx]\n",
             pc, regX, reg[regX], reg[regY]);
        }
        else
        {
            std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Not Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], regY Val: [%hhx]\n",
             pc, regX ,reg[regX], reg[regY]);
        }
        break;

    case 0x6000 : // 0x6XNN: Vx = NN
        regX = ((opcode & 0x0F00) >> 8);
        reg[regX] = (opcode & 0x00FF);
        // Printing info
        std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Register [%hhx] updated to val [%hhx].\n",
            regX, reg[regX]);
        break;

    case 0x7000 : // 0x7XNN: Vx += NN
        regX = ((opcode & 0x0F00) >> 8);
        reg[regX] += (opcode & 0x00FF);
        std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Register [%hhx] updated to val [%hhx].\n",
            regX, reg[regX]);
        break;

    case 0x8000 : // Doing All operations related to Arthimetic
        regX = ((opcode & 0x0F00) >> 8);
        regY = ((opcode & 0x00F0) >> 4);
        // Identifying and doing operations
        switch ((opcode & 0x000F))
        {
        case 0x00 : // Assign: Vx = Vy
            reg[regX] = reg[regY];
            break;

        case 0x01: //Bitwise OR: Vx |= Vy
            reg[regX] |= reg[regY];
            break;

        case 0x02: //Bitwise AND: Vx &= Vy
            reg[regX] &= reg[regY];
            break;

        case 0x03: //Bitwise XOR: Vx ^= Vy
            reg[regX] ^= reg[regY];
            break;

        case 0x04: // Add Vx += Vy. And Carry Flag VF
            if (reg[regX] > (0xFF - reg[regY]))
            {// Setting Carry Flag
                reg[0x0F] = 0x01;
            }
            else
            {// Not setting carry flag
                reg[0x0F] = 0x00;
            }
            reg[regX] += reg[regY];
            break;

        case 0x05: // Sub Vx -= Vy. VF 1 when no borrow
            if (reg[regX] >= reg[regY])
            {// This is no borrow
                reg[0x0F] = 0x01;
            }
            else
            {// There is borrow
                reg[0x0F] = 0x00;
            }
            reg[regX] -= reg[regY];
            break;

        case 0x06: // Vx>>=1 and VF is LSB of Vx
            reg[0x0F] = (reg[regX] && 0x01);
            reg[regX] = (reg[regX] >> 1);
            break;

        case 0x07: //Sub Vx = Vy-Vx. VF 1 when no borrow
            if (reg[regY] >= reg[regX])
            {// No borrow
                reg[0x0F] = 0x01;
            }
            else
            {// Borrow
                reg[0x0F] = 0x00;
            }
            reg[regX] = reg[regY] - reg[regX];
            break;

        case 0x0E: // Vx <<=1 & VF is MSB of Vx
            reg[0x0F] = ((reg[regX] & 0x80) >> 7);
            reg[regX] = (reg[regX] << 1);
            break;

        default:
            std::fprintf(myDebugFile,"[E] <opcodeHandler.cpp>::Unknown Opcode: %hx\n",
            opcode);
            return false;
            break;
        }
        // Printing info
        std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Register [%hhx] updated to val [%hhx].\n",
            regX, reg[regX]);
        std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::VF val is [%hhx].\n",
            reg[0x0F]);
        break;

    case 0x9000 : // 0x9XY0: Skip nxt instr if (Vx != Vy)
        regX = ((opcode & 0x0F00) >> 8);
        regY = ((opcode & 0x00F0) >> 4);
        if (reg[regY] != reg[regX])
        {
            pc = pc + 2;
            std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], regY Val: [%hhx]\n",
             pc, regX, reg[regX], reg[regY]);
        }
        else
        {
            std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Not Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], regY Val: [%hhx]\n",
             pc, regX, reg[regX], reg[regY]);
        }
        break;

    case 0xA000 : // 0xANNN: I = NNN
        IReg = (opcode & 0x0FFF);
        std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Register I updated to [%hx]\n",
             IReg);
        break;

    case 0xB000 : // 0xBNNN: PC=V[0]+NNN
        pc = reg[0x00] + (opcode & 0x0FFF);
        std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::PC updated to [%hx]\n",
             pc);
        break;

    case 0xC000: //0xCXNN: Vx = rand()&NN
        regX = ((opcode & 0x0F00) >> 8);
        reg[regX] = rand() & (opcode & 0x00FF);
        std::fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Register [%hhx] updated to [%hhx]\n",
             regX, reg[regX]);
        break;

    case 0xF000: // Multiple set of instructions of type 0xFXDD
        regX = ((opcode & 0x0F00) >> 8);
        switch (opcode & 0x00FF)
        {
        case 0x001E: // I += Vx
            IReg += reg[regX];
            std::fprintf(myDebugFile,
                "[I] <opcodeHandler.cpp>::Register I updated to [%hx]\n",
                IReg);
            break;

        case 0x0029: //I = sprite_address[Vx]
            IReg = spriteAddr[reg[regX]];
            std::fprintf(myDebugFile,
                "[I] <opcodeHandler.cpp>::Register I updated to [%hx]\n",
                IReg);
            break;

        case 0x0033: //BCD(Vx) store at I, I+1, I+2
            mem->setMemData(reg[regX]/100, IReg);
            mem->setMemData(((reg[regX] % 100) / 10), IReg+1);
            mem->setMemData((reg[regX] % 10), IReg+2);
            std::fprintf(myDebugFile,
                "[I] <opcodeHandler.cpp>::Updated mem address at location IReg: [%hx] to [%hhx], " 
                "IReg+1: [%hx] to [%hhx], and IReg+2: [%hx] to [%hhx]\n",
                IReg, reg[regX]/100, IReg+1, ((reg[regX] % 100) / 10), IReg+2, (reg[regX] % 10));
            break;

        case 0x0055: //reg_dump(Vx,&I)
            for (char i = 0; i <= regX; i++)
            {
                mem->setMemData(reg[i], IReg+i);
                std::fprintf(myDebugFile,
                    "[I] <opcodeHandler.cpp>::Register [%hhx] with val: [%hhx], stored at memory location: [%hx]\n",
                    i, reg[i], IReg+i);
            }
            break;

        case 0x0065: //reg_load(Vx,&I)
            for (char i = 0; i <= regX; i++)
            {
                reg[i] = mem->getMemData(IReg+i);
                std::fprintf(myDebugFile,
                    "[I] <opcodeHandler.cpp>::Register [%hhx] updated to [%hhx] from memory location [%hx]\n",
                    i, reg[i], IReg+i);
            }
            break;

        default:
            std::fprintf(myDebugFile,"[E] <opcodeHandler.cpp>::Unknown Opcode: %hx\n",
            opcode);
            return false;
            break;
        }
        break;

    default:
        std::fprintf(myDebugFile,"[E] <opcodeHandler.cpp>::Unknown Opcode: %hx\n",
        opcode);
        return false;
        break;
    }


    /* Completed List:
    8XY0    Assign
    8XY1    BitOp
    8XY2    BitOp
    8XY3    BitOp
    8XY4    Math
    8XY5    Math
    8XY6    BitOp
    8XY7    Math
    8XYE    BitOp
    6XNN    Const
    7XNN    Const
    1NNN    Flow
    2NNN    Flow
    00EE    Flow
    3XNN    Cond
    4XNN    Cond
    5XY0    Cond
    9XY0    Cond
    ANNN    MEM
    BNNN    Flow
    CXNN    Rand
    FX1E    MEM
    FX29    MEM
    FX33    BCD
    FX55    MEM
    FX65    MEM
    */
    return true;
}