.file "OsHwSchedPrio.s"
.unified
.thumb_func
.section ".text"
.align 4
.globl OsHwSearchForHighPrio
.extern OsSchedPrioTypeSize
.extern OsHwSchedPrioReg

_osNoReadyTaskIsFound .equ 0xA0A0F5F5UL

_OsHwSearchForHighPrio:
                      push {r4,r7,lr}
#if 0
                      ldr r0, =OsSchedPrioTypeSize
                      mov sizeof(OsSchedPrioType),r7
                      addi -4,r7,r7
                      mov _OsHwSchedPrioReg,r4
                      mov 32,r0
                      add r7,r4
_nextword:
                      ld.w 0[r4],r5
                      sch1l r5,r6                       -- search for the high prio ready task (1 from Left)
                      cmp r6,r0
                      bne _end
                      addi -4,r4,r4
                      addi -4,r7,r7
                      cmp r0,r7
                      bp _nextword                        -- continue searching for the high prio flag in the next word of the variable OsHwSchedPrioReg (struct OsSchedPrioType)
                      mov _osNoReadyTaskIsFound,r0
                      pop {r4,r7,pc}
 _end:
                      shr 2,r7
                      shl 5,r7
                      sub r6,r0
                      add r7,r0
#endif
                      pop, {r4,r7,pc}
