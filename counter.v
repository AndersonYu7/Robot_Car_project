module counter (
    input clk,
    input reset_n,
    input clk_cnt_i,
    input clk_stop_i,
    output reg [31:0] count,
    output reg [31:0] l_count
);

reg stop_count = 0;

always @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
        count <= 32'b0;
        l_count <= 32'b0;
        stop_count <= 1'b0;
    end else begin
        if (!stop_count) begin
            if (clk_cnt_i) begin
                count <= count + 32'b1;
            end
            if (clk_stop_i) begin
                l_count <= count;
                stop_count <= 1'b1;
            end
        end
    end
end

endmodule
