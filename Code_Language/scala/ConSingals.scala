package rtl.riscv32e

import chisel3._
import chisel3.util._
import chisel3.util.experimental.decode._

object ConFlags {
  // ImmCon
  val IMM_I       = "b10000"
  val IMM_S       = "b01000"
  val IMM_B       = "b00100"
  val IMM_U       = "b00010"
  val IMM_J       = "b00001"
  val IMM_R       = "b?????"
  // Data1Con
  val DATA1_RS1   = "b0"
  val DATA1_ZERO  = "b1"
  val DATA1_DC    = "b?"
  // Data2Con
  val DATA2_RS2   = "b0"
  val DATA2_IMM   = "b1"
  val DATA2_DC    = "b?"
  // AluCon
  val ALU_ADD     = "b0000"
  val ALU_SUB     = "b0100"
  val ALU_SLT     = "b0110"
  val ALU_SLTU    = "b0111"
  val ALU_EQ      = "b0101"
  val ALU_AND     = "b1000"
  val ALU_OR      = "b1100"
  val ALU_XOR     = "b1110"
  val ALU_SLL     = "b1101"
  val ALU_SRL     = "b1011"
  val ALU_SRA     = "b1111"
  val ALU_DC      = "b????"
  // pcCon
  val PC_PLUS4    = "b00"
  val PC_OFFSET_B = "b01"
  val PC_ALU      = "b10"
  val PC_OFFSET_J = "b11"
  val PC_DC       = "b??"
  // ByteCon
  val BY_B        = "b00"
  val BY_H        = "b01"
  val BY_W        = "b10"
  val BY_DC       = "b??"
  // SignCon
  val SG_U        = "b0"
  val SG_S        = "b1"
  val SG_DC       = "b?"
  // RegCon
  val RC_ALU      = "b000"
  val RC_MEM      = "b001"
  val RC_PC4      = "b010"
  val RC_AUIPC    = "b011"
  val RC_CSR      = "b111"
  val RC_DC       = "b???"
  // CsrDataSel
  val CSR_IMM     = "b0"
  val CSR_SRC1    = "b1"
  val CSR_DATADC  = "b?"
  // CsrFunctSel
  val CSR_ANDNOT  = "b001"
  val CSR_OR      = "b010"
  val CSR_DATA    = "b100"
  val CSR_FUNDC   = "b???"
}

case class InstructionPattern(
  val decoderEntry: DecoderEntry)
    extends DecodePattern {
  def bitPat: BitPat = BitPat(decoderEntry.instPat)
}

object DecoderEntryField extends DecodeField[InstructionPattern, ConSignals] {
  def name       = "DecoderEntry"
  def chiselType = new ConSignals()
  // Default to invalid instruction, should only be used in testing
  override def default: BitPat = ConPatterns(invalidInst = "b1").combined

  def genTable(instPat: InstructionPattern): BitPat =
    instPat.decoderEntry.conPat.combined
}

class ConSignals extends Bundle {
  val immCon      = UInt(5.W)
  val data1Con    = Bool()
  val data2Con    = Bool()
  val aluCon      = UInt(4.W)
  val pcCon       = UInt(2.W)
  val branchNot   = Bool()
  val memWen      = Bool()
  val memRen      = Bool()
  val byteCon     = UInt(2.W)
  val signCon     = Bool()
  val regWen      = Bool()
  val regCon      = UInt(3.W)
  val csrDataSel  = Bool()
  val csrFunctSel = UInt(3.W)
  val csrWen      = Bool()
  val ecall       = Bool()
  val mret        = Bool()
  val invalidInst = Bool()
}

import ConFlags._
case class ConPatterns(
  immCon:      String = IMM_I,
  data1Con:    String = DATA1_RS1,
  data2Con:    String = DATA2_RS2,
  aluCon:      String = ALU_ADD,
  pcCon:       String = PC_PLUS4,
  branchNot:   String = "b?",
  memWen:      String = "b0",
  memRen:      String = "b0",
  byteCon:     String = BY_DC,      // Only for load/store instructions
  signCon:     String = SG_DC,      // Only for load instructions
  regWen:      String = "b1",
  regCon:      String = RC_ALU,
  csrDataSel:  String = CSR_DATADC, // Only for CSR instructions
  csrFunctSel: String = CSR_FUNDC,  // Only for CSR instructions
  csrWen:      String = "b0",
  ecall:       String = "b0",
  mret:        String = "b0",
  invalidInst: String = "b0") {
  val allFields: Seq[String] = Seq(
    immCon,
    data1Con,
    data2Con,
    aluCon,
    pcCon,
    branchNot,
    memWen,
    memRen,
    byteCon,
    signCon,
    regWen,
    regCon,
    csrDataSel,
    csrFunctSel,
    csrWen,
    ecall,
    mret,
    invalidInst
  )

  def combined: BitPat = allFields.map(BitPat(_)).reduce(_ ## _)
}

case class DecoderEntry(
  name:    String,
  instPat: String,
  conPat:  ConPatterns)

object DecoderTable {
  import ConFlags._
  val table: Seq[DecoderEntry] = Seq(
    // 移位指令
    DecoderEntry(
      "sll",
      "b0000000_?????_?????_001_?????_0110011",
      ConPatterns(
        immCon = IMM_R,
        aluCon = ALU_SLL
      )
    ),
    DecoderEntry(
      "slli",
      "b0000000_?????_?????_001_?????_0010011",
      ConPatterns(
        data2Con = DATA2_IMM,
        aluCon = ALU_SLL
      )
    ),
    DecoderEntry(
      "srl",
      "b0000000_?????_?????_101_?????_0110011",
      ConPatterns(
        immCon = IMM_R,
        aluCon = ALU_SRL
      )
    ),
    DecoderEntry(
      "srli",
      "b0000000_?????_?????_101_?????_0010011",
      ConPatterns(
        data2Con = DATA2_IMM,
        aluCon = ALU_SRL
      )
    ),
    DecoderEntry(
      "sra",
      "b0100000_?????_?????_101_?????_0110011",
      ConPatterns(
        immCon = IMM_R,
        aluCon = ALU_SRA
      )
    ),
    DecoderEntry(
      "srai",
      "b0100000_?????_?????_101_?????_0010011",
      ConPatterns(
        data2Con = DATA2_IMM,
        aluCon = ALU_SRA
      )
    ),
    // 算数指令
    DecoderEntry(
      "add",
      "b0000000_?????_?????_000_?????_0110011",
      ConPatterns(
        immCon = IMM_R
      )
    ),
    DecoderEntry(
      "addi",
      "b???????_?????_?????_000_?????_0010011",
      ConPatterns(
        data2Con = DATA2_IMM
      )
    ),
    DecoderEntry(
      "sub",
      "b0100000_?????_?????_000_?????_0110011",
      ConPatterns(
        immCon = IMM_R,
        aluCon = ALU_SUB
      )
    ),
    DecoderEntry(
      "lui",
      "b???????_?????_?????_???_?????_0110111",
      ConPatterns(
        immCon = IMM_U,
        data1Con = DATA1_ZERO,
        data2Con = DATA2_IMM
      )
    ),
    DecoderEntry(
      "auipc",
      "b???????_?????_?????_???_?????_0010111",
      ConPatterns(
        immCon = IMM_U,
        data1Con = DATA1_DC,
        data2Con = DATA2_DC,
        aluCon = ALU_DC,
        regCon = RC_AUIPC
      )
    ),
    // 逻辑指令
    DecoderEntry(
      "xor",
      "b0000000_?????_?????_100_?????_0110011",
      ConPatterns(
        immCon = IMM_R,
        aluCon = ALU_XOR
      )
    ),
    DecoderEntry(
      "xori",
      "b???????_?????_?????_100_?????_0010011",
      ConPatterns(
        data2Con = DATA2_IMM,
        aluCon = ALU_XOR
      )
    ),
    DecoderEntry(
      "or",
      "b0000000_?????_?????_110_?????_0110011",
      ConPatterns(
        immCon = IMM_R,
        aluCon = ALU_OR
      )
    ),
    DecoderEntry(
      "ori",
      "b???????_?????_?????_110_?????_0010011",
      ConPatterns(
        data2Con = DATA2_IMM,
        aluCon = ALU_OR
      )
    ),
    DecoderEntry(
      "and",
      "b0000000_?????_?????_111_?????_0110011",
      ConPatterns(
        immCon = IMM_R,
        aluCon = ALU_AND
      )
    ),
    DecoderEntry(
      "andi",
      "b???????_?????_?????_111_?????_0010011",
      ConPatterns(
        data2Con = DATA2_IMM,
        aluCon = ALU_AND
      )
    ),
    // 比较-置位指令
    DecoderEntry(
      "slt",
      "b0000000_?????_?????_010_?????_0110011",
      ConPatterns(
        immCon = IMM_R,
        aluCon = ALU_SLT
      )
    ),
    DecoderEntry(
      "slti",
      "b???????_?????_?????_010_?????_0010011",
      ConPatterns(
        data2Con = DATA2_IMM,
        aluCon = ALU_SLT
      )
    ),
    DecoderEntry(
      "sltu",
      "b0000000_?????_?????_011_?????_0110011",
      ConPatterns(
        immCon = IMM_R,
        aluCon = ALU_SLTU
      )
    ),
    DecoderEntry(
      "sltiu",
      "b???????_?????_?????_011_?????_0010011",
      ConPatterns(
        data2Con = DATA2_IMM,
        aluCon = ALU_SLTU
      )
    ),
    // 分支指令
    DecoderEntry(
      "beq",
      "b???????_?????_?????_000_?????_1100011",
      ConPatterns(
        immCon = IMM_B,
        aluCon = ALU_EQ,
        pcCon = PC_OFFSET_B,
        branchNot = "b0",
        regWen = "b0",
        regCon = RC_DC
      )
    ),
    DecoderEntry(
      "bne",
      "b???????_?????_?????_001_?????_1100011",
      ConPatterns(
        immCon = IMM_B,
        aluCon = ALU_EQ,
        pcCon = PC_OFFSET_B,
        branchNot = "b1",
        regWen = "b0",
        regCon = RC_DC
      )
    ),
    DecoderEntry(
      "blt",
      "b???????_?????_?????_100_?????_1100011",
      ConPatterns(
        immCon = IMM_B,
        aluCon = ALU_SLT,
        pcCon = PC_OFFSET_B,
        branchNot = "b0",
        regWen = "b0",
        regCon = RC_DC
      )
    ),
    DecoderEntry(
      "bge",
      "b???????_?????_?????_101_?????_1100011",
      ConPatterns(
        immCon = IMM_B,
        aluCon = ALU_SLT,
        pcCon = PC_OFFSET_B,
        branchNot = "b1",
        regWen = "b0",
        regCon = RC_DC
      )
    ),
    DecoderEntry(
      "bltu",
      "b???????_?????_?????_110_?????_1100011",
      ConPatterns(
        immCon = IMM_B,
        aluCon = ALU_SLTU,
        pcCon = PC_OFFSET_B,
        branchNot = "b0",
        regWen = "b0",
        regCon = RC_DC
      )
    ),
    DecoderEntry(
      "bgeu",
      "b???????_?????_?????_111_?????_1100011",
      ConPatterns(
        immCon = IMM_B,
        aluCon = ALU_SLTU,
        pcCon = PC_OFFSET_B,
        branchNot = "b1",
        regWen = "b0",
        regCon = RC_DC
      )
    ),
    // 跳转并连接指令
    DecoderEntry(
      "jal",
      "b???????_?????_?????_???_?????_1101111",
      ConPatterns(
        immCon = IMM_J,
        aluCon = ALU_DC,
        pcCon = PC_OFFSET_J,
        regCon = RC_PC4
      )
    ),
    DecoderEntry(
      "jalr",
      "b???????_?????_?????_000_?????_1100111",
      ConPatterns(
        data2Con = DATA2_IMM,
        pcCon = PC_ALU,
        regCon = RC_PC4
      )
    ),
    // Load指令
    DecoderEntry(
      "lb",
      "b???????_?????_?????_000_?????_0000011",
      ConPatterns(
        data2Con = DATA2_IMM,
        regCon = RC_MEM,
        memRen = "b1",
        byteCon = BY_B,
        signCon = SG_S
      )
    ),
    DecoderEntry(
      "lbu",
      "b???????_?????_?????_100_?????_0000011",
      ConPatterns(
        data2Con = DATA2_IMM,
        regCon = RC_MEM,
        memRen = "b1",
        byteCon = BY_B,
        signCon = SG_U
      )
    ),
    DecoderEntry(
      "lh",
      "b???????_?????_?????_001_?????_0000011",
      ConPatterns(
        data2Con = DATA2_IMM,
        regCon = RC_MEM,
        memRen = "b1",
        byteCon = BY_H,
        signCon = SG_S
      )
    ),
    DecoderEntry(
      "lhu",
      "b???????_?????_?????_101_?????_0000011",
      ConPatterns(
        data2Con = DATA2_IMM,
        regCon = RC_MEM,
        memRen = "b1",
        byteCon = BY_H,
        signCon = SG_U
      )
    ),
    DecoderEntry(
      "lw",
      "b???????_?????_?????_010_?????_0000011",
      ConPatterns(
        data2Con = DATA2_IMM,
        regCon = RC_MEM,
        memRen = "b1",
        byteCon = BY_W
      )
    ),
    // Store指令
    DecoderEntry(
      "sb",
      "b???????_?????_?????_000_?????_0100011",
      ConPatterns(
        immCon = IMM_S,
        data2Con = DATA2_IMM,
        regWen = "b0",
        memWen = "b1",
        byteCon = BY_B,
        regCon = RC_DC
      )
    ),
    DecoderEntry(
      "sh",
      "b???????_?????_?????_001_?????_0100011",
      ConPatterns(
        immCon = IMM_S,
        data2Con = DATA2_IMM,
        regWen = "b0",
        memWen = "b1",
        byteCon = BY_H,
        regCon = RC_DC
      )
    ),
    DecoderEntry(
      "sw",
      "b???????_?????_?????_010_?????_0100011",
      ConPatterns(
        immCon = IMM_S,
        data2Con = DATA2_IMM,
        regWen = "b0",
        memWen = "b1",
        byteCon = BY_W,
        regCon = RC_DC
      )
    ),
    // CSR指令
    DecoderEntry(
      "csrrw",
      "b???????_?????_?????_001_?????_1110011",
      ConPatterns(
        data2Con = DATA2_DC,
        aluCon = ALU_DC,
        regCon = RC_CSR,
        csrDataSel = CSR_SRC1,
        csrFunctSel = CSR_DATA,
        csrWen = "b1"
      )
    ),
    DecoderEntry(
      "csrrs",
      "b???????_?????_?????_010_?????_1110011",
      ConPatterns(
        data2Con = DATA2_DC,
        aluCon = ALU_DC,
        regCon = RC_CSR,
        csrDataSel = CSR_SRC1,
        csrFunctSel = CSR_OR,
        csrWen = "b1"
      )
    ),
    DecoderEntry(
      "csrrc",
      "b???????_?????_?????_011_?????_1110011",
      ConPatterns(
        data2Con = DATA2_DC,
        aluCon = ALU_DC,
        regCon = RC_CSR,
        csrDataSel = CSR_SRC1,
        csrFunctSel = CSR_ANDNOT,
        csrWen = "b1"
      )
    ),
    DecoderEntry(
      "csrrwi",
      "b???????_?????_?????_101_?????_1110011",
      ConPatterns(
        data2Con = DATA2_DC,
        aluCon = ALU_DC,
        regCon = RC_CSR,
        csrDataSel = CSR_IMM,
        csrFunctSel = CSR_DATA,
        csrWen = "b1"
      )
    ),
    DecoderEntry(
      "csrrsi",
      "b???????_?????_?????_110_?????_1110011",
      ConPatterns(
        data2Con = DATA2_DC,
        aluCon = ALU_DC,
        regCon = RC_CSR,
        csrDataSel = CSR_IMM,
        csrFunctSel = CSR_OR,
        csrWen = "b1"
      )
    ),
    DecoderEntry(
      "csrrci",
      "b???????_?????_?????_111_?????_1110011",
      ConPatterns(
        data2Con = DATA2_DC,
        aluCon = ALU_DC,
        regCon = RC_CSR,
        csrDataSel = CSR_IMM,
        csrFunctSel = CSR_ANDNOT,
        csrWen = "b1"
      )
    ),
    // System指令
    DecoderEntry(
      "ecall",
      "b0000000_00000_00000_000_00000_1110011",
      ConPatterns(
        regWen = "b0",
        ecall = "b1"
      )
    ),
    DecoderEntry(
      "mret",
      "b0011000_00010_00000_000_00000_1110011",
      ConPatterns(
        regWen = "b0",
        mret = "b1"
      )
    ),
    // Halt指令
    DecoderEntry(
      "halt(ebreak)",
      "b0000000_00001_00000_000_00000_1110011",
      ConPatterns(
        regWen = "b0"
      )
    )
  )

  val instructionPatterns: Seq[InstructionPattern] = {
    table.map { entry =>
      InstructionPattern(entry)
    }
  }
}
