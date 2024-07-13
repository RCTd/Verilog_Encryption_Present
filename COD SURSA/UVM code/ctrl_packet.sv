class ctrl_packet extends uvm_sequence_item;

  bit data;

  `uvm_object_utils_begin(ctrl_packet)
    `uvm_field_int  (data,      UVM_DEFAULT)
  `uvm_object_utils_end

  function new(string name = "ctrl_packet");
    super.new(name);
  endfunction

endclass
  
