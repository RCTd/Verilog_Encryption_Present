class cmd_sequencer extends uvm_sequencer #(cmd_transfer);

  `uvm_component_utils(cmd_sequencer)

  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction : new

endclass :cmd_sequencer
