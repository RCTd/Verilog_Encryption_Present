class cmd_packet extends uvm_sequence_item;

  bit                           data_type;
  logic [80-1 : 0 ]  			data;

  `uvm_object_utils_begin(cmd_packet)
  `uvm_field_int  (data_type,    UVM_DEFAULT)
    `uvm_field_int  (data,  UVM_DEFAULT)
  `uvm_object_utils_end

  function new(string name = "cmd_packet");
    super.new(name);
  endfunction
endclass
