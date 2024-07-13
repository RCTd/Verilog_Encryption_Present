`include "scoreboard.sv"
`include "ctrl_seq_lib.sv"
`include "cmd_seq_lib.sv"

class env extends uvm_env;

  // Virtual Interface variable
  protected virtual interface ctrl_if ctrl_vif;
  protected virtual interface cmd_if  cmd_vif;
  protected virtual interface out_if  out_vif;

  // The following two bits are used to control whether checks and coverage are
  // done both in the general monitor class and the interface.
  bit intf_checks_enable = 1; 
  bit intf_coverage_enable = 1;

  // Components of the environment
  ctrl_agent ctrl_agt;
  cmd_agent  cmd_agt;
  out_agent  out_agt;

  scoreboard scoreboard0; 

  uvm_event reset;

  // Provide implementations of virtual methods such as get_type_name and create
  `uvm_component_utils_begin(env)
    `uvm_field_int(intf_checks_enable, UVM_DEFAULT)
    `uvm_field_int(intf_coverage_enable, UVM_DEFAULT)
  `uvm_component_utils_end

  // new - constructor
  function new(string name, uvm_component parent);
    super.new(name, parent);
  endfunction : new

  // build_phase
  function void build_phase(uvm_phase phase);
    string inst_name;
//    set_phase_domain("uvm");
    super.build_phase(phase);
     if(!uvm_config_db#(virtual ctrl_if)::get(this, "", "ctrl_vif", ctrl_vif))
       `uvm_fatal("NOVIF",{"virtual ctrl interface must be set for: ",get_full_name(),".ctrl_vif"});
     if(!uvm_config_db#(virtual cmd_if )::get(this, "", "cmd_vif",  cmd_vif))
       `uvm_fatal("NOVIF",{"virtual cmd interface must be set for: ", get_full_name(),".cmd_vif"});
     if(!uvm_config_db#(virtual out_if )::get(this, "", "out_vif",  out_vif))
       `uvm_fatal("NOVIF",{"virtual out interface must be set for: ", get_full_name(),".out_vif"});
     
    scoreboard0 = scoreboard::type_id::create("scoreboard0", this);
    
    ctrl_agt = ctrl_agent::type_id::create("ctrl_agt", this);
    cmd_agt  =  cmd_agent::type_id::create("cmd_agt",  this);
    out_agt  =  out_agent::type_id::create("out_agt",  this);

    reset = new();
    uvm_config_db#(uvm_event)::set(null, "*", "reset", reset);

  endfunction : build_phase

  function void connect_phase(uvm_phase phase);
    // Connect monitors to scoreboard
    ctrl_agt.monitor.item_collected_port.connect(
      scoreboard0.ctrl_item_collected_export);
    cmd_agt.monitor.item_collected_port.connect(
      scoreboard0.cmd_item_collected_export);
    out_agt.monitor.item_collected_port.connect(
      scoreboard0.out_item_collected_export);
  endfunction : connect_phase

  // update_vif_enables
  protected task update_vif_enables();
    forever begin
      @(intf_checks_enable || intf_coverage_enable);
      ctrl_vif.has_checks <= intf_checks_enable;
      ctrl_vif.has_coverage <= intf_coverage_enable;
      cmd_vif.has_checks <= intf_checks_enable;
      cmd_vif.has_coverage <= intf_coverage_enable;
      out_vif.has_checks <= intf_checks_enable;
      out_vif.has_coverage <= intf_coverage_enable;
    end
  endtask : update_vif_enables

  // implement run task
  task run_phase(uvm_phase phase);
    fork
      update_vif_enables();
    join
  endtask : run_phase

endclass : env


  
