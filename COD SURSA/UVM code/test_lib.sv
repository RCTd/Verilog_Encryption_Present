`include "env.sv"

// Base Test
class example_base_test extends uvm_test;

  `uvm_component_utils(example_base_test)

  env env0;
  uvm_table_printer printer;
  bit test_pass = 1;

  function new(string name = "example_base_test", 
    uvm_component parent=null);
    super.new(name,parent);
  endfunction : new

  virtual function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    uvm_config_db#(int)::set(this, "*", "recording_detail", UVM_FULL);
    env0 = env::type_id::create("env0", this);
    printer = new();
    printer.knobs.depth = 3;
  endfunction : build_phase

  function void end_of_elaboration_phase(uvm_phase phase);
    `uvm_info(get_type_name(),
      $sformatf("Printing the test topology :\n%s", this.sprint(printer)), UVM_LOW)
  endfunction : end_of_elaboration_phase

  task run_phase(uvm_phase phase);
    phase.phase_done.set_drain_time(this, 50);
  endtask : run_phase

  function void extract_phase(uvm_phase phase);
   if(env0.scoreboard0.sbd_error)
     test_pass = 1'b0;
  endfunction
  
  function void report_phase(uvm_phase phase);
    if(test_pass) begin
      `uvm_info(get_type_name(), "** UVM TEST PASSED **", UVM_NONE)
    end
    else begin
      `uvm_error(get_type_name(), "** UVM TEST FAIL **")
    end
  endfunction

endclass : example_base_test

class simple_test extends example_base_test;

  `uvm_component_utils(simple_test)

  int num_of_repetitions = 5;

  function new(string name = "simple_test", uvm_component parent=null);
    super.new(name,parent);
  endfunction : new

  virtual function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    uvm_config_db#(uvm_object_wrapper)::set(this,
				"env0.ctrl_agt.sequencer.reset_phase",
                "default_sequence",
                reset_at_0::type_id::get());

    uvm_config_db#(uvm_object_wrapper)::set(this,
				"env0.cmd_agt.sequencer.main_phase",
                "default_sequence",
                start_phase::type_id::get());

    uvm_config_db#(int unsigned)::set(this, 
				"env0.cmd_agt.sequencer.start_phase", "text",   0);
    uvm_config_db#(int unsigned)::set(this, 
				"env0.cmd_agt.sequencer.start_phase", "key",    1);

  endfunction : build_phase

endclass : simple_test
