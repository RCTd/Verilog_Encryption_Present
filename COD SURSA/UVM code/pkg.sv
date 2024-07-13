package pkg;
  
  import uvm_pkg::*;

// `include "uvm_macros.svh"

  typedef uvm_config_db#(virtual ctrl_if) ctrl_vif_config;
  typedef uvm_config_db#(virtual cmd_if ) cmd_vif_config;
  typedef uvm_config_db#(virtual out_if ) out_vif_config;
  typedef virtual ctrl_if ctrl_vif;
  typedef virtual ctrl_if cmd_vif;
  typedef virtual ctrl_if out_vif;

typedef enum int { TEXT = 0, KEY = 1} data_type;
typedef enum int { ACTIVE_RESET = 0, INACTIVE_RESET} ctrl_type;
typedef enum int { INACTIVE = 0, ACTIVE} toggle_type;

`include "ctrl_packet.sv"
`include "cmd_packet.sv"
`include "out_packet.sv"

`include "ctrl_transfer.sv"
`include "cmd_transfer.sv"
`include "out_transfer.sv"

`include "ctrl_monitor.sv"
`include "ctrl_sequencer.sv"
`include "ctrl_driver.sv"
`include "ctrl_agent.sv"

`include "cmd_monitor.sv"
`include "cmd_sequencer.sv"
`include "cmd_driver.sv"
`include "cmd_agent.sv"

`include "out_monitor.sv"
`include "out_agent.sv"

endpackage: pkg

  

