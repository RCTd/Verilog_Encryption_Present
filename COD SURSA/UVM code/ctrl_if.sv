interface ctrl_if;

  // Control flags
  bit                has_checks = 1;
  bit                has_coverage = 1;

  // Actual Signals
  logic              clk;
  logic              reset;

  // Coverage and assertions to be implemented here.
  always @(negedge clk)
    begin

      // Reset must not be X or Z
      assert_reset_unknown:assert property (
        disable iff(!has_checks)
        !$isunknown(reset) )
        else 
          $error("ERR_RESET_XZ\n Reset went to X or Z");
        end

        endinterface : ctrl_if


