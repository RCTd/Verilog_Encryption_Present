class ctrl_sequencer extends uvm_sequencer #(ctrl_transfer);

  `uvm_component_utils(ctrl_sequencer)

  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction : new

endclass :ctrl_sequencer
