class out_packet extends uvm_sequence_item;
  
  logic [64-1:0] out;
  logic 		 done;

  `uvm_object_utils_begin(out_packet)
    `uvm_field_int  (out,   UVM_DEFAULT)    
  	`uvm_field_int  (done,  UVM_DEFAULT)
  `uvm_object_utils_end

  function new(string name = "out_packet");
    super.new(name);
  endfunction
endclass
