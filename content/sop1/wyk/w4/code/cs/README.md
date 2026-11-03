# Debugging context switches

## Build the kernel and userspace program

Install basic tooling:

```shell
sudo pacman -S --needed base-devel bc cpio pahole python wget \
  qemu-system-x86 gdb
```

Download and extract kernel source code:

```shell
wget https://cdn.kernel.org/pub/linux/kernel/v7.x/linux-7.2.15.tar.xz
tar -xf linux-7.2.15.tar.xz
```

Prepare kernel configuration with debugging enabled:

```shell
cd linux-7.2.15
make defconfig
./scripts/config --enable CONFIG_DEBUG_INFO
./scripts/config --enable CONFIG_DEBUG_INFO_DWARF_TOOLCHAIN_DEFAULT
./scripts/config --enable CONFIG_GDB_SCRIPTS
./scripts/config --enable CONFIG_FRAME_POINTER
./scripts/config --disable CONFIG_RANDOMIZE_BASE
make olddefconfig
```

Build the kernel:

```shell
cd linux-7.2.15
make -j$(nproc)
```

Verify existence of the outputs:

```shell
cd linux-7.2.15
# Compressed kernel image
ls -lh vmlinux arch/x86/boot/bzImage
# GDB plugin 
ls -lh vmlinux-gdb.py 
```

Compile the init program:

```shell
mkdir -p rootfs
gcc -static -O0 -g init.c -o rootfs/init
```

Create compressed initramfs image, containing our binary:

```shell
cd rootfs
find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz
```

## Run the system and attach the debugger

```shell
qemu-system-x86_64 \
  -kernel linux-7.2.15/arch/x86/boot/bzImage \
  -initrd initramfs.cpio.gz \
  -smp 1 \
  -append "console=ttyS0 nokaslr" \
  -nographic \
  -s
```

```shell
gdb -iex "add-auto-load-safe-path $PWD/linux-7.2.15" \
    -ex "target remote :1234" \
    -ex "add-symbol-file $PWD/rootfs/init" \
    -ex "layout split" \
    linux-7.2.15/vmlinux
```

Break on the timer interrupt:

```gdb
break asm_sysvec_apic_timer_interrupt
condition 1 ((unsigned long*)($rsp))[1] == 0x33
continue
```

## State right after the interrupt

Where are we now?

```text
disassemble $pc
```

```text
Dump of assembler code for function asm_sysvec_apic_timer_interrupt:
=> 0xffffffff81001470 <+0>:     endbr64
   0xffffffff81001474 <+4>:     nopl   (%rax)
   0xffffffff81001477 <+7>:     cld
   0xffffffff81001478 <+8>:     push   $0xffffffffffffffff
   0xffffffff8100147a <+10>:    call   0xffffffff81001930 <error_entry>
   0xffffffff8100147f <+15>:    mov    %rax,%rsp
   0xffffffff81001482 <+18>:    mov    %rsp,%rdi
   0xffffffff81001485 <+21>:    call   0xffffffff8242f040 <sysvec_apic_timer_interrupt>
   0xffffffff8100148a <+26>:    jmp    0xffffffff81001aa0 <error_return>
``` 

How did we get here? Let's inspect Interrupt Descriptor Table (IDT).

```gdb
monitor info registers
```

```shell
IDT=     fffffe0000000000 00000fff
```

So IDT is located at `fffffe0000000000` and has `0x0fff / 16 = 256` entries.

Print the entry for the timer interrupt (`0xec`):

```gdb
print/x ((struct gate_struct *)0xfffffe0000000000)[0xec]
print/x idt_table[0xec]
```
```text
$8 = {offset_low = 0x1470, 
      offset_middle = 0x8100, 
      offset_high = 0xffffffff, 
      segment = 0x10, 
      bits = {ist = 0x0, zero = 0x0, type = 0xe, dpl = 0x0, p = 0x1}, 
      reserved = 0x0}
```

Calculate the address of the interrupt handler:

```gdb
set $gate = ((struct gate_struct *)0xfffffe0000000000)[0xec]
print/x ((unsigned long)$gate.offset_high << 32) | ($gate.offset_middle << 16) | $gate.offset_low
```
```shell
$9 = 0xffffffff81001470
```

Compare the address of the interrupt handler with the address of the current function:

```gdb
print $pc
print &asm_sysvec_apic_timer_interrupt
```

Now, where's the stack right now?

```gdb
print $rsp
```

Since `bits.ist = 0x0` the CPU loads RSP from current `Task State Segment` (TSS)`.
`monitor info registers` shows the value of `Task Register (TR)`:
```text
TR =0040 fffffe0000003000 00004087 00008900 DPL=0 TSS64-avl
```

The loaded stack pointer is `0xfffffe0000003000` taken from `sp0` field:
```gdb
print/x *((struct x86_hw_tss *)0xfffffe0000003000)
```
```text
{reserved1 = 0x0, sp0 = 0xfffffe0000003000, sp1 = 0x0, sp2 = 0x7ffcae60b9f8, reserved2 = 0x0, ist = {0xfffffe000000b000, 0xfffffe000000e000, 0xfffffe0000011000,
0xfffffe0000014000, 0xfffffe0000017000, 0x0, 0x0}, reserved3 = 0x0, reserved4 = 0x0, reserved5 = 0x0, io_bitmap_base = 0x4088}
```

Note, the value of `$rsp` is already below `sp0`. That's because
CPU pushed several values after switching to the interrupt stack:

```gdb
x/5gx $rsp
```

```text
0xfffffe0000002fd8:     0x0000000000402fa7      0x0000000000000033
0xfffffe0000002fe8:     0x0000000000010217      0x00007ffec5659a10
0xfffffe0000002ff8:     0x000000000000002b
```

That's `RIP`, `CS`, `RFLAGS`, `RSP` and `SS`.
CPU automatically saves those values as _IRET frame_ before
jumping to the interrupt handler.

```gdb
info symbol ((unsigned long*)$rsp)[0]
```

Value `RIP = 0x0000000000402fa7` points directly to some instruction in the `.text` of our binary, i.e.:

```text
burn_cpu + 50 in section .text of /sop-site/content/sop1/wyk/w4/code/cs/rootfs/init
```

Note that saved stack pointer points somewhere totally different than
current `$rsp`:

```gdb
print/x ((unsigned long*)$rsp)[3]
```

## Early interrupt handler instructions

Right now, `$rsp` (`0xfffffe0000002fd8`) points to the **entry trampoline stack** inside `cpu_entry_area`
(Meltdown vulnerability mitigation). Before calling the C interrupt handler, the kernel must do some work
to switch to the actual kernel stack.

It first pushes a dummy error code (`-1`) so the frame layout matches exception frames.

```gdb
disassemble $pc
stepi 5
disassemble $pc
```

Then, a series of push instructions saves remaining general-purpose registers to the trampoline stack.
Those are 21 values from `struct pt_regs` (see https://elixir.bootlin.com/linux/v7.2.14/source/arch/x86/include/asm/ptrace.h#L103).

```
tbreak *0xffffffff81001973
continue
disassemble $pc
print/x *(struct pt_regs *)($rsp + 8)
```

Next, `swapgs` privileged instruction switches `GS` base to kernel Per-CPU area.
This contains things like `current_task`, kernel page tables, and especially
address of kernel stack for the current process.

`monitor info registers` before show `GS = 0`, note the change after:

```gdb
stepi
monitor info registers
```

```text
GS =0000 ffff88808418d000 00000000 00000000
```

Then after series of jumps `sync_regs` is reached:

```gdb
stepi 6
x/6i %pc
```

`lea` here loads `pt_regs* eregs` argument into `%rdi`.

```gdb
stepi 2
disassemble $pc
```

Here kernel locates the actual kernel stack with `mov %gs:0x145adf4(%rip),%rax`
and makes space (168 bytes) for copied `pt_regs` with `sub`.
Then there's a loop (inlined `memcpy`) copying `pt_regs` in 32b chunks. 
Copied `pt_regs` address is returned in `%rax`.

```gdb
finish
```

Now we see the stack switch with ` mov %rax,%rsp` and passing args to C portion
of the interrupt handler.

```gdb
stepi 2
print/x *(struct pt_regs*)$rdi
stepi
list sysvec_apic_timer_interrupt
```

We're now fully in kernel mode, so accessing the current process context is easy:

```shell
print $lx_current()->comm
print $lx_current()->pid
# lx_current() evaluates to *(struct task_struct **)(__per_cpu_offset[0] + (unsigned long)&current_task)
```

## The actual context switch

Inside `sysvec_apic_timer_interrupt` the timer handler updates timekeeping, 
charges CPU time to the current process, and—if the process's timeslice has expired
merely sets the `TIF_NEED_RESCHED` bit in the current task's `thread_info.flags`.

[//]: # ( TODO: show that decision point)

On the way out of the interrupt, right before returning to user space, 
the kernel checks if resched bit is set. If it is, it calls `schedule()` -> `__schedule()` -> `context_switch()`:

```gdb
break schedule
continue
bt
print $lx_current()->comm
```

Try jumping around the frames to see how we got here.

```gdb
break __switch_to_asm
continue
bt
```

After scheduler makes a decision, it calls `__switch_to_asm()`, passing two arguments:
current tasks struct in `%rdi` (`prev`) and next task struct in `%rsi` (`next`).
This is where switching context actually happens.

```gdb
disassemble $pc
```

The instruction pointer got already saved on the stack by calling into `__switch_to_asm()`:

```gdb
info symbol *(unsigned long*)$rsp
__schedule + 1086 in section .text of /home/saqq/repos/sop-site/content/sop1/wyk/w4/code/cs/linux-7.2.15/vmlinux
```

This is the saved execution point of the previous task. Next, the rest of the context gets saved.

```text
push   %rbp
push   %rbx
push   %r12
push   %r13
push   %r14
push   %r15
```

A set of `push` instructions saves calee-saved registers. 

```text
mov    %rsp,0xc48(%rdi)
```

This `mov` stores the stack pointer of the current task in it's `task_struct`: `prev->thread.sp`.
This is the only thing that gets written to the `task_struct` in the kernel.
The rest is sitting on the kernel stack itself.

Then a stack switch happens by loading it from `next->thread.sp`:

```text
mov    0xc48(%rsi),%rsp
```

Next, a series of security-related instructions are invoked. 
Then, saved registers are restored from the `next` stack:

```text
pop    %r15
pop    %r14
pop    %r13
pop    %r12
pop    %rbx
pop    %rbp
```

Now at the top of the stack we do have a perfectly valid `%rip` of the next task:

```gdb
break *(__switch_to_asm+92)
continue
info symbol *(unsigned long*)$rsp
bt
```

Inside [__switch_to](https://elixir.bootlin.com/linux/v7.2.14/source/arch/x86/kernel/process_64.c#L610)
C function kernel updates per-cpu variables, like `current_task`.

```gdb
stepi
disassemble $pc
```

[//]: # ( TODO: show instructions doing that)

Here you can verify the target process:
```gdb
print next_p->comm
print next_p->pid
```

Now, stepping-out will leave to `__schedule` but of the `next` task.

```gdb
finish
bt
print $lx_current()->comm
```

## Back to the user space

We're now on the next task's kernel stack, on the way out to the user space.
To get there reliably break on the `error_return` jumped to after C-portion of the timer interrupt.

```gdb
break error_return if ((struct pt_regs*)$rsp)->cs == 0x33
continue
disassemble $pc
```

```text
push   %rax
pushf
pop    %rax
test   $0x200,%eax
je     0xffffffff81001aac <error_return+12>
ud2
pop    %rax
testb  $0x3,0x88(%rsp)
je     0xffffffff8100169c <common_interrupt_return+204>
jmp    0xffffffff810015d0 <common_interrupt_return>
```

The stack now contains saved `pt_regs` of the next process.

```gdb
print/x *(struct pt_regs*)$rsp
info symbol ((struct pt_regs*)$rsp)->ip
```

This tail jumps to `common_interrupt_return` which undoes the interrupt entry path in rev order:
1. restores user registers from `pt_regs`
2. runs `swapgs`
3. finally, executes `iretq` instruction

`iretq` reverses what happens on hardware interrupt, pops 5 values: `rip`, `cs`, `rflags`, `rsp` and `ss`,
effectively returning to user-space code at state in which it was interrupted.


