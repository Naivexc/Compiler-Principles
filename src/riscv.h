#define IS_IMM12(x) ((x) >= -2048 && (x) <= 2047)
#define LI(rd, imm) fprintf(yyout, "  li %s, %d\n", rd, imm);
#define LA(rd, label) fprintf(yyout, "  la %s, %s\n", rd, label);
#define ADD(rd, rs1, rs2) fprintf(yyout, "  add %s, %s, %s\n", rd, rs1, rs2);
#define ADDI(rd, rs1, imm12)                                       \
    {                                                              \
        if (IS_IMM12(imm12))                                       \
            fprintf(yyout, "  addi %s, %s, %d\n", rd, rs1, imm12); \
        else                                                       \
        {                                                          \
            LI("t6", imm12);                                       \
            ADD(rd, rs1, "t6");                                    \
        }                                                          \
    }
#define LW(rs, imm12, rd)                                       \
    {                                                           \
        if (IS_IMM12(imm12))                                    \
            fprintf(yyout, "  lw %s, %d(%s)\n", rs, imm12, rd); \
        else                                                    \
        {                                                       \
            LI("t6", imm12)                                     \
            ADD("t6", "t6", rd)                                 \
            fprintf(yyout, "  lw %s, 0(%s)\n", rs, "t6");       \
        }                                                       \
    }
#define SW(rs2, imm12, rs1)                                       \
    {                                                             \
        if (IS_IMM12(imm12))                                      \
            fprintf(yyout, "  sw %s, %d(%s)\n", rs2, imm12, rs1); \
        else                                                      \
        {                                                         \
            LI("t6", imm12)                                       \
            ADD("t6", "t6", rs1)                                  \
            fprintf(yyout, "  sw %s, 0(%s)\n", rs2, "t6");        \
        }                                                         \
    }
#define SUB(rd, rs1, rs2)                                   \
    {                                                       \
        fprintf(yyout, "  sub %s, %s, %s\n", rd, rs1, rs2); \
    }
#define SLT(rd, rs1, rs2)                                   \
    {                                                       \
        fprintf(yyout, "  slt %s, %s, %s\n", rd, rs1, rs2); \
    }
#define SGT(rd, rs1, rs2)                                   \
    {                                                       \
        fprintf(yyout, "  sgt %s, %s, %s\n", rd, rs1, rs2); \
    }
#define SEQZ(rd, rs)                               \
    {                                              \
        fprintf(yyout, "  seqz %s, %s\n", rd, rs); \
    }
#define SNEZ(rd, rs)                               \
    {                                              \
        fprintf(yyout, "  snez %s, %s\n", rd, rs); \
    }
#define XOR(rd, rs1, rs2)                                   \
    {                                                       \
        fprintf(yyout, "  xor %s, %s, %s\n", rd, rs1, rs2); \
    }
#define XORI(rd, rs1, imm12)                                 \
    {                                                        \
        fprintf(yyout, "xori %s, %s, %d\n", rd, rs1, imm12); \
    }
#define OR(rd, rs1, rs2)                                   \
    {                                                      \
        fprintf(yyout, "  or %s, %s, %s\n", rd, rs1, rs2); \
    }
#define AND(rd, rs1, rs2)                                   \
    {                                                       \
        fprintf(yyout, "  and %s, %s, %s\n", rd, rs1, rs2); \
    }
#define MUL(rd, rs1, rs2)                                   \
    {                                                       \
        fprintf(yyout, "  mul %s, %s, %s\n", rd, rs1, rs2); \
    }
#define DIV(rd, rs1, rs2)                                   \
    {                                                       \
        fprintf(yyout, "  div %s, %s, %s\n", rd, rs1, rs2); \
    }
#define REM(rd, rs1, rs2)                                   \
    {                                                       \
        fprintf(yyout, "  rem %s, %s, %s\n", rd, rs1, rs2); \
    }
#define JALR(rd, offset, rs)                                   \
    {                                                          \
        fprintf(yyout, "  jalr %s, %d(%s)\n", rd, offset, rs); \
    }
#define SP_DEV(sp_dev)                                \
    {                                                 \
        fprintf(yyout, "\n//sp_dev: %d\n\n", sp_dev); \
    }
#define BEQZ(rs, label)                                           \
    {                                                             \
        std::string label_j = "label_j_" + itostr(label_j_count); \
        ++label_j_count;                                          \
        LA("t5", label);                                          \
        fprintf(yyout, "  bnez %s, %s\n", rs, label_j.c_str());   \
        JALR("x0", 0, "t5");                                      \
        fprintf(yyout, "%s:\n", label_j.c_str());                 \
    }
#define BNEZ(rs, label)                                           \
    {                                                             \
        std::string label_j = "label_j_" + itostr(label_j_count); \
        ++label_j_count;                                          \
        LA("t5", label);                                          \
        fprintf(yyout, "  beqz %s, %s\n", rs, label_j.c_str());   \
        JALR("x0", 0, "t5");                                      \
        fprintf(yyout, "%s:\n", label_j.c_str());                 \
    }
#define J(label)             \
    {                        \
        LA("t5", label);     \
        JALR("x0", 0, "t5"); \
    }
#define CALL(label)                           \
    {                                         \
        fprintf(yyout, "  call %s\n", label); \
    }
#define RET                        \
    {                              \
        fprintf(yyout, "  ret\n"); \
    }
#define SIZE_OF_KIND(kind) \
    (                      \
        ((kind.tag) == KOOPA_RTT_INT32) || ((kind.tag) == KOOPA_RTT_POINTER) ? 4 : kind.data.array.len * SIZE_OFKIND)
#define DEBUG(str)                     \
    {                                  \
        fprintf(yyout, "//%s\n", str); \
    }

extern void Visit(const koopa_raw_program_t &program);
extern void Visit(const koopa_raw_slice_t &slice);
extern void Visit(const koopa_raw_function_t &func);
extern void Visit(const koopa_raw_basic_block_t &bb);
extern void Visit(const koopa_raw_value_t &value);
extern void Visit(koopa_raw_integer_t integer);
extern void Visit(koopa_raw_return_t ret);
extern void Visit(koopa_raw_binary_t binary);
extern void Visit(koopa_raw_load_t load);
extern void Visit(koopa_raw_store_t store);
extern void Visit(koopa_raw_branch_t branch);
extern void Visit(koopa_raw_jump_t jump);
extern void Visit(koopa_raw_call_t call);
extern void Visit(koopa_raw_global_alloc_t global_alloc);
extern void Visit(koopa_raw_get_elem_ptr_t get_elem_ptr);
extern void Visit(koopa_raw_get_ptr_t get_ptr);