#Clock signal
create_clock -period 10.000 -name sys_clk_pin -add [get_ports clk];

# Clock Pin, 100MHz input
set_property PACKAGE_PIN N11 [get_ports clk];
set_property IOSTANDARD LVCMOS33 [get_ports clk];

# DDR3 Pins, See ll7a35ek_ddr3.xdc which is used by mig tools, not here

# Flash Pins
set_property PACKAGE_PIN J13 [get_ports spi_flash_io0_io];
set_property PACKAGE_PIN J14 [get_ports spi_flash_io1_io];
#set_property PACKAGE_PIN K15 [get_ports spi_flash_io2_io];     # uncomment this line if using spix4 mode
#set_property PACKAGE_PIN K16 [get_ports spi_flash_io3_io];     # uncomment this line if using spix4 mode
set_property PACKAGE_PIN L12 [get_ports spi_flash_ss_io[0]];
set_property IOSTANDARD LVCMOS33 [get_ports {spi_flash_*}];

# Keys' Pin
set_property PACKAGE_PIN R3  [get_ports key[0]];    # PCB Designator "K4.Push"
set_property PACKAGE_PIN T3  [get_ports key[1]];    # PCB Designator "K4.Up"
set_property PACKAGE_PIN R1  [get_ports key[2]];    # PCB Designator "K4.Down"
set_property PACKAGE_PIN T2  [get_ports key[3]];    # PCB Designator "K4.Left"
set_property PACKAGE_PIN R2  [get_ports key[4]];    # PCB Designator "K4.Right"
set_property PACKAGE_PIN P1  [get_ports key[5]];    # PCB Designator "K2"
set_property PACKAGE_PIN T4  [get_ports key[6]];    # PCB Designator "K3"
set_property IOSTANDARD LVCMOS33 [get_ports {key[*]}];

# LEDs' Pin
set_property PACKAGE_PIN P5  [get_ports led[0]];
set_property PACKAGE_PIN L5  [get_ports led[1]];
set_property PACKAGE_PIN N4  [get_ports led[2]];
set_property PACKAGE_PIN P4  [get_ports led[3]];
set_property PACKAGE_PIN M4  [get_ports led[4]];
set_property PACKAGE_PIN L4  [get_ports led[5]];
set_property PACKAGE_PIN P3  [get_ports led[6]];
set_property PACKAGE_PIN N3  [get_ports led[7]];
set_property IOSTANDARD LVCMOS33 [get_ports {led[*]}];

# XADC's Vp & Vn, not use(tied ground)
#set_property PACKAGE_PIN H8 [get_ports Vp_Vn_v_p];
#set_property PACKAGE_PIN J7 [get_ports Vp_Vn_v_n];
#set_property IOSTANDARD LVCMOS33 [get_ports {Vp_Vn_v_*}];

# LCD pins
#set_property PACKAGE_PIN M2 [get_ports lcd_scl];
#set_property PACKAGE_PIN N1 [get_ports lcd_sda];
#set_property PACKAGE_PIN M1 [get_ports lcd_rstn];
#set_property IOSTANDARD LVCMOS33 [get_ports {lcd_*}];

# PA
#set_property PACKAGE_PIN B7  [get_ports pa04];  # p
#set_property PACKAGE_PIN A7  [get_ports gpio2214_sck0];  # n
#set_property PACKAGE_PIN B6  [get_ports gpio2214_sda0];  # p
#set_property PACKAGE_PIN B5  [get_ports gpio2214_clk0];  # n
#set_property PACKAGE_PIN A5  [get_ports gpio2214_addr0];  # p
#set_property PACKAGE_PIN A4  [get_ports gpio2214_sd0];  # n
#set_property PACKAGE_PIN B4  [get_ports gpio2214_int0];  # p
set_property PACKAGE_PIN A3  [get_ports gpio2214_clk1];  # n
set_property PACKAGE_PIN B2  [get_ports gpio2214_sda1];  # p
set_property PACKAGE_PIN A2  [get_ports gpio2214_sck1];  # n
#set_property PACKAGE_PIN B1  [get_ports gpio2214_sck0];  # n
set_property PACKAGE_PIN C1  [get_ports gpio2214_clk0];  # p
set_property PACKAGE_PIN D1  [get_ports gpio2214_sda0];  # n
set_property PACKAGE_PIN E2  [get_ports gpio2214_sck0];  # p
#set_property PACKAGE_PIN E1  [get_ports gpio2214_sd1];  # n
#set_property PACKAGE_PIN F2  [get_ports gpio2214_int1];  # p
#set_property PACKAGE_PIN G1  [get_ports gpio2214_addr1];  # n
#set_property PACKAGE_PIN G2  [get_ports gpio2214_clk1];  # p
#set_property PACKAGE_PIN H1  [get_ports gpio2214_sda1];  # n
#set_property PACKAGE_PIN H2  [get_ports gpio2214_sck1];  # p
#set_property PACKAGE_PIN L3  [get_ports pa44];  # p
#set_property PACKAGE_PIN L2  [get_ports pa45];  # n
#set_property PACKAGE_PIN K3  [get_ports pa47];  # p
#set_property PACKAGE_PIN K2  [get_ports pa48];  # n
#set_property PACKAGE_PIN K1  [get_ports pa50];  # p
#set_property PACKAGE_PIN J1  [get_ports pa51];  # n
#set_property PACKAGE_PIN J3  [get_ports pa53];  # p
#set_property PACKAGE_PIN H3  [get_ports pa54];  # n
#set_property PACKAGE_PIN F4  [get_ports pa56];  # p
#set_property PACKAGE_PIN F3  [get_ports pa57];  # n
#set_property PACKAGE_PIN C2  [get_ports pa64];  # n
#set_property PACKAGE_PIN C3  [get_ports pa65];  # p
#set_property PACKAGE_PIN D3  [get_ports pa67];  # n
#set_property PACKAGE_PIN E3  [get_ports pa68];  # p
#set_property PACKAGE_PIN C4  [get_ports pa70];  # n
#set_property PACKAGE_PIN D4  [get_ports pa71];  # p
#set_property PACKAGE_PIN D5  [get_ports pa73];  # n
#set_property PACKAGE_PIN D6  [get_ports pa74];  # p
#set_property PACKAGE_PIN C6  [get_ports pa76];  # n
#set_property PACKAGE_PIN C7  [get_ports pa77];  # p
#set_property IOSTANDARD LVCMOS33 [get_ports {pa*}];    # You may change this when unload JP1 and power VCCO35 by VIOA pin of PA 

# PB
set_property PACKAGE_PIN R5 [get_ports LCD_RD]
set_property PACKAGE_PIN T5 [get_ports LCD_RSTN]
set_property PACKAGE_PIN T7 [get_ports {LCD_DATA[0]}]
set_property PACKAGE_PIN T8 [get_ports {LCD_DATA[1]}]
set_property PACKAGE_PIN T9 [get_ports {LCD_DATA[2]}]
set_property PACKAGE_PIN T10 [get_ports {LCD_DATA[3]}]
set_property PACKAGE_PIN R12 [get_ports {LCD_DATA[4]}]
set_property PACKAGE_PIN T12 [get_ports {LCD_DATA[5]}]
set_property PACKAGE_PIN R13 [get_ports {LCD_DATA[6]}]
set_property PACKAGE_PIN T13 [get_ports {LCD_DATA[7]}]
set_property PACKAGE_PIN T15 [get_ports {LCD_DATA[8]}]
set_property PACKAGE_PIN T14 [get_ports {LCD_DATA[9]}]
set_property PACKAGE_PIN R16 [get_ports {LCD_DATA[10]}]
set_property PACKAGE_PIN R15 [get_ports {LCD_DATA[11]}]
set_property PACKAGE_PIN P16 [get_ports {LCD_DATA[12]}]
set_property PACKAGE_PIN P15 [get_ports {LCD_DATA[13]}]
set_property PACKAGE_PIN N16 [get_ports {LCD_DATA[14]}]
set_property PACKAGE_PIN M16 [get_ports {LCD_DATA[15]}]
set_property PACKAGE_PIN K13 [get_ports touch_sda];  # p
set_property PACKAGE_PIN L13 [get_ports touch_int];  # n
set_property PACKAGE_PIN L14 [get_ports touch_rst];  # p
set_property PACKAGE_PIN M14 [get_ports touch_sck];  # n
#set_property PACKAGE_PIN N13 [get_ports pb50];  # p
#set_property PACKAGE_PIN P13 [get_ports pb51];  # n
#set_property PACKAGE_PIN N14 [get_ports pb53];  # p
#set_property PACKAGE_PIN P14 [get_ports pb54];  # n
#set_property PACKAGE_PIN P10 [get_ports pb56];  # p
#set_property PACKAGE_PIN P11 [get_ports pb57];  # n
#set_property PACKAGE_PIN R11 [get_ports pb64];  # n
#set_property PACKAGE_PIN R10 [get_ports pb65];  # p
#set_property PACKAGE_PIN P9  [get_ports pb67];  # n
#set_property PACKAGE_PIN N9  [get_ports pb68];  # p
#set_property PACKAGE_PIN R8  [get_ports pb70];  # n
#set_property PACKAGE_PIN P8  [get_ports pb71];  # p
#set_property PACKAGE_PIN R7  [get_ports pb73];  # n
set_property PACKAGE_PIN R6 [get_ports LCD_CS]
set_property PACKAGE_PIN N6 [get_ports LCD_RS]
set_property PACKAGE_PIN M6 [get_ports LCD_WR]
#set_property IOSTANDARD LVCMOS33 [get_ports {pb*}];

# IO Bank 14 default 3.3V.
set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 14]];
    
# IO Bank 34 default 3.3V.
set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 34]];

# IO Bank 35 default 3.3V. You may change this when unloading JP1 and powering VCCO35 by VIOA pin of PA 
set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 35]];