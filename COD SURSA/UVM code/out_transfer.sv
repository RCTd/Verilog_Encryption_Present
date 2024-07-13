class out_transfer extends uvm_sequence_item;                      

  rand logic [64-1:0] out;
  rand logic done;

  `uvm_object_utils_begin(out_transfer)
    `uvm_field_int      (out, UVM_DEFAULT)  
    `uvm_field_int      (done, UVM_DEFAULT)
  `uvm_object_utils_end

  // new - constructor
  function new (string name = "out_transfer_inst");
    super.new(name);
  endfunction : new

endclass : out_transfer
