; RUN: llc -mtriple=mips-linux-gnu -relocation-model=static < %s | FileCheck --check-prefix=ALL --check-prefix=O32 --check-prefix=O32-BE %s
; RUN: llc -mtriple=mipsel-linux-gnu -relocation-model=static < %s | FileCheck --check-prefix=ALL --check-prefix=O32 --check-prefix=O32-LE %s

; RUN-TODO: llc -mtriple=mips64-linux-gnu -relocation-model=static -mattr=-n64,+o32 < %s | FileCheck --check-prefix=ALL --check-prefix=O32 %s
; RUN-TODO: llc -mtriple=mips64el-linux-gnu -relocation-model=static -mattr=-n64,+o32 < %s | FileCheck --check-prefix=ALL --check-prefix=O32 %s

; RUN: llc -mtriple=mips64-linux-gnu -relocation-model=static -mattr=-n64,+n32 < %s | FileCheck --check-prefix=ALL --check-prefix=N32 --check-prefix=N32-BE %s
; RUN: llc -mtriple=mips64el-linux-gnu -relocation-model=static -mattr=-n64,+n32 < %s | FileCheck --check-prefix=ALL --check-prefix=N32 --check-prefix=N32-LE %s

; RUN: llc -mtriple=mips64-linux-gnu -relocation-model=static -mattr=-n64,+n64 < %s | FileCheck --check-prefix=ALL --check-prefix=N64 --check-prefix=N64-BE %s
; RUN: llc -mtriple=mips64el-linux-gnu -relocation-model=static -mattr=-n64,+n64 < %s | FileCheck --check-prefix=ALL --check-prefix=N64 --check-prefix=N64-LE %s

; Test struct returns for all ABI's and byte orders.

@struct_byte = global {i8} zeroinitializer
@struct_2byte = global {i8,i8} zeroinitializer

declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i32, i1)

define inreg {i8} @ret_struct_i8() nounwind {
entry:
        %0 = load volatile {i8}* @struct_byte
        ret {i8} %0
}

; ALL-LABEL: ret_struct_i8:
; O32-DAG:           lui [[R1:\$[0-9]+]], %hi(struct_byte)
; O32-DAG:           lbu $2, %lo(struct_byte)([[R1]])

; N32-LE-DAG:        lui [[R1:\$[0-9]+]], %hi(struct_byte)
; N32-LE-DAG:        lb $2, %lo(struct_byte)([[R1]])

; N32-BE-DAG:        lui [[R1:\$[0-9]+]], %hi(struct_byte)
; N32-BE-DAG:        lb [[R2:\$[0-9]+]], %lo(struct_byte)([[R1]])
; N32-BE-DAG:        dsll $2, [[R2]], 56

; N64-LE-DAG:        ld  [[R1:\$[0-9]+]], %got_disp(struct_byte)($1)
; N64-LE-DAG:        lb $2, 0([[R1]])

; N64-BE-DAG:        ld  [[R1:\$[0-9]+]], %got_disp(struct_byte)($1)
; N64-BE-DAG:        lb [[R2:\$[0-9]+]], 0([[R1]])
; N64-BE-DAG:        dsll $2, [[R2]], 56

; This test is based on the way clang currently lowers {i8,i8} to {i16}.
; FIXME: It should probably work for without any lowering too but this doesn't
;        work as expected. Each member gets mapped to a register rather than
;        packed into a single register.
define inreg {i16} @ret_struct_i16() nounwind {
entry:
        %retval = alloca {i8,i8}, align 1
        %0 = bitcast {i8,i8}* %retval to i8*
        call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* getelementptr inbounds ({i8,i8}* @struct_2byte, i32 0, i32 0), i64 2, i32 1, i1 false)
        %1 = bitcast {i8,i8}* %retval to {i16}*
        %2 = load volatile {i16}* %1
        ret {i16} %2
}

; ALL-LABEL: ret_struct_i16:
; O32-DAG:           lui [[R1:\$[0-9]+]], %hi(struct_2byte)
; O32-DAG:           lhu [[R2:\$[0-9]+]], %lo(struct_2byte)([[R1]])
; O32-DAG:           sh  [[R2]], 0([[SP:\$sp]])
; O32-DAG:           lhu $2, 0([[SP:\$sp]])

; N32-LE-DAG:        lui [[R1:\$[0-9]+]], %hi(struct_2byte)
; N32-LE-DAG:        lhu [[R2:\$[0-9]+]], %lo(struct_2byte)([[R1]])
; N32-LE-DAG:        sh  [[R2]], 8([[SP:\$sp]])
; N32-LE-DAG:        lh  $2, 8([[SP:\$sp]])

; N32-BE-DAG:        lui [[R1:\$[0-9]+]], %hi(struct_2byte)
; N32-BE-DAG:        lhu [[R2:\$[0-9]+]], %lo(struct_2byte)([[R1]])
; N32-BE-DAG:        sh  [[R2]], 8([[SP:\$sp]])
; N32-BE-DAG:        lh  [[R3:\$[0-9]+]], 8([[SP:\$sp]])
; N32-BE-DAG:        dsll $2, [[R3]], 48

; N64-LE-DAG:        ld  [[R1:\$[0-9]+]], %got_disp(struct_2byte)($1)
; N64-LE-DAG:        lhu [[R2:\$[0-9]+]], 0([[R1]])
; N64-LE-DAG:        sh  [[R2]], 8([[SP:\$sp]])
; N64-LE-DAG:        lh  $2, 8([[SP:\$sp]])

; N64-BE-DAG:        ld  [[R1:\$[0-9]+]], %got_disp(struct_2byte)($1)
; N64-BE-DAG:        lhu [[R2:\$[0-9]+]], 0([[R1]])
; N64-BE-DAG:        sh  [[R2]], 8([[SP:\$sp]])
; N64-BE-DAG:        lh  [[R3:\$[0-9]+]], 8([[SP:\$sp]])
; N64-BE-DAG:        dsll $2, [[R3]], 48
