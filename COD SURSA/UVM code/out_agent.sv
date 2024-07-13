class out_agent extends uvm_agent;
  out_monitor monitor;

  // Provide implementations of virtual methods such as get_type_name and create
  `uvm_component_utils(out_agent)

  // new - constructor
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction : new

  // build_phase
  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    monitor = out_monitor::type_id::create("monitor", this);
  endfunction : build_phase

endclass : out_agent
