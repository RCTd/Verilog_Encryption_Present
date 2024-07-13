class ctrl_driver extends uvm_driver #(ctrl_transfer);

  // The virtual interface used to drive and view HDL signals.
  protected virtual ctrl_if ctrl_vif;

  // Provide implmentations of virtual methods such as get_type_name and create
  `uvm_component_utils(ctrl_driver)

  // new - constructor
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction : new

  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
     if(!uvm_config_db#(virtual ctrl_if)::get(this, "", "ctrl_vif", ctrl_vif))
       `uvm_fatal("NOVIF",{"virtual interface must be set for: ",get_full_name(),".ctrl_vif"});
  endfunction: build_phase

  // run phase
  virtual task run_phase(uvm_phase phase);
    fork
      get_and_drive();
    join
  endtask : run_phase

  // get_and_drive 
  virtual protected task get_and_drive();
    forever begin
      seq_item_port.get_next_item(req);
      $cast(rsp, req.clone());
      rsp.set_id_info(req);

      drive_transfer(rsp);
      
      seq_item_port.item_done();
    end
  endtask : get_and_drive

  // drive_transfer
  virtual protected task drive_transfer (ctrl_transfer trans);
    //TO DO//
    if (trans.transmit_delay > 0) begin
      repeat(trans.transmit_delay) @(negedge ctrl_vif.clk);
    end
    toggle_signal(trans);

    if(trans.transmit_hold > 0) begin
      repeat(trans.transmit_hold) @(negedge ctrl_vif.clk);
      toggle_signal(trans);
    end
  endtask : drive_transfer
      

  virtual protected task toggle_signal(ctrl_transfer trans);
    if(trans.toggle) begin
      if(trans.reset == ACTIVE_RESET) begin
        ctrl_vif.reset <= ~ctrl_vif.reset;
      end
    end
    else begin
      ctrl_vif.reset <= trans.reset;
    end
  endtask : toggle_signal
      

endclass : ctrl_driver


