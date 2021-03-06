#include "include/opcodeHandler.h"
#include "include/memory.h"
#include <cstdlib>
#include <ctime>

// Method for disabling DEBUGGING FILE TO FILE Basis
// #define DISABLE_DEBUG_LOGS
#ifdef DISABLE_DEBUG_LOGS
    #define fprintf(myDebugFile, fmt, ...) (0)
#endif

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
    fprintf(myDebugFile,"[I] [PC: %hx]<opcodeHandler.cpp>::Next Instruction is: %hx\n",
    pc - 2, opcode);
    return true;
}

bool OpcodeHandler::emulateInstr(Memory *mem, graphics *myGraphics, keyboard *myKeyboard,
     timer *myTimer)
{
    // 0NNN: This instruction is not implemented.

    // Setting Graphics flag to false
    m_updateGraphics = false;

    // Data for instructions
    unsigned char regX;
    unsigned char regY;
    unsigned char NNData; //Stored 0xDDNN Last to hex data if needed
    unsigned short addr; //Used for storing memory address
    bool keyPress; //Used by 0xFX0A wait for a key press

    // Variables for 0xDXYN: Draw Sprite Instruction
    unsigned short xPos;
    unsigned short yPos;
    unsigned short height;
    unsigned char rowPixel;
    switch (opcode & 0xF000)
    {
    case 0x0000 : // 3 Instructions: 0xNNN; 0x0E0 and 0x00EE
        switch (opcode)
        {
        case 0x00E0 : // 0x00E0 Clear Screen
            m_updateGraphics = true;
            for (int i = 0; i < (SCREEN_HEIGHT / screenSizeModifier); i++)
            {
                for (int j = 0; j < (SCREEN_WIDTH / screenSizeModifier); j++)
                {
                   myGraphics->pixels[i][j] = 0x00;
                }
            }
            fprintf(myDebugFile, "[I] <opcodeHandler.cpp>::Cleared the Screen\n");
            break;

        case 0x00EE : // 0x00EE Return from subroutine
            sptr = (sptr - 1) % 16;
            pc = stack[sptr];
            fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Return from subroutine. PC updated to [%hx] &"
            " stack pointer updated to [%d]\n",
                pc,
                pc, sptr);  
            break;

        default:
            fprintf(myDebugFile,
            "[E] [PC: %hx]<opcodeHandler.cpp>::Unknown Opcode: %hx\n",
            pc,
            opcode);
            return false;
            break;
        }
        break;

    case 0x1000 : // 0x1NNN: pc = NNN
        pc = (0x0FFF & opcode);
        fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::PC updated to [%hx]\n",
            pc);
        break;

    case 0x2000 : //0x2NNN: call subroutine at NNN
        stack[sptr] = pc;
        sptr = (sptr + 1) % 16;
        pc = (0x0FFF & opcode);
        fprintf(myDebugFile,
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
            fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], NN Val: [%hhx]\n",
                pc, regX, reg[regX], NNData);
        }
        else
        {
            fprintf(myDebugFile,
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
            fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], NN Val: [%hhx]\n",
                pc, regX, reg[regX], NNData);
        }
        else
        {
            fprintf(myDebugFile,
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
            fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], regY Val: [%hhx]\n",
                pc, regX, reg[regX], reg[regY]);
        }
        else
        {
            fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Not Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], regY Val: [%hhx]\n",
                pc, regX ,reg[regX], reg[regY]);
        }
        break;

    case 0x6000 : // 0x6XNN: Vx = NN
        regX = ((opcode & 0x0F00) >> 8);
        reg[regX] = (opcode & 0x00FF);
        // Printing info
        fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Register [%hhx] updated to val [%hhx].\n",
            regX, reg[regX]);
        break;

    case 0x7000 : // 0x7XNN: Vx += NN
        regX = ((opcode & 0x0F00) >> 8);
        reg[regX] += (opcode & 0x00FF);
        fprintf(myDebugFile,
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
            fprintf(myDebugFile,"[E] <opcodeHandler.cpp>::Unknown Opcode: %hx\n",
            opcode);
            return false;
            break;
        }
        // Printing info
        fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Register [%hhx] updated to val [%hhx].\n",
            regX, reg[regX]);
        fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::VF val is [%hhx].\n",
            reg[0x0F]);
        break;

    case 0x9000 : // 0x9XY0: Skip nxt instr if (Vx != Vy)
        regX = ((opcode & 0x0F00) >> 8);
        regY = ((opcode & 0x00F0) >> 4);
        if (reg[regY] != reg[regX])
        {
            pc = pc + 2;
            fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], regY Val: [%hhx]\n",
                pc, regX, reg[regX], reg[regY]);
        }
        else
        {
            fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Not Skipped next Instruction. PC: [%hx],"
            "Register [%hhx] Val: [%hhx], regY Val: [%hhx]\n",
                pc, regX, reg[regX], reg[regY]);
        }
        break;

    case 0xA000 : // 0xANNN: I = NNN
        IReg = (opcode & 0x0FFF);
        fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Register I updated to [%hx]\n",
                IReg);
        break;

    case 0xB000 : // 0xBNNN: PC=V[0]+NNN
        pc = reg[0x00] + (opcode & 0x0FFF);
        fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::PC updated to [%hx]\n",
                pc);
        break;

    case 0xC000: //0xCXNN: Vx = rand()&NN
        regX = ((opcode & 0x0F00) >> 8);
        reg[regX] = rand() & (opcode & 0x00FF);
        fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Register [%hhx] updated to [%hhx]\n",
                regX, reg[regX]);
        break;

    case 0xD000: //0xDXYN: Draw sprite at location Vx, Vy till height N and set VF if collison
        m_updateGraphics = true;

        regX = ((opcode & 0x0F00) >> 8);
        regY = ((opcode & 0x00F0) >> 4);
        xPos = reg[regX] % (SCREEN_WIDTH / screenSizeModifier);
        yPos = reg[regY] % (SCREEN_HEIGHT / screenSizeModifier);
        height = (opcode & 0x000F);
        rowPixel = 0x00;

        reg[0x0F] = 0;
        for (int i = 0; i < (height); i++)
        {
            rowPixel = mem->getMemData(IReg + i);
            for (int j = 0; j < 8; j++)
            {
                if ( (rowPixel & (0x80 >> j)) != 0)
                {
                    unsigned short x = (xPos + j) % (SCREEN_WIDTH / screenSizeModifier);
                    unsigned short y = (yPos + i) % (SCREEN_HEIGHT / screenSizeModifier);
                    if (myGraphics->pixels[y][x] == 1)
                    {
                        reg[0x0F] = 1;
                    }
                    myGraphics->pixels[y][x] = (myGraphics->pixels[y][x] + 1) % 2;
                }
            }
            
        }
        fprintf(myDebugFile,
            "[I] <opcodeHandler.cpp>::Drawn Sprite at Pos:(%hd, %hd) and height:%hd. "
            "Also update reg[16] to %hhx\n",
            xPos, yPos, height, reg[0x0F]);
        break;

    case 0xE000: // Keyboard Skip Instructions
        regX = ((opcode & 0x0F00) >> 8);
        switch (opcode & 0x00FF)
        {
        case 0x009E: // if(key() == Vx) skip nxt Instruction
            if (myKeyboard->getKeyState(reg[regX]) == 1)
            {
                pc = pc + 2;
            }
            fprintf(myDebugFile,"[I] <opcodeHandler.cpp>::" 
            "PC updated to %hx, key in reg[%hhx] is %hhx\n",
            pc, regX, reg[regX]);
            break;

        case 0x00A1: // if(key() != Vx) skip nxt Instruction
            if (myKeyboard->getKeyState(reg[regX]) == 0)
            {
                pc = pc + 2;
            }
            fprintf(myDebugFile,"[I] <opcodeHandler.cpp>::" 
            "PC updated to %hx, key in reg[%hhx] is %hhx\n",
            pc, regX, reg[regX]);
            break;

        default:
            fprintf(myDebugFile,"[E] <opcodeHandler.cpp>::Unknown Opcode: %hx\n",
            opcode);
            return false;
            break;
        }
    break;

    case 0xF000: // Multiple set of instructions of type 0xFXDD
        regX = ((opcode & 0x0F00) >> 8);
        switch (opcode & 0x00FF)
        {
        case 0x0007: // 0xFX07: Vx = timer val
            reg[regX] = myTimer->getDelayTimer();
            fprintf(myDebugFile,"[I] <opcodeHandler.cpp>::Register[%hhx] updated to %hhx\n",
                regX, reg[regX]);
            break;
    
        case 0x000A: // Wait till a single press and store it in Vx
            keyPress = false;
            for (char i = 0; i < 0x10; i++)
            {
                if (myKeyboard->getKeyState(i) == 1)
                {
                    keyPress = true;
                    reg[regX] = i;
                    fprintf(myDebugFile,"[I] <opcodeHandler.cpp>::Key %hhx pressed,"
                    " moving to next Instruction\n",
                    i);
                    break;
                }
            }
            
            if(!keyPress)
            {
                fprintf(myDebugFile,"[I] <opcodeHandler.cpp>::No key pressed, "
                "staying at the same instruction\n");
                pc = pc - 2;
            }
            break;

        case 0x0015: // 0xFX15: Set delay timer to Vx
            myTimer->setDelayTimer(reg[regX]);
            fprintf(myDebugFile, "[I] <opcodeHandler.cpp>::Delay Timer set to %hhx\n",
                myTimer->getDelayTimer());
            break;

        case 0x0018: // 0xFX18: Set sound timer to Vx
            myTimer->setSoundTimer(reg[regX]);
            fprintf(myDebugFile, "[I] <opcodeHandler.cpp>::Sound Timer set to %hhx\n",
                reg[regX]);
            break;

        case 0x001E: // I += Vx
            IReg += reg[regX];
            fprintf(myDebugFile,
                "[I] <opcodeHandler.cpp>::Register I updated to [%hx]\n",
                IReg);
            break;

        case 0x0029: //I = sprite_address[Vx]
            IReg = spriteAddr[reg[regX]];
            fprintf(myDebugFile,
                "[I] <opcodeHandler.cpp>::Register I updated to [%hx]\n",
                IReg);
            break;

        case 0x0033: //BCD(Vx) store at I, I+1, I+2
            mem->setMemData(reg[regX]/100, IReg);
            mem->setMemData(((reg[regX] % 100) / 10), IReg+1);
            mem->setMemData((reg[regX] % 10), IReg+2);
            fprintf(myDebugFile,
                "[I] <opcodeHandler.cpp>::Updated mem address at location IReg: [%hx] to [%hhx], " 
                "IReg+1: [%hx] to [%hhx], and IReg+2: [%hx] to [%hhx]\n",
                IReg, reg[regX]/100, IReg+1, ((reg[regX] % 100) / 10), IReg+2, (reg[regX] % 10));
            break;

        case 0x0055: //reg_dump(Vx,&I)
            for (char i = 0; i <= regX; i++)
            {
                mem->setMemData(reg[i], IReg+i);
                fprintf(myDebugFile,
                    "[I] <opcodeHandler.cpp>::Register [%hhx] with val: [%hhx], stored at memory location: [%hx]\n",
                    i, reg[i], IReg+i);
            }
            break;

        case 0x0065: //reg_load(Vx,&I)
            for (char i = 0; i <= regX; i++)
            {
                reg[i] = mem->getMemData(IReg+i);
                fprintf(myDebugFile,
                    "[I] <opcodeHandler.cpp>::Register [%hhx] updated to [%hhx] from memory location [%hx]\n",
                    i, reg[i], IReg+i);
            }
            break;

        default:
            fprintf(myDebugFile,"[E] <opcodeHandler.cpp>::Unknown Opcode: %hx\n",
            opcode);
            return false;
            break;
        }
        break;

    default:
        fprintf(myDebugFile,"[E] <opcodeHandler.cpp>::Unknown Opcode: %hx\n",
        opcode);
        return false;
        break;
    }

    // Sleeping for sometime to handle the system speed
    std::this_thread::sleep_for(std::chrono::microseconds(EMULATION_SPEED_MICROS));

    return true;
}

bool OpcodeHandler::getGraphicsFlag()
{
    return m_updateGraphics;
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
00E0    Disp
DXYN    Disp
EX9E    KeyOp
EXA1    KeyOp
FX0A    KeyOp
FX07    Timer
FX15    Timer
FX18    Sound
*/