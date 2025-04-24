In this project, we will practice computer programming skills, particularly input/output, computer language, and writing programs in assembly language. These programs will run on a simulator and assembler, both written in C.

We will work with a RISC processor named SIMP, which is similar to MIPS but simpler.

The SIMP simulator simulates the processor and a few I/O devices:

LEDs

7-segment display

256×256 monochrome monitor

Hard disk

Each instruction is executed in one clock cycle. The processor runs at a frequency of 512 Hz, performing 512 assembly instructions per second.
Here is a diagram illustrating the project structure (note: the visual layout and arrows from the PDF will be recreated if needed):

![image](https://github.com/user-attachments/assets/08caaf55-d91f-44a1-b144-767b5f3d46f5)

Registers
The SIMP processor contains 16-bit wide registers, with a total of 32 registers. Their names and functions follow standard calling conventions, similar to what you might have seen in MIPS architecture:

![image](https://github.com/user-attachments/assets/8bb77c07-64ef-48b5-ad9d-ec51552a4460)

Registers like $imm1 and $imm2 always contain constant immediate values (after sign extension).

The $zero register always contains zero. Any instruction that writes to $zero, $imm1, or $imm2 will have no effect — their values remain constant.

Memory
Instruction Memory:
Width: 48 bits

Depth: 4096 lines

The Program Counter (PC) is 12 bits wide.

Instructions are stored in consecutive lines and executed one per clock cycle.

Instructions do not support byte or short-level operations — only word-level access (32 bits per word).

Data Memory:
Width: 32 bits

Depth: 4096 lines

All memory accesses read or write full 32-bit words.

Instruction Format and Encoding
Each instruction in SIMP is 48 bits wide and has the following field structure:


![image](https://github.com/user-attachments/assets/f8d6fe8d-5b7b-4c79-a70d-e0b08446b701)

Supported Opcodes:

![image](https://github.com/user-attachments/assets/c3c7cf7b-3330-4a4a-89e5-f78e91fd74e6)


I/O (Input/Output)
The processor supports I/O through in and out instructions, which access a special array of hardware registers (IORegisters). Their initial values after reset are 0.

IORegister Table:

![image](https://github.com/user-attachments/assets/69a17b5b-b8b2-4606-a1cb-5c261033c247)

![image](https://github.com/user-attachments/assets/68a1e3dc-fc7c-4b0a-a3b8-e771189bedc0)

Interrupts (IRQs)
SIMP supports 3 interrupts: irq0, irq1, and irq2.

IRQ 0: Triggered by the timer.

IRQ 1: Used by the hard disk to signal completion of a read/write.

IRQ 2: Triggered externally using the irq2in.txt input file.

Interrupt Logic:
Each clock cycle, the processor checks:

ini
Copy
Edit
irq = (irq0enable & irq0status) | (irq1enable & irq1status) | (irq2enable & irq2status)
If irq == 1 and the processor is not already in an interrupt handler:

It jumps to the address in irqhandler.

It saves the current PC in irqreturn.

If inside an interrupt handler and another interrupt arrives (before executing reti), the processor ignores it temporarily.

Interrupt service routines should clear the status registers (irqXstatus) after handling.

To return from an interrupt, the program uses:

nginx
Copy
Edit
reti  # sets PC = irqreturn


Timer
The timer is 32-bit and triggers irq0 when enabled (timerenable = 1).

Every clock cycle, if the timer is enabled, timercurrent increases by 1.

When timercurrent == timermax, it resets to 0 and triggers IRQ 0 by setting irq0status = 1.

LEDs
The SIMP processor is connected to 32 LEDs. The state of the LEDs is controlled by writing a 32-bit word to the leds hardware register:

Bit 0 → LED 0 (rightmost)

Bit 31 → LED 31 (leftmost)

A bit value of 1 turns the LED on, 0 turns it off

Monitor
A monochrome monitor with 256×256 resolution is connected to the SIMP processor.

Each pixel is represented by an 8-bit luminance value:

0 = white

255 = black

Values in between = gray shades

The monitor maintains a frame buffer:

256 rows of 256 bytes (pixels)

Starts all-zero (white)

Rows are scanned top to bottom, and left to right

Writing a Pixel:
To write a pixel:

Set monitoraddr with the pixel index (0 to 65535)

Set monitordata with the gray value

Execute out to set monitorcmd = 1
→ This updates the corresponding pixel on screen.

Reading back from monitorcmd returns 0.

Hard Disk
The SIMP processor is connected to a 64KB hard disk, consisting of 128 sectors, each 512 bytes.

It uses DMA for reading/writing sectors (128 words per sector).

It is linked to IRQ 1, which is triggered upon completion.

Using the Disk:
Check that diskstatus = 0 (free)

Write sector number to disksector

Write buffer memory address to diskbuffer

Write command (1=read, 2=write) to diskcmd

It takes 1024 clock cycles to complete a read/write.

During this time, diskstatus = 1

After completion, diskstatus = 0, irq1status = 1

Simulator Behavior
The simulator emulates the fetch-decode-execute loop:

Starts at PC = 0

Reads instruction from imemin.txt

Decodes and executes it

Unless it’s a jump, the next instruction is at PC + 1

Execution ends on halt instruction

Running the Simulator
The simulator (sim.exe) accepts 14 command-line parameters:

Copy
Edit
sim.exe imemin.txt dmemin.txt diskin.txt irq2in.txt dmemout.txt regout.txt trace.txt hwregtrace.txt cycles.txt leds.txt display7seg.txt diskout.txt monitor.txt monitor.yuv
Each input file should be present, even if not used by your program. For example, diskin.txt must exist even if your program doesn’t use the disk.

Assembler Behavior
To compile assembly code:

cpp
Copy
Edit
asm.exe program.asm imemin.txt dmemin.txt
The assembler translates assembly code into machine code and memory initialization files.

It supports labels, comments, and constants in decimal, hexadecimal, or label references.

Labels are defined like: L1: and referenced as L1 in instructions.


Assumptions and Constraints
Max input line length: 500 characters

Max label length: 50 characters

Labels must start with a letter and can contain letters/numbers

Ignore extra whitespace or tab spacing

Hex supports both upper/lowercase (e.g., 0xAB and 0xab)

Follow course forum updates for clarifications























