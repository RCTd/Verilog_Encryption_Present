
class cmd_driver extends uvm_driver #(cmd_transfer);

  // The virtual interface used to drive and view HDL signals.
  protected virtual cmd_if cmd_vif;

  // Provide implmentations of virtual methods such as get_type_name and create
  `uvm_component_utils(cmd_driver)

  // new - constructor
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction : new

  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
     if(!uvm_config_db#(virtual cmd_if)::get(this, "", "cmd_vif", cmd_vif))
       `uvm_fatal("NOVIF",{"virtual interface must be set for: ",get_full_name(),".cmd_vif"});
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
      @(posedge cmd_vif.clk);
      cmd_vif.pl   <= 2'b0;
      seq_item_port.get_next_item(req);
      $cast(rsp, req.clone());
      rsp.set_id_info(req);

      drive_transfer(rsp);
      
      seq_item_port.item_done();
      seq_item_port.put_response(rsp);
    end
  endtask : get_and_drive

  // drive_transfer
  virtual protected task drive_transfer (cmd_transfer trans);
    //TO DO//
    if (trans.transmit_delay > 0) begin
      repeat(trans.transmit_delay) @(posedge cmd_vif.clk);
    end
    cmd_vif.in <= trans.data;
    case (trans.data_type)
      TEXT:  cmd_vif.pl[TEXT] <= 1'b1;
      KEY:   cmd_vif.pl[KEY] <= 1'b1;
    endcase

  endtask : drive_transfer

endclass : cmd_driver


