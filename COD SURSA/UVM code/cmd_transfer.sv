class cmd_transfer extends uvm_sequence_item;                      

  rand bit                  data_type;
  rand logic [80-1 : 0]    	data;
  rand int unsigned         transmit_delay = 0;

  constraint c_transmit_delay { 
    transmit_delay <= 100 ; 
  }

  constraint c_transmit_data 
  { 
    solve data_type before data;
    (data_type==TEXT)    -> data inside{ [0:(1<<64)-1] };
    (data_type==KEY)  	-> data inside{ [0:(1<<80)-1] };
  }


  `uvm_object_utils_begin(cmd_transfer)
  `uvm_field_int      (data_type,        UVM_DEFAULT)
    `uvm_field_int      (data,   UVM_DEFAULT)
    `uvm_field_int      (transmit_delay,    UVM_DEFAULT)
  `uvm_object_utils_end

  // new - constructor
  function new (string name = "cmd_transfer_inst");
    super.new(name);
  endfunction : new

endclass : cmd_transfer
