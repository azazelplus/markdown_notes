package rtl.riscv32e

import chisel3._
import chisel3.util._
import chisel3.util.experimental.decode._

class FinishBox extends BlackBox with HasBlackBoxResource {
  val io = IO(new Bundle {
    val finish = Input(Bool())
  })
  addResource("FinishBox.v")
}

class InvalidInstBox extends BlackBox with HasBlackBoxResource {
  val io = IO(new Bundle {
    val clock   = Input(Clock())
    val invFlag = Input(Bool())
    val reset   = Input(Bool())
  })
  addResource("InvalidInstBox.v")
}

class Decoder extends Module {
  val io = IO(
    new Bundle {
      val inst       = Input(UInt(32.W))
      val gprWdata   = Input(UInt(32.W))
      val csrWdata   = Input(UInt(32.W))
      val pc         = Input(UInt(32.W))
      val conSignals = Output(new ConSignals)
      val imm        = Output(UInt(32.W))
      val rs1data    = Output(UInt(32.W))
      val rs2data    = Output(UInt(32.W))
      val csrdata    = Output(UInt(32.W))
      val mtvecCSR   = Output(UInt(32.W))
      val mepcCSR    = Output(UInt(32.W))
      val zimm       = Output(UInt(5.W))
    }
  )

  val allPossibleInst = DecoderTable.instructionPatterns
  val decodeTable     = new DecodeTable(allPossibleInst, Seq(DecoderEntryField))
  val decodeResult    = decodeTable.decode(io.inst)
  io.conSignals := decodeResult(DecoderEntryField) // Immediate generation
  val immI = Cat(Fill(20, io.inst(31)), io.inst(31, 20))
  val immS = Cat(Fill(20, io.inst(31)), io.inst(31, 25), io.inst(11, 7))
  val immB = Cat(
    Fill(19, io.inst(31)),
    io.inst(31),
    io.inst(7),
    io.inst(30, 25),
    io.inst(11, 8),
    0.U(1.W)
  )
  val immU = Cat(io.inst(31, 12), Fill(12, 0.U))
  val immJ = Cat(
    Fill(11, io.inst(31)),
    io.inst(31),
    io.inst(19, 12),
    io.inst(20),
    io.inst(30, 21),
    0.U(1.W)
  )
  io.imm := MuxLookup(io.conSignals.immCon, 0.U)(
    Seq(
      ConFlags.IMM_I.U(5.W) -> immI,
      ConFlags.IMM_S.U(5.W) -> immS,
      ConFlags.IMM_B.U(5.W) -> immB,
      ConFlags.IMM_U.U(5.W) -> immU,
      ConFlags.IMM_J.U(5.W) -> immJ
    )
  )

  val gprsFile = Module(new GPRsFile())
  gprsFile.io.rs1   := io.inst(19, 15)
  gprsFile.io.rs2   := io.inst(24, 20)
  gprsFile.io.waddr := io.inst(11, 7)
  gprsFile.io.wdata := io.gprWdata
  gprsFile.io.wen   := io.conSignals.regWen
  io.rs1data        := gprsFile.io.rs1data
  io.rs2data        := gprsFile.io.rs2data

  val csrsFile = Module(new CSRsFile())
  csrsFile.io.addr       := io.inst(31, 20)
  csrsFile.io.wdata      := io.csrWdata
  csrsFile.io.pc         := io.pc
  csrsFile.io.conSignals := io.conSignals
  io.csrdata             := csrsFile.io.rdata
  io.zimm                := io.inst(19, 15)
  io.mtvecCSR            := csrsFile.io.mtvecCSR
  io.mepcCSR             := csrsFile.io.mepcCSR

  val finBox = Module(new FinishBox)
  finBox.io.finish := (io.inst === "h00100073".U) // ebreak

  val invalidInstBox = Module(new InvalidInstBox)
  invalidInstBox.io.clock   := clock
  invalidInstBox.io.reset   := reset
  invalidInstBox.io.invFlag := io.conSignals.invalidInst
}
