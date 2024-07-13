`include "pkg.sv"
`include "present_decoder.v"
`include "cmd_if.sv"
`include "ctrl_if.sv"
`include "out_if.sv"


module tb_top;
  import uvm_pkg::*;
  import pkg::*;
  `include "test_lib.sv" 

  cmd_if  cmd_vif(); // SystemVerilog Interface
  ctrl_if ctrl_vif(); // SystemVerilog Interface
  out_if  out_vif(); // SystemVerilog Interface
  
  present_decoder dut(
    ctrl_vif.clk,
    ctrl_vif.reset,
    cmd_vif.pl,
    out_vif.out,
    cmd_vif.in,
    out_vif.done
    );

  assign cmd_vif.clk = ctrl_vif.clk;  
  assign out_vif.clk = ctrl_vif.clk;

  initial begin automatic uvm_coreservice_t cs_ = uvm_coreservice_t::get();
    uvm_config_db#(virtual ctrl_if)::set(cs_.get_root(), "*", "ctrl_vif", ctrl_vif);
    uvm_config_db#(virtual cmd_if )::set(cs_.get_root(), "*", "cmd_vif" , cmd_vif);
    uvm_config_db#(virtual out_if )::set(cs_.get_root(), "*", "out_vif" , out_vif);

    run_test();
  end
   initial begin      
      $dumpfile("dump.vcd");
      $dumpvars();     
    
   end
  initial begin 
    ctrl_vif.reset  <= 1'b0;
    ctrl_vif.clk <= 1'b1;
    cmd_vif.pl <= 2'b0;
  end

  //Generate Clock
  always
    #5 ctrl_vif.clk = ~ctrl_vif.clk;

endmodule
    

