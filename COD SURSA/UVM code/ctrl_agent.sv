class ctrl_agent extends uvm_agent;
  ctrl_driver       driver;
  ctrl_sequencer    sequencer;
  ctrl_monitor      monitor;

  // Provide implementations of virtual methods such as get_type_name and create
  `uvm_component_utils(ctrl_agent)

  // new - constructor
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction : new

  // build_phase
  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    monitor = ctrl_monitor::type_id::create("monitor", this);

    if(get_is_active() == UVM_ACTIVE) begin
      sequencer = ctrl_sequencer::type_id::create("sequencer", this);
      driver = ctrl_driver::type_id::create("driver", this);
    end
  endfunction : build_phase

  // connect_phase
  function void connect_phase(uvm_phase phase);
    if(get_is_active() == UVM_ACTIVE) begin
      driver.seq_item_port.connect(sequencer.seq_item_export);
    end
  endfunction : connect_phase

endclass : ctrl_agent


