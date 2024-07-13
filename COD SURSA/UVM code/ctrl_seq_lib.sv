//------------------------------------------------------------------------------
//
// SEQUENCE: base_ctrl_sequence
//
//------------------------------------------------------------------------------

// This sequence raises/drops objections in the pre/post_body so that root
// sequences raise objections but subsequences do not.

virtual class base_ctrl_sequence extends uvm_sequence #(ctrl_transfer);

  function new(string name="base_ctrl_seq");
     super.new(name);
     set_automatic_phase_objection(1);
  endfunction
  
endclass : base_ctrl_sequence

//------------------------------------------------------------------------------
//
// SEQUENCE: reset_at_0
//
//------------------------------------------------------------------------------

// This sequence resets the dut once (designed for reset phase)

class reset_at_0 extends base_ctrl_sequence;
  
  function new(string name = "reset_at_0");
    super.new(name);
  endfunction

  `uvm_object_utils(reset_at_0)

  virtual task body();
    `uvm_do_with(req,
              { toggle == INACTIVE;
                reset == INACTIVE_RESET;
                transmit_delay == 1;
                transmit_hold  == 0;} )
    `uvm_do_with(req,
              { toggle == INACTIVE;
                reset == ACTIVE_RESET;
                transmit_delay == 2;
                transmit_hold  == 1;} )
    `uvm_do_with(req,
              { toggle == INACTIVE;
                reset == INACTIVE_RESET;
                transmit_delay == 3;
                transmit_hold  == 1;} )
  endtask : body
endclass : reset_at_0

//------------------------------------------------------------------------------
//
// SEQUENCE: multiple_ctrl
//
//------------------------------------------------------------------------------

// This sequence runs random sequences

class multiple_ctrl extends base_ctrl_sequence;
  
  function new(string name = "multiple_ctrl");
    super.new(name);
  endfunction

  `uvm_object_utils(multiple_ctrl)

  virtual task body();
    repeat(10) begin
      `uvm_do(req)
    end
    repeat(20) begin
      `uvm_do_with(req, 
              { reset == INACTIVE_RESET; } )
    end
  endtask : body
endclass : multiple_ctrl
