class cmd_agent extends uvm_agent;
  cmd_driver driver;
  cmd_sequencer sequencer;
  cmd_monitor monitor;

  // Provide implementations of virtual methods such as get_type_name and create
  `uvm_component_utils(cmd_agent)

  // new - constructor
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction : new

  // build_phase
  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    monitor = cmd_monitor::type_id::create("monitor", this);

    if(get_is_active() == UVM_ACTIVE) begin
      sequencer = cmd_sequencer::type_id::create("sequencer", this);
      driver = cmd_driver::type_id::create("driver", this);
    end
  endfunction : build_phase

  // connect_phase
  function void connect_phase(uvm_phase phase);
    if(get_is_active() == UVM_ACTIVE) begin
      driver.seq_item_port.connect(sequencer.seq_item_export);
    end
  endfunction : connect_phase

endclass : cmd_agent


