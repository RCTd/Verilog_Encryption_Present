//------------------------------------------------------------------------------
//
// CLASS: ctrl_transfer
//
//------------------------------------------------------------------------------

class ctrl_transfer extends uvm_sequence_item;                      

  rand bit  toggle;
  rand bit  reset;
  rand int unsigned         transmit_delay = 0;
  rand int unsigned         transmit_hold = 0;

  constraint c_transmit_delay { 
    transmit_delay <=10000 ;
    transmit_delay > 0;
  }
  constraint c_transmit_hold { 
    transmit_hold <= 10000 ; 
  }

  `uvm_object_utils_begin(ctrl_transfer)
    `uvm_field_int      (toggle,  UVM_DEFAULT)
    `uvm_field_int      (reset,  UVM_DEFAULT)
    `uvm_field_int      (transmit_delay,UVM_DEFAULT)
    `uvm_field_int      (transmit_hold, UVM_DEFAULT)
  `uvm_object_utils_end

  // new - constructor
  function new (string name = "ctrl_transfer_inst");
    super.new(name);
  endfunction : new

endclass : ctrl_transfer
